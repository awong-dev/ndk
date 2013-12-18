//===------------------------- UnwindLevel1.c -----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//
// Implements C++ ABI Exception Handling Level 1 as documented at:
//      http://mentorembedded.github.io/cxx-abi/abi-eh.html
// using libunwind
//
//===----------------------------------------------------------------------===//

#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libunwind.h"
#include "unwind.h"
#include "config.h"

#if _LIBUNWIND_BUILD_ZERO_COST_APIS

static _Unwind_Reason_Code
unwind_phase1(unw_context_t *uc, struct _Unwind_Exception *exception_object) {
  // EHABI #7.3 discusses preserving the VRS in a "temporary VRS" during
  // phase 1 and then restoring it to the "primary VRS" for phase 2. The
  // effect is phase 2 doesn't see any of the VRS manipulations from phase 1.
  // In this implementation, the phases don't share the VRS backing store.
  // Instead, they are passed the original |uc| and they create a new VRS
  // from scratch thus achieving the same effect.
  unw_cursor_t cursor1;
  unw_init_local(&cursor1, uc);

  // Walk each frame looking for a place to stop.
  for (bool handlerNotFound = true; handlerNotFound;) {

    // Ask libuwind to get next frame (skip over first which is
    // _Unwind_RaiseException).
    int stepResult = unw_step(&cursor1);
    if (stepResult == 0) {
      _LIBUNWIND_TRACE_UNWINDING("unwind_phase1(ex_ojb=%p): unw_step() reached "
                            "bottom => _URC_END_OF_STACK\n",
                            exception_object);
      return _URC_END_OF_STACK;
    } else if (stepResult < 0) {
      _LIBUNWIND_TRACE_UNWINDING("unwind_phase1(ex_ojb=%p): unw_step failed => "
                            "_URC_FATAL_PHASE1_ERROR\n",
                            exception_object);
      return _URC_FATAL_PHASE1_ERROR;
    }

    // See if frame has code to run (has personality routine).
    unw_proc_info_t frameInfo;
    unw_word_t sp;
    if (unw_get_proc_info(&cursor1, &frameInfo) != UNW_ESUCCESS) {
      _LIBUNWIND_TRACE_UNWINDING("unwind_phase1(ex_ojb=%p): unw_get_proc_info "
                            "failed => _URC_FATAL_PHASE1_ERROR\n",
                            exception_object);
      return _URC_FATAL_PHASE1_ERROR;
    }

    // When tracing, print state information.
    if (_LIBUNWIND_TRACING_UNWINDING) {
      char functionName[512];
      unw_word_t offset;
      if ((unw_get_proc_name(&cursor1, functionName, 512, &offset) !=
           UNW_ESUCCESS) || (frameInfo.start_ip + offset > frameInfo.end_ip))
        strcpy(functionName, ".anonymous.");
      unw_word_t pc;
      unw_get_reg(&cursor1, UNW_REG_IP, &pc);
      _LIBUNWIND_TRACE_UNWINDING(
          "unwind_phase1(ex_ojb=%p): pc=0x%llX, start_ip=0x%llX, func=%s, "
          "lsda=0x%llX, personality=0x%llX\n",
          exception_object, pc, frameInfo.start_ip, functionName,
          frameInfo.lsda, frameInfo.handler);
    }

    // If there is a personality routine, ask it if it will want to stop at
    // this frame.
    if (frameInfo.handler != 0) {
      __personality_routine p =
          (__personality_routine)(long)(frameInfo.handler);
      _LIBUNWIND_TRACE_UNWINDING(
          "unwind_phase1(ex_ojb=%p): calling personality function %p\n",
          exception_object, p);
      struct _Unwind_Context *context = (struct _Unwind_Context *)(&cursor1);
#ifdef __arm__
      exception_object->pr_cache.fnstart = frameInfo.start_ip;
      exception_object->pr_cache.ehtp = (_Unwind_EHT_Header *)frameInfo.unwind_info;
      exception_object->pr_cache.additional = frameInfo.flags;
      _Unwind_Reason_Code personalityResult =
          (*p)(_US_VIRTUAL_UNWIND_FRAME, exception_object, context);
#else
      _Unwind_Reason_Code personalityResult =
          (*p)(1, _UA_SEARCH_PHASE, exception_object->exception_class,
               exception_object, context);
#endif
      switch (personalityResult) {
      case _URC_HANDLER_FOUND:
        // found a catch clause or locals that need destructing in this frame
        // stop search and remember stack pointer at the frame
        handlerNotFound = false;
#ifndef __arm__
        unw_get_reg(&cursor1, UNW_REG_SP, &sp);
        exception_object->private_2 = (uintptr_t)sp;
#else
        // p should have initialized barrier_cache. #7.3.5
#endif
        _LIBUNWIND_TRACE_UNWINDING("unwind_phase1(ex_ojb=%p): "
                                   "_URC_HANDLER_FOUND \n",
                                   exception_object);
        return _URC_NO_REASON;

      case _URC_CONTINUE_UNWIND:
        _LIBUNWIND_TRACE_UNWINDING(
            "unwind_phase1(ex_ojb=%p): _URC_CONTINUE_UNWIND\n",
            exception_object);
        // continue unwinding
        break;

#ifdef __arm__
      // # 7.3.3
      case _URC_FAILURE:
        return _URC_FAILURE;
#endif

      default:
        // something went wrong
        _LIBUNWIND_TRACE_UNWINDING(
            "unwind_phase1(ex_ojb=%p): _URC_FATAL_PHASE1_ERROR\n",
            exception_object);
        return _URC_FATAL_PHASE1_ERROR;
      }
    }
  }
  return _URC_NO_REASON;
}


