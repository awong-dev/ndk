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

// shared_lock(mutex_type& m, defer_lock_t);

#include <shared_mutex>
#include <cassert>

int main()
{
#if _LIBCPP_STD_VER > 11
<<<<<<< HEAD
    std::shared_mutex m;
    std::shared_lock<std::shared_mutex> lk(m, std::defer_lock);
=======
    std::shared_timed_mutex m;
    std::shared_lock<std::shared_timed_mutex> lk(m, std::defer_lock);
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    assert(lk.mutex() == &m);
    assert(lk.owns_lock() == false);
#endif  // _LIBCPP_STD_VER > 11
}
