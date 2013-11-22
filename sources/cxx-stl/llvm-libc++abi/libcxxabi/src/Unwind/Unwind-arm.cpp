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
#include "UnwindCursor.hpp"

#if __arm__ && !CXXABI_SJLJ

extern "C" _Unwind_Reason_Code __aeabi_unwind_cpp_pr0(
    _Unwind_State state,
    _Unwind_Control_Block *ucbp,
    _Unwind_Context *context) {
#warning TODO(danakj): Support ARM/NEON here.
  assert(false && "pr0 not supported");
  return _URC_FAILURE;
}

extern "C" _Unwind_Reason_Code __aeabi_unwind_cpp_pr1(
    _Unwind_State state,
    _Unwind_Control_Block *ucbp,
    _Unwind_Context *context) {
#warning TODO(danakj): Support ARM/NEON here.
  assert(false && "pr1 not supported");
  return _URC_FAILURE;
}

extern "C" _Unwind_Reason_Code __aeabi_unwind_cpp_pr2(
    _Unwind_State state,
    _Unwind_Control_Block *ucbp,
    _Unwind_Context *context) {
#warning TODO(danakj): Support ARM/NEON here.
  assert(false && "pr2 not supported");
  return _URC_FAILURE;
}

#endif  // __arm__ && !CXXABI_SJLJ
