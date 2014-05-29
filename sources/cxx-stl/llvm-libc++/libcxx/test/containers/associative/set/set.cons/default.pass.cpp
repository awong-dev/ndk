//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <set>

// class set

// set();

#include <set>
#include <cassert>

#include "min_allocator.h"

int main()
{
    {
    std::set<int> m;
    assert(m.empty());
    assert(m.begin() == m.end());
    }
#if __cplusplus >= 201103L
    {
    std::set<int, std::less<int>, min_allocator<int>> m;
    assert(m.empty());
    assert(m.begin() == m.end());
    }
<<<<<<< HEAD
=======
    {
    std::set<int> m = {};
    assert(m.empty());
    assert(m.begin() == m.end());
    }
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
#endif
}
