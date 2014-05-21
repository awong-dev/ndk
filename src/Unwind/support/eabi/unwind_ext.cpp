//===--------------------- unwind_ext.cpp ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//
//===----------------------------------------------------------------------===//

#include "config.h"
#include "unwind_ext.h"

#if _LIBUNWIND_BUILD_SJLJ_APIS
static struct _Unwind_FunctionContext *stack_top_ = NULL;

// Accessors to get get/set linked list of frames for sjlj based execeptions.
_LIBUNWIND_HIDDEN
struct _Unwind_FunctionContext *__Unwind_SjLj_GetTopOfFunctionStack() {
  return stack_top_;
}

_LIBUNWIND_HIDDEN
void __Unwind_SjLj_SetTopOfFunctionStack(struct _Unwind_FunctionContext *fc) {
  stack_top_ = fc;
}
#endif

