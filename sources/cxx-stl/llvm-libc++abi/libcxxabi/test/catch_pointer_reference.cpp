//===---------------------- catch_pointer_referece.cpp --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Test case derived from: https://gcc.gnu.org/ml/gcc-patches/2009-08/msg00264.html
//
//===----------------------------------------------------------------------===//

#include <exception>
#include <stdlib.h>
#include <assert.h>

struct A {};
struct B : A {};

void f1()
{
    B b;
    throw &b;
    assert(false);
}

int main()
{
    try
    {
        f1();
        assert(false);
    }
    catch (A *a)
    {
        assert(true);
    }
    catch (...)
    {
        assert(false);
    }

    try
    {
        f1();
        assert(false);
    }
    catch (B *b)
    {
        assert(true);
    }
    catch (...)
    {
        assert(false);
    }
    try
    {
        f1();
        assert(false);
    }
    catch (A *&a)
    {
        assert(true);
    }
    catch (...)
    {
        assert(false);
    }

    try
    {
        f1();
        assert(false);
    }
    catch (B *&b)
    {
        assert(true);
    }
    catch (...)
    {
        assert(false);
    }
}