static _Unwind_Reason_Code
unwind_phase2(unw_context_t *uc, struct _Unwind_Exception *exception_object, bool resume) {
  // See comment at the start of unwind_phase1 regarding VRS integrity.
  unw_cursor_t cursor2;
  unw_init_local(&cursor2, uc);

  _LIBUNWIND_TRACE_UNWINDING("unwind_phase2(ex_ojb=%p)\n", exception_object);
  int frame_count = 0;

  // Walk each frame until we reach where search phase said to stop.
  while (true) {
    // Ask libuwind to get next frame (skip over first which is
    // _Unwind_RaiseException or _Unwind_Resume).
    int stepResult = unw_step(&cursor2);
    if (stepResult == 0) {
      _LIBUNWIND_TRACE_UNWINDING("unwind_phase2(ex_ojb=%p): unw_step() reached "
                            "bottom => _URC_END_OF_STACK\n",
                            exception_object);
      return _URC_END_OF_STACK;
    } else if (stepResult < 0) {
      _LIBUNWIND_TRACE_UNWINDING("unwind_phase2(ex_ojb=%p): unw_step failed => "
                            "_URC_FATAL_PHASE1_ERROR\n",
                            exception_object);
      return _URC_FATAL_PHASE2_ERROR;
    }

#ifdef __arm__
    // #7.4.1 says we need to preserve pc for when _Unwind_Resume is called
    // back, to find this same frame.
    unw_word_t pc;
    unw_get_reg(&cursor2, UNW_REG_IP, &pc);
    exception_object->unwinder_cache.reserved2 = (uint32_t)pc;
#endif

    // Get info about this frame.
    unw_word_t sp;
    unw_proc_info_t frameInfo;
    unw_get_reg(&cursor2, UNW_REG_SP, &sp);
    if (unw_get_proc_info(&cursor2, &frameInfo) != UNW_ESUCCESS) {
      _LIBUNWIND_TRACE_UNWINDING("unwind_phase2(ex_ojb=%p): unw_get_proc_info "
                            "failed => _URC_FATAL_PHASE1_ERROR\n",
                            exception_object);
      return _URC_FATAL_PHASE2_ERROR;
    }

    // When tracing, print state information.
    if (_LIBUNWIND_TRACING_UNWINDING) {
      char functionName[512];
      unw_word_t offset;
      if ((unw_get_proc_name(&cursor2, functionName, 512, &offset) !=
           UNW_ESUCCESS) || (frameInfo.start_ip + offset > frameInfo.end_ip))
        strcpy(functionName, ".anonymous.");
      _LIBUNWIND_TRACE_UNWINDING(
          "unwind_phase2(ex_ojb=%p): start_ip=0x%llX, func=%s, sp=0x%llX, "
          "lsda=0x%llX, personality=0x%llX\n",
          exception_object, frameInfo.start_ip, functionName, sp,
          frameInfo.lsda, frameInfo.handler);
    }

    // If there is a personality routine, tell it we are unwinding.
    if (frameInfo.handler != 0) {
      __personality_routine p =
          (__personality_routine)(long)(frameInfo.handler);
      struct _Unwind_Context *context = (struct _Unwind_Context *)(&cursor2);
#ifdef __arm__
      exception_object->pr_cache.fnstart = frameInfo.start_ip;
      exception_object->pr_cache.ehtp = (_Unwind_EHT_Header *)frameInfo.unwind_info;
      exception_object->pr_cache.additional = frameInfo.flags;
      _Unwind_State state =
          resume ? _US_UNWIND_FRAME_RESUME : _US_UNWIND_FRAME_STARTING;
      // Resume only ever makes sense for 1 frame.
      _Unwind_Reason_Code personalityResult =
          (*p)(state, exception_object, context);
      if (resume && frame_count == 1) {
        // On a resume, first unwind the _Unwind_Resume() frame. The next frame
        // is now the landing pad for the cleanup from a previous execution of
        // phase2. To continue unwindingly correctly, replace VRS[15] with the
        // IP of the frame that the previous run of phase2 installed the context
        // for. After this, continue unwinding as if normal.
        //
        // See #7.4.6 for details.
        unw_set_reg(&cursor2, UNW_REG_IP, exception_object->unwinder_cache.reserved2);
        resume = false;
      }
#else
      _Unwind_Action action = _UA_CLEANUP_PHASE;
      if (sp == exception_object->private_2) {
        // Tell personality this was the frame it marked in phase 1.
        action = (_Unwind_Action)(_UA_CLEANUP_PHASE | _UA_HANDLER_FRAME);
      }
       _Unwind_Reason_Code personalityResult =
          (*p)(1, action, exception_object->exception_class, exception_object,
               context);
#endif
      switch (personalityResult) {
      case _URC_CONTINUE_UNWIND:
        // Continue unwinding
        _LIBUNWIND_TRACE_UNWINDING(
            "unwind_phase2(ex_ojb=%p): _URC_CONTINUE_UNWIND\n",
            exception_object);
#ifdef __arm__
        // TODO(piman) do we want any check here?
#else
        if (sp == exception_object->private_2) {
          // Phase 1 said we would stop at this frame, but we did not...
          _LIBUNWIND_ABORT("during phase1 personality function said it would "
                           "stop here, but now if phase2 it did not stop here");
        }
#endif
        break;
      case _URC_INSTALL_CONTEXT:
        _LIBUNWIND_TRACE_UNWINDING(
            "unwind_phase2(ex_ojb=%p): _URC_INSTALL_CONTEXT\n",
            exception_object);
        // Personality routine says to transfer control to landing pad.
        // We may get control back if landing pad calls _Unwind_Resume().
        if (_LIBUNWIND_TRACING_UNWINDING) {
          unw_word_t pc;
          unw_get_reg(&cursor2, UNW_REG_IP, &pc);
          unw_get_reg(&cursor2, UNW_REG_SP, &sp);
          _LIBUNWIND_TRACE_UNWINDING("unwind_phase2(ex_ojb=%p): re-entering  "
                                     "user code with ip=0x%llX, sp=0x%llX\n",
                                    exception_object, pc, sp);
        }
        unw_resume(&cursor2);
        // unw_resume() only returns if there was an error.
        return _URC_FATAL_PHASE2_ERROR;
#ifdef __arm__
      // # 7.4.3
      case _URC_FAILURE:
        abort();
//        return _URC_FAILURE;
#endif
      default:
        // Personality routine returned an unknown result code.
        _LIBUNWIND_DEBUG_LOG("personality function returned unknown result %d",
                      personalityResult);
        return _URC_FATAL_PHASE2_ERROR;
      }
    }
    frame_count++;
  }

  // Clean up phase did not resume at the frame that the search phase
  // said it would...
  return _URC_FATAL_PHASE2_ERROR;
}

