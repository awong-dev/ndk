//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// test <stdbool.h>

#include <stdbool.h>

#ifndef __bool_true_false_are_defined
#error __bool_true_false_are_defined not defined
#endif

<<<<<<< HEAD
#if !defined(__clang__)
// GCC defines bool, true, and false in lib/gcc/arm-linux-androideabi/4.8/include/stdbool.h
#else

=======
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
#ifdef bool
#error bool should not be defined
#endif

#ifdef true
#error true should not be defined
#endif

#ifdef false
#error false should not be defined
#endif

<<<<<<< HEAD
#endif

=======
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
int main()
{
}
