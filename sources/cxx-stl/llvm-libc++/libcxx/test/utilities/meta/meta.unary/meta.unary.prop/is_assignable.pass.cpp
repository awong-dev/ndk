//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// type_traits

// is_assignable

#include <type_traits>

struct A
{
};

struct B
{
    void operator=(A);
};

template <class T, class U>
void test_is_assignable()
{
    static_assert(( std::is_assignable<T, U>::value), "");
}

template <class T, class U>
void test_is_not_assignable()
{
    static_assert((!std::is_assignable<T, U>::value), "");
}

struct D;

<<<<<<< HEAD
=======
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
struct C
{
    template <class U>
    D operator,(U&&);
};

struct E
{
    C operator=(int);
};
<<<<<<< HEAD
=======
#endif
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx

int main()
{
    test_is_assignable<int&, int&> ();
    test_is_assignable<int&, int> ();
    test_is_assignable<int&, double> ();
    test_is_assignable<B, A> ();
    test_is_assignable<void*&, void*> ();
<<<<<<< HEAD
    test_is_assignable<E, int> ();

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
=======

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    test_is_assignable<E, int> ();

>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    test_is_not_assignable<int, int&> ();
    test_is_not_assignable<int, int> ();
#endif
    test_is_not_assignable<A, B> ();
    test_is_not_assignable<void, const void> ();
    test_is_not_assignable<const void, const void> ();
    test_is_not_assignable<int(), int> ();
}