#ifndef __arm__
static _Unwind_Reason_Code
unwind_phase2_forced(unw_context_t *uc,
                     struct _Unwind_Exception *exception_object,
                     _Unwind_Stop_Fn stop, void *stop_parameter) {
  unw_cursor_t cursor2;
  unw_init_local(&cursor2, uc);

  // Walk each frame until we reach where search phase said to stop
  while (unw_step(&cursor2) > 0) {

    // Update info about this frame.
    unw_proc_info_t frameInfo;
    if (unw_get_proc_info(&cursor2, &frameInfo) != UNW_ESUCCESS) {
      _LIBUNWIND_TRACE_UNWINDING("unwind_phase2_forced(ex_ojb=%p): unw_step "
                                 "failed => _URC_END_OF_STACK\n",
                                 exception_object);
      return _URC_FATAL_PHASE1_ERROR;
    }

    // When tracing, print state information.
    if (_LIBUNWIND_TRACING_UNWINDING) {
      char functionName[512];
      unw_word_t offset;
      if ((unw_get_proc_name(&cursor2, functionName, 512, &offset) !=
           UNW_ESUCCESS) || (frameInfo.start_ip + offset > frameInfo.end_ip))
        strcpy(functionName, ".anonymous.");
      _LIBUNWIND_TRACE_UNWINDING("unwind_phase2_forced(ex_ojb=%p):  "
                                 "start_ip=0x%llX, func=%s, lsda=0x%llX, "
                                 " personality=0x%llX\n",
                                 exception_object, frameInfo.start_ip,
                                 functionName, frameInfo.lsda,
                                 frameInfo.handler);
    }

    // Call stop function at each frame.
    _Unwind_Action action =
        (_Unwind_Action)(_UA_FORCE_UNWIND | _UA_CLEANUP_PHASE);
    _Unwind_Reason_Code stopResult =
        (*stop)(1, action, exception_object->exception_class, exception_object,
                (struct _Unwind_Context *)(&cursor2), stop_parameter);
    _LIBUNWIND_TRACE_UNWINDING(
        "unwind_phase2_forced(ex_ojb=%p): stop function returned %d\n",
        exception_object, stopResult);
    if (stopResult != _URC_NO_REASON) {
      _LIBUNWIND_TRACE_UNWINDING(
          "unwind_phase2_forced(ex_ojb=%p): stopped by stop function\n",
          exception_object);
      return _URC_FATAL_PHASE2_ERROR;
    }

    // If there is a personality routine, tell it we are unwinding.
    if (frameInfo.handler != 0) {
      __personality_routine p =
          (__personality_routine)(long)(frameInfo.handler);
      _LIBUNWIND_TRACE_UNWINDING(
          "unwind_phase2_forced(ex_ojb=%p): calling personality function %p\n",
          exception_object, p);
      _Unwind_Reason_Code personalityResult =
          (*p)(1, action, exception_object->exception_class, exception_object,
               (struct _Unwind_Context *)(&cursor2));
      switch (personalityResult) {
      case _URC_CONTINUE_UNWIND:
        _LIBUNWIND_TRACE_UNWINDING("unwind_phase2_forced(ex_ojb=%p): "
                                "personality  returned _URC_CONTINUE_UNWIND\n",
                                 exception_object);
        // Destructors called, continue unwinding
        break;
      case _URC_INSTALL_CONTEXT:
        _LIBUNWIND_TRACE_UNWINDING("unwind_phase2_forced(ex_ojb=%p): "
                                  "personality returned _URC_INSTALL_CONTEXT\n",
                                   exception_object);
        // We may get control back if landing pad calls _Unwind_Resume().
        unw_resume(&cursor2);
        break;
      default:
        // Personality routine returned an unknown result code.
        _LIBUNWIND_TRACE_UNWINDING("unwind_phase2_forced(ex_ojb=%p): "
                                   "personality returned %d, "
                                   "_URC_FATAL_PHASE2_ERROR\n",
                                   exception_object, personalityResult);
        return _URC_FATAL_PHASE2_ERROR;
      }
    }
  }

  // Call stop function one last time and tell it we've reached the end
  // of the stack.
  _LIBUNWIND_TRACE_UNWINDING("unwind_phase2_forced(ex_ojb=%p): calling stop "
                        "function with _UA_END_OF_STACK\n",
                        exception_object);
  _Unwind_Action lastAction =
      (_Unwind_Action)(_UA_FORCE_UNWIND | _UA_CLEANUP_PHASE | _UA_END_OF_STACK);
  (*stop)(1, lastAction, exception_object->exception_class, exception_object,
          (struct _Unwind_Context *)(&cursor2), stop_parameter);

  // Clean up phase did not resume at the frame that the search phase said it
  // would.
  return _URC_FATAL_PHASE2_ERROR;
}
#endif


