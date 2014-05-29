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

// bool owns_lock() const noexcept;

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
    std::shared_lock<std::shared_mutex> lk0;
    assert(lk0.owns_lock() == false);
    std::shared_lock<std::shared_mutex> lk1(m);
=======
    std::shared_lock<std::shared_timed_mutex> lk0;
    assert(lk0.owns_lock() == false);
    std::shared_lock<std::shared_timed_mutex> lk1(m);
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    assert(lk1.owns_lock() == true);
    lk1.unlock();
    assert(lk1.owns_lock() == false);
    static_assert(noexcept(lk0.owns_lock()), "owns_lock must be noexcept");
#endif  // _LIBCPP_STD_VER > 11
}
