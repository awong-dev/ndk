//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <vector>

// vector(size_type n, const value_type& x);

#include <vector>
#include <cassert>

#include "../../../stack_allocator.h"
#include "min_allocator.h"
<<<<<<< HEAD
=======
#include "asan_testing.h"
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx

template <class C>
void
test(typename C::size_type n, const typename C::value_type& x)
{
    C c(n, x);
    assert(c.__invariants());
    assert(c.size() == n);
<<<<<<< HEAD
=======
    assert(is_contiguous_container_asan_correct(c)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    for (typename C::const_iterator i = c.cbegin(), e = c.cend(); i != e; ++i)
        assert(*i == x);
}

int main()
{
    test<std::vector<int> >(50, 3);
    test<std::vector<int, stack_allocator<int, 50> > >(50, 5);
#if __cplusplus >= 201103L
    test<std::vector<int, min_allocator<int>> >(50, 3);
#endif
}