/// Called by __cxa_throw.  Only returns if there is a fatal error.
_LIBUNWIND_EXPORT _Unwind_Reason_Code
_Unwind_RaiseException(struct _Unwind_Exception *exception_object) {
  _LIBUNWIND_TRACE_API("_Unwind_RaiseException(ex_obj=%p)\n",
                             exception_object);
  unw_context_t uc;
  unw_getcontext(&uc);

#ifdef __arm__
  exception_object->unwinder_cache.reserved1 = 0;
#else
  // Mark that this is a non-forced unwind, so _Unwind_Resume()
  // can do the right thing.
  exception_object->private_1 = 0;
  exception_object->private_2 = 0;
#endif

  // phase 1: the search phase
  _Unwind_Reason_Code phase1 = unwind_phase1(&uc, exception_object);
  if (phase1 != _URC_NO_REASON)
    return phase1;

  // phase 2: the clean up phase
  return unwind_phase2(&uc, exception_object, false);
}



/// When _Unwind_RaiseException() is in phase2, it hands control
/// to the personality function at each frame.  The personality
/// may force a jump to a landing pad in that function, the landing
/// pad code may then call _Unwind_Resume() to continue with the
/// unwinding.  Note: the call to _Unwind_Resume() is from compiler
/// geneated user code.  All other _Unwind_* routines are called
/// by the C++ runtime __cxa_* routines.
///
/// Note: re-throwing an exception (as opposed to continuing the unwind)
/// is implemented by having the code call __cxa_rethrow() which
/// in turn calls _Unwind_Resume_or_Rethrow().
_LIBUNWIND_EXPORT void
_Unwind_Resume(struct _Unwind_Exception *exception_object) {
  _LIBUNWIND_TRACE_API("_Unwind_Resume(ex_obj=%p)\n", exception_object);
  unw_context_t uc;
  unw_getcontext(&uc);

#ifdef __arm__
  // TODO(piman): Do we need a "force unwind" mechanism?
#else
  if (exception_object->private_1 != 0)
    unwind_phase2_forced(&uc, exception_object,
                         (_Unwind_Stop_Fn) exception_object->private_1,
                         (void *)exception_object->private_2);
  else
#endif
    unwind_phase2(&uc, exception_object, true);

  // Clients assume _Unwind_Resume() does not return, so all we can do is abort.
  _LIBUNWIND_ABORT("_Unwind_Resume() can't return");
}



