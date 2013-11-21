//===--------------------------- Unwind-sjlj.c ----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//
//  Implements setjump-longjump based C++ exceptions
//
//===----------------------------------------------------------------------===//

#include <unwind.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "config.h"
#include "libunwind.h"

#if __arm__ && !CXXABI_SJLJ

typedef uint32_t _Unwind_State;
static const _Unwind_State _US_VIRTUAL_UNWIND_FRAME = 0; 
static const _Unwind_State _US_UNWIND_FRAME_STARTING = 1; 
static const _Unwind_State _US_UNWIND_FRAME_RESUME = 2; 
 
typedef struct _Unwind_Context _Unwind_Context; 
typedef uint32_t _Unwind_EHT_Header; 

typedef struct _Unwind_Control_Block {
  char exception_class[8];
  void (*exception_cleanup)(_Unwind_Reason_Code, struct _Unwind_Control_Block *);
  /* Unwinder cache, private fields for the unwinder's use */
  struct {
    uint32_t reserved1; /* init reserved1 to 0, then don't touch */
    uint32_t reserved2;
    uint32_t reserved3;
    uint32_t reserved4;
    uint32_t reserved5;
  } unwinder_cache;
  /* Propagation barrier cache (valid after phase 1): */
  struct {
    uint32_t sp;
    uint32_t bitpattern[5];
  } barrier_cache;
  /* Cleanup cache (preserved over cleanup): */
  struct {
    uint32_t bitpattern[4];
  } cleanup_cache;
  /* Pr cache (for pr's benefit): */
  struct {
    uint32_t fnstart; /* function start address */
    _Unwind_EHT_Header *ehtp; /* pointer to EHT entry header word */
    uint32_t additional; /* additional data */
    uint32_t reserved1;
  } pr_cache;
  long long int :0; /* Force alignment of next item to 8-byte boundary */
} _Unwind_Control_Block;

_Unwind_Reason_Code __aeabi_unwind_cpp_pr0(_Unwind_State state,
                                           _Unwind_Control_Block *ucbp,
                                           _Unwind_Context *context) {
#warning TODO(danakj): Support ARM/NEON here.
  assert(false && "pr0 not supported");
  return _URC_FAILURE;
}

_Unwind_Reason_Code __aeabi_unwind_cpp_pr1(_Unwind_State state,
                                           _Unwind_Control_Block *ucbp,
                                           _Unwind_Context *context) {
#warning TODO(danakj): Support ARM/NEON here.
  assert(false && "pr1 not supported");
  return _URC_FAILURE;
}

_Unwind_Reason_Code __aeabi_unwind_cpp_pr2(_Unwind_State state,
                                           _Unwind_Control_Block *ucbp,
                                           _Unwind_Context *context) {
#warning TODO(danakj): Support ARM/NEON here.
  assert(false && "pr2 not supported");
  return _URC_FAILURE;
}

int unw_getcontext(unw_context_t* thread_state) {
#warning TODO(danakj): Support ARM/NEON here.
  assert(false && "unw_getcontext not supported");
  return UNW_EUNSPEC;
}

#endif  // __arm__ && !CXXABI_SJLJ
