//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <vector>

// vector(const Alloc& = Alloc());

#include <vector>
#include <cassert>

#include "test_allocator.h"
#include "../../../NotConstructible.h"
#include "../../../stack_allocator.h"
#include "min_allocator.h"
<<<<<<< HEAD
=======
#include "asan_testing.h"
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx

template <class C>
void
test0()
{
    C c;
    assert(c.__invariants());
    assert(c.empty());
    assert(c.get_allocator() == typename C::allocator_type());
<<<<<<< HEAD
=======
    assert(is_contiguous_container_asan_correct(c)); 
#if __cplusplus >= 201103L
    C c1 = {};
    assert(c1.__invariants());
    assert(c1.empty());
    assert(c1.get_allocator() == typename C::allocator_type());
    assert(is_contiguous_container_asan_correct(c1)); 
#endif
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
}

template <class C>
void
test1(const typename C::allocator_type& a)
{
    C c(a);
    assert(c.__invariants());
    assert(c.empty());
    assert(c.get_allocator() == a);
<<<<<<< HEAD
=======
    assert(is_contiguous_container_asan_correct(c)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
}

int main()
{
    {
    test0<std::vector<int> >();
    test0<std::vector<NotConstructible> >();
    test1<std::vector<int, test_allocator<int> > >(test_allocator<int>(3));
    test1<std::vector<NotConstructible, test_allocator<NotConstructible> > >
        (test_allocator<NotConstructible>(5));
    }
    {
        std::vector<int, stack_allocator<int, 10> > v;
        assert(v.empty());
    }
#if __cplusplus >= 201103L
    {
    test0<std::vector<int, min_allocator<int>> >();
    test0<std::vector<NotConstructible, min_allocator<NotConstructible>> >();
    test1<std::vector<int, min_allocator<int> > >(min_allocator<int>{});
    test1<std::vector<NotConstructible, min_allocator<NotConstructible> > >
        (min_allocator<NotConstructible>{});
    }
    {
        std::vector<int, min_allocator<int> > v;
        assert(v.empty());
    }
#endif
}