#ifndef __arm__
/// Not used by C++.
/// Unwinds stack, calling "stop" function at each frame.
/// Could be used to implement longjmp().
_LIBUNWIND_EXPORT _Unwind_Reason_Code
_Unwind_ForcedUnwind(struct _Unwind_Exception *exception_object,
                     _Unwind_Stop_Fn stop, void *stop_parameter) {
  _LIBUNWIND_TRACE_API("_Unwind_ForcedUnwind(ex_obj=%p, stop=%p)\n",
                  exception_object, stop);
  unw_context_t uc;
  unw_getcontext(&uc);

  // Mark that this is a forced unwind, so _Unwind_Resume() can do
  // the right thing.
  exception_object->private_1 = (uintptr_t) stop;
  exception_object->private_2 = (uintptr_t) stop_parameter;

  // do it
  return unwind_phase2_forced(&uc, exception_object, stop, stop_parameter);
}
#endif


/// Called by personality handler during phase 2 to get LSDA for current frame.
_LIBUNWIND_EXPORT uintptr_t
_Unwind_GetLanguageSpecificData(struct _Unwind_Context *context) {
  unw_cursor_t *cursor = (unw_cursor_t *)context;
  unw_proc_info_t frameInfo;
  uintptr_t result = 0;
  if (unw_get_proc_info(cursor, &frameInfo) == UNW_ESUCCESS)
    result = (uintptr_t)frameInfo.lsda;
  _LIBUNWIND_TRACE_API("_Unwind_GetLanguageSpecificData(context=%p)"
                             "=> 0x%" PRIXPTR "\n", context, result);
  if (result != 0) {
    if (*((uint8_t *)result) != 0xFF)
      _LIBUNWIND_DEBUG_LOG("lsda at 0x%" PRIXPTR " does not start with 0xFF\n", result);
  }
  return result;
}


