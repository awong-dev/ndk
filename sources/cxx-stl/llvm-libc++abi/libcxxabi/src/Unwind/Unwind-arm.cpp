//===--------------------------- Unwind-arm.c ----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//
//  Implements ARM zero-cost C++ exceptions
//
//===----------------------------------------------------------------------===//

#include <unwind.h>

#include <stdbool.h>
#include <stdlib.h>

#include "config.h"
#include "libunwind.h"

#if __arm__ && !CXXABI_SJLJ
namespace {

// Strange order: take words in order, but inside word, take from most to least
// signinficant byte.
uint8_t getByte(uint32_t* data, size_t offset) {
  uint8_t* byteData = reinterpret_cast<uint8_t*>(data);
  return byteData[(offset & ~0x03) + (3 - (offset&0x03))];
}

bool unwindStack(_Unwind_Context* context, uint32_t* data, size_t offset, size_t len) {
  bool wrotePC = false;
  bool finish = false;
  while (offset < len && !finish) {
    uint8_t byte = getByte(data, offset++);
    if ((byte & 0x80) == 0) {
      uint32_t sp = _Unwind_GetGR(context, UNW_ARM_SP);
      if (byte & 0x40)
        sp -= ((byte & 0x3f) << 2) + 4;
      else
        sp += (byte << 2) + 4;
      _Unwind_SetGR(context, UNW_ARM_SP, sp);
    } else {
      switch (byte & 0xf0) {
        case 0x80: {
          if (offset >= len)
            return false;
          uint16_t registers = ((byte & 0x0f) << 12) | (getByte(data, offset++) << 4);
          if (!registers)
            return false;
          if (registers & (1<<15))
            wrotePC = true;
          _Unwind_VRS_Pop(context, _UVRSC_CORE, registers, _UVRSD_UINT32);
          break;
        }
        case 0x90: {
          uint8_t reg = byte & 0x0f;
          if (reg == 13 || reg == 15)
            return false;
          uint32_t sp = _Unwind_GetGR(context, UNW_ARM_R0 + reg);
          _Unwind_SetGR(context, UNW_ARM_SP, sp);
          break;
        }
        case 0xa0: {
          uint8_t numRegisters = 1 + byte & 0x07;
          uint16_t registers = ((1<<numRegisters) - 1) << 4;
          if (byte & 0x08)
            registers |= 1<<14;
          _Unwind_VRS_Pop(context, _UVRSC_CORE, registers, _UVRSD_UINT32);
          break;
        }
        case 0xb0: {
          switch (byte) {
            case 0xb0:
              finish = true;
              break;
            case 0xb1: {
              if (offset >= len)
                return false;
              uint8_t registers = getByte(data, offset++);
              if (registers & 0xf0 || !registers)
                return false;
              _Unwind_VRS_Pop(context, _UVRSC_CORE, registers, _UVRSD_UINT32);
              break;
            }
            case 0xb2: {
              uint32_t addend = 0;
              while (true) {
                if (offset >= len)
                  return false;
                uint8_t v = getByte(data, offset++);
                addend = addend << 7 | (v & 0x7f);
                if ((v & 0x80) == 0)
                  break;
              }
              uint32_t sp = _Unwind_GetGR(context, UNW_ARM_SP);
              sp += 0x204 + addend;
              _Unwind_SetGR(context, UNW_ARM_SP, sp);
              break;
            }
            case 0xb3:
              // TODO(piman): pop VFP single precision from FSTMFDX.
              return false;
            case 0xb4:
            case 0xb5:
            case 0xb6:
            case 0xb7:
              return false;
            default:
              // TODO(piman): pop VFP double precision from FSTMFDX.
              return false;
          }
          break;
        }
        default:
          // TODO(piman): iwMMX, VFP double precision from FSTMFDD, spares.
          return false;
      }
    }
  }
  if (!wrotePC) {
    uint32_t lr = _Unwind_GetGR(context, UNW_ARM_R14);
    _Unwind_SetGR(context, UNW_ARM_R15, lr);
  }
  return true;
}

}

extern "C" _Unwind_Reason_Code __aeabi_unwind_cpp_pr0(
    _Unwind_State state,
    _Unwind_Control_Block *ucbp,
    _Unwind_Context *context) {
  // TODO(piman): handle phase1/phase2.
  return _Unwind_One_Frame(ucbp, context);
}

extern "C" _Unwind_Reason_Code __aeabi_unwind_cpp_pr1(
    _Unwind_State state,
    _Unwind_Control_Block *ucbp,
    _Unwind_Context *context) {
  // TODO(piman): handle phase1/phase2.
  return _Unwind_One_Frame(ucbp, context);
}

extern "C" _Unwind_Reason_Code __aeabi_unwind_cpp_pr2(
    _Unwind_State state,
    _Unwind_Control_Block *ucbp,
    _Unwind_Context *context) {
  // TODO(piman): handle phase1/phase2.
  return _Unwind_One_Frame(ucbp, context);
}

extern "C" _Unwind_Reason_Code _Unwind_One_Frame(
    _Unwind_Control_Block* ucbp,
    struct _Unwind_Context* context) {
  uint32_t* unwindingData = ucbp->pr_cache.ehtp + 1;
  uint32_t unwindInfo = *unwindingData;
  int choice = (unwindInfo & 0x0f000000) >> 24;
  size_t len = 0;
  size_t startOffset = 0;
  switch (choice) {
    case 0:
      len = 4;
      startOffset = 1;
      break;
    case 1:
    case 2:
      len = 4 + 4 *((unwindInfo & 0x00ff0000) >> 16);
      startOffset = 2;
      break;
    default:
      return _URC_FAILURE;
  }
  return unwindStack(context, unwindingData, startOffset, len) ? _URC_CONTINUE_UNWIND : _URC_FAILURE;
}

extern "C" _Unwind_VRS_Result _Unwind_VRS_Pop(
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
  uint32_t* sp = reinterpret_cast<uint32_t*>(_Unwind_GetGR(context, UNW_ARM_R13));
  for (int i = 0; i < 16; ++i) {
    if (!(discriminator & (1<<i)))
      continue;
    uint32_t value = *sp++;
    if (i == 13) {
      reg13Value = value;
      do13 = true;
    } else {
      _Unwind_SetGR(context, UNW_ARM_R0 + i, value);
    }
  }
  if (do13) {
    _Unwind_SetGR(context, UNW_ARM_SP, reg13Value);
  } else {
    _Unwind_SetGR(context, UNW_ARM_SP, reinterpret_cast<uint32_t>(sp));
  }
}

#endif  // __arm__ && !CXXABI_SJLJ
