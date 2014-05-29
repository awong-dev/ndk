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

// shared_lock(shared_lock&& u);

#include <shared_mutex>
#include <cassert>

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
    std::shared_lock<std::shared_mutex> lk = std::move(lk0);
=======
    std::shared_lock<std::shared_timed_mutex> lk0(m);
    std::shared_lock<std::shared_timed_mutex> lk = std::move(lk0);
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    assert(lk.mutex() == &m);
    assert(lk.owns_lock() == true);
    assert(lk0.mutex() == nullptr);
    assert(lk0.owns_lock() == false);
#endif  // _LIBCPP_STD_VER > 11
}