#if __arm__ && !CXXABI_SJLJ

_Unwind_VRS_Result _Unwind_VRS_Set(
    _Unwind_Context *context,
    _Unwind_VRS_RegClass regclass,
    uint32_t regno,
    _Unwind_VRS_DataRepresentation representation,
    void *valuep) {
  _LIBUNWIND_TRACE_API("_Unwind_SetGR(context=%p, regclass=%d reg=%d, rep=%d, "
                             "value=0x%0llX)\n", context, regclass,
                             regno, representation, 0LL /* FIXME */);
  unw_cursor_t *cursor = (unw_cursor_t *)context;
  switch (regclass) {
    case _UVRSC_CORE: {
      if (representation != _UVRSD_UINT32)
        _LIBUNWIND_ABORT("Core register representation must be _UVRSD_UINT32.");
      return unw_set_reg(cursor, regno, *((unw_word_t*)valuep)) == UNW_ESUCCESS ?
          _UVRSR_OK : _UVRSR_FAILED;
    }
    // FIXME: Are these right? See 4.7 on lazy-saving
    case _UVRSC_WMMXD:
    case _UVRSC_WMMXC:
    case _UVRSC_VFP: {
      unw_fpreg_t value;
      switch (representation) {
        case _UVRSD_VFPX:
          // TODO(ajwong): What does this mean?
          break;
        case _UVRSD_UINT64: {
          uint64_t tmp = *(uint64_t*)valuep;
          memcpy(&value, &tmp, sizeof(tmp));
          break;
        }
        case _UVRSD_FLOAT: {
          float tmp = *(float*)valuep;
          memcpy(&value, &tmp, sizeof(tmp));
          break;
        }
        case _UVRSD_DOUBLE: {
          double tmp = *(double*)valuep;
          memcpy(&value, &tmp, sizeof(tmp));
          break;
        }
        case _UVRSD_UINT32: {
          uint32_t tmp = *(uint32_t*)valuep;
          memcpy(&value, &tmp, sizeof(tmp));
          break;
        }
        default:
          _LIBUNWIND_ABORT("Invalid VFP data representation.");
      }
      return unw_set_fpreg(cursor, regno, value) == UNW_ESUCCESS ?
          _UVRSR_OK : _UVRSR_FAILED;
    }
  }
  return _UVRSR_NOT_IMPLEMENTED;
}

