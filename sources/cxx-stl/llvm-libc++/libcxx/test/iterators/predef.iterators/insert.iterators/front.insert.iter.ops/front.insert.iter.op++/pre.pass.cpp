//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <iterator>

// front_insert_iterator

// front_insert_iterator<Cont>& operator++();

#include <iterator>
#include <list>
#include <cassert>
<<<<<<< HEAD
=======
#include "nasty_containers.hpp"
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx

template <class C>
void
test(C c)
{
    std::front_insert_iterator<C> i(c);
    std::front_insert_iterator<C>& r = ++i;
    assert(&r == &i);
}

int main()
{
    test(std::list<int>());
<<<<<<< HEAD
=======
    test(nasty_list<int>());
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
}
