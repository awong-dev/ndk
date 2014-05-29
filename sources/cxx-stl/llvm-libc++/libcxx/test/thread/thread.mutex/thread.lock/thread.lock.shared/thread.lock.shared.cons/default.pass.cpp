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

// shared_lock();

#include <shared_mutex>
#include <cassert>

int main()
{
#if _LIBCPP_STD_VER > 11
<<<<<<< HEAD
    std::shared_lock<std::shared_mutex> ul;
=======
    std::shared_lock<std::shared_timed_mutex> ul;
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    assert(!ul.owns_lock());
    assert(ul.mutex() == nullptr);
#endif  // _LIBCPP_STD_VER > 11
}