_Unwind_VRS_Result _Unwind_VRS_Get(
    _Unwind_Context *context,
    _Unwind_VRS_RegClass regclass,
    uint32_t regno,
    _Unwind_VRS_DataRepresentation representation,
    void *valuep) {
  _LIBUNWIND_TRACE_API("_Unwind_SetGR(context=%p, regclass=%d reg=%d, rep=%d, "
                             "value=0x%0llX)\n", context, regclass,
                             regno, representation, 0LL /* FIXME */);
  unw_cursor_t *cursor = (unw_cursor_t *)context;
  switch (regclass) {
    case _UVRSC_CORE: {
      if (representation != _UVRSD_UINT32)
        _LIBUNWIND_ABORT("Core register representation must be _UVRSD_UINT32.");
      return unw_get_reg(cursor, regno, (unw_word_t*)valuep) == UNW_ESUCCESS ?
          _UVRSR_OK : _UVRSR_FAILED;
    }
    // FIXME: Are these right? See 4.7 on lazy-saving
    case _UVRSC_WMMXD:
    case _UVRSC_WMMXC:
    case _UVRSC_VFP: {
      unw_fpreg_t value;
      if (unw_get_fpreg(cursor, regno, &value) != UNW_ESUCCESS)
        return _UVRSR_FAILED;
      switch (representation) {
        case _UVRSD_VFPX:
          // TODO(ajwong): What does this mean?
          break;
        case _UVRSD_UINT64: {
          uint64_t tmp;
          memcpy(&tmp, &value, sizeof(tmp));
          *(uint64_t*)valuep = tmp;
          break;
        }
        case _UVRSD_FLOAT: {
          float tmp;
          memcpy(&tmp, &value, sizeof(tmp));
          *(float*)valuep = tmp;
          break;
        }
        case _UVRSD_DOUBLE: {
          double tmp;
          memcpy(&tmp, &value, sizeof(tmp));
          *(double*)valuep = tmp;
          break;
        }
        case _UVRSD_UINT32: {
          uint32_t tmp;
          memcpy(&tmp, &value, sizeof(tmp));
          *(uint32_t*)valuep = tmp;
          break;
        }
        default:
          _LIBUNWIND_ABORT("Invalid VFP data representation.");
      }
      return _UVRSR_OK;
    }
  }
  return _UVRSR_NOT_IMPLEMENTED;
}

_Unwind_VRS_Result _Unwind_VRS_Pop(
    _Unwind_Context *context,
    _Unwind_VRS_RegClass regclass,
    uint32_t discriminator,
    _Unwind_VRS_DataRepresentation representation) {
  if (regclass != _UVRSC_CORE || representation != _UVRSD_UINT32) {
    // TODO(piman): VFP, ...
    _LIBUNWIND_ABORT("during phase1 personality function said it would "
                     "stop here, but now if phase2 it did not stop here");
    return _UVRSR_NOT_IMPLEMENTED;
  }
  bool do13 = false;
  uint32_t reg13Value = 0;
  uint32_t* sp;
  if (_Unwind_VRS_Get(context, _UVRSC_CORE, UNW_ARM_SP,
                      _UVRSD_UINT32, &sp) != _UVRSR_OK) {
    return _UVRSR_FAILED;
  }
  for (int i = 0; i < 16; ++i) {
    if (!(discriminator & (1<<i)))
      continue;
    uint32_t value = *sp++;
    if (i == 13) {
      reg13Value = value;
      do13 = true;
    } else {
      if (_Unwind_VRS_Set(context, _UVRSC_CORE, UNW_ARM_R0 + i,
                          _UVRSD_UINT32, &value) != _UVRSR_OK) {
        return _UVRSR_FAILED;
      }
    }
  }
  if (do13) {
    return _Unwind_VRS_Set(context, _UVRSC_CORE, UNW_ARM_SP,
                           _UVRSD_UINT32, &reg13Value);
  } else {
    return _Unwind_VRS_Set(context, _UVRSC_CORE, UNW_ARM_SP,
                        _UVRSD_UINT32, &sp);
  }
}

