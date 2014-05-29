//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <shared_mutex>

// template <class Mutex> class shared_lock;

// shared_lock(shared_lock const&) = delete;

#include <shared_mutex>

#if _LIBCPP_STD_VER > 11
<<<<<<< HEAD
std::shared_mutex m;
=======
std::shared_timed_mutex m;
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
#endif  // _LIBCPP_STD_VER > 11

int main()
{
#if _LIBCPP_STD_VER > 11
<<<<<<< HEAD
    std::shared_lock<std::shared_mutex> lk0(m);
    std::shared_lock<std::shared_mutex> lk = lk0;
=======
    std::shared_lock<std::shared_timed_mutex> lk0(m);
    std::shared_lock<std::shared_timed_mutex> lk = lk0;
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
#else
#   error
#endif  // _LIBCPP_STD_VER > 11
}
