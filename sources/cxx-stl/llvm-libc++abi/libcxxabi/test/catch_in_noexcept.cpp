//===---------------------- catch_in_noexcept.cpp--------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <exception>
#include <stdlib.h>
#include <assert.h>

struct A {};

void f1() noexcept
{
    try {
        A a;
        throw a;
        assert(false);
    } catch (...) {
        assert(true);
        return;
    }
    assert(false);
}

int main()
{
    f1();
}