#else

/// Called by personality handler during phase 2 to get register values.
_LIBUNWIND_EXPORT uintptr_t _Unwind_GetGR(struct _Unwind_Context *context,
                                          int index) {
  unw_cursor_t *cursor = (unw_cursor_t *)context;
  unw_word_t result;
  unw_get_reg(cursor, index, &result);
  _LIBUNWIND_TRACE_API("_Unwind_GetGR(context=%p, reg=%d) => 0x%llX\n",
    context,
                  index, (uint64_t) result);
  return (uintptr_t)result;
}



/// Called by personality handler during phase 2 to alter register values.
_LIBUNWIND_EXPORT void _Unwind_SetGR(struct _Unwind_Context *context, int index,
                                     uintptr_t new_value) {
  _LIBUNWIND_TRACE_API("_Unwind_SetGR(context=%p, reg=%d, "
                             "value=0x%0llX)\n", context,
                             index, (uint64_t) new_value);
  unw_cursor_t *cursor = (unw_cursor_t *)context;
  unw_set_reg(cursor, index, new_value);
}

#endif  // __arm__ && !CXXABI_SJLJ

/// Called by personality handler during phase 2 to get instruction pointer.
_LIBUNWIND_EXPORT uintptr_t _Unwind_GetIP(struct _Unwind_Context *context) {
  unw_cursor_t *cursor = (unw_cursor_t *)context;
  unw_word_t result;
  unw_get_reg(cursor, UNW_REG_IP, &result);
  _LIBUNWIND_TRACE_API("_Unwind_GetIP(context=%p) => 0x%llX\n", context,
                             (uint64_t) result);
  return (uintptr_t)result;
}



/// Called by personality handler during phase 2 to alter instruction pointer,
/// such as setting where the landing pad is, so _Unwind_Resume() will
/// start executing in the landing pad.
_LIBUNWIND_EXPORT void _Unwind_SetIP(struct _Unwind_Context *context,
                                     uintptr_t new_value) {
  _LIBUNWIND_TRACE_API("_Unwind_SetIP(context=%p, value=0x%0llX)\n",
                             context, (uint64_t) new_value);
  unw_cursor_t *cursor = (unw_cursor_t *)context;
  unw_set_reg(cursor, UNW_REG_IP, new_value);
}


/// Called by personality handler during phase 2 to find the start of the
/// function.
_LIBUNWIND_EXPORT uintptr_t
_Unwind_GetRegionStart(struct _Unwind_Context *context) {
  unw_cursor_t *cursor = (unw_cursor_t *)context;
  unw_proc_info_t frameInfo;
  uintptr_t result = 0;
  if (unw_get_proc_info(cursor, &frameInfo) == UNW_ESUCCESS)
    result = (uintptr_t)frameInfo.start_ip;
  _LIBUNWIND_TRACE_API("_Unwind_GetRegionStart(context=%p) => 0x%" PRIXPTR "\n",
                             context, result);
  return result;
}


/// Called by personality handler during phase 2 if a foreign exception
// is caught.
_LIBUNWIND_EXPORT void
_Unwind_DeleteException(struct _Unwind_Exception *exception_object) {
  _LIBUNWIND_TRACE_API("_Unwind_DeleteException(ex_obj=%p)\n",
                              exception_object);
  if (exception_object->exception_cleanup != NULL)
    (*exception_object->exception_cleanup)(_URC_FOREIGN_EXCEPTION_CAUGHT,
                                           exception_object);
}

#endif // _LIBUNWIND_BUILD_ZERO_COST_APIS
