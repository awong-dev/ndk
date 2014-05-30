//===---------------------- catch_pointer_referece.cpp --------------------===//
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
#include <stdio.h>

struct Base {};
struct Derived : Base {};

template <typename HandlerType, typename ThrowType>
void assert_catches()
{
    try
    {
        Derived d;
        throw static_cast<ThrowType>(&d);
        printf("%s\n", __PRETTY_FUNCTION__);
        assert(false && "Statements after throw must be unreachable");
    }
    catch (HandlerType h)
    {
        assert(true);
        return;
    }
    catch (...)
    {
        printf("%s\n", __PRETTY_FUNCTION__);
        assert(false && "Should not have entered catch-all");
    }

    printf("%s\n", __PRETTY_FUNCTION__);
    assert(false && "The catch should have returned");
}

template <typename HandlerType, typename ThrowType>
void assert_cannot_catch()
{
    try
    {
        Derived d;
        throw static_cast<ThrowType>(&d);
        printf("%s\n", __PRETTY_FUNCTION__);
        assert(false && "Statements after throw must be unreachable");
    }
    catch (HandlerType h)
    {
        printf("%s\n", __PRETTY_FUNCTION__);
        assert(false && "Should not have entered the catch");
    }
    catch (...)
    {
        assert(true);
        return;
    }

    printf("%s\n", __PRETTY_FUNCTION__);
    assert(false && "The catch-all should have returned");
}

void f1()
{
    // Test that every combination of handler of type:
    //   cv1 Base * cv2
    // catches an exception of type:
    //   Derived *
    assert_catches<               Base *               , Derived *>();
    assert_catches<const          Base *               , Derived *>();
    assert_catches<      volatile Base *               , Derived *>();
    assert_catches<const volatile Base *               , Derived *>();
    assert_catches<               Base * const         , Derived *>();
    assert_catches<               Base * const volatile, Derived *>();
    assert_catches<const          Base * const         , Derived *>();
    assert_catches<const          Base * const volatile, Derived *>();
    assert_catches<const volatile Base * const         , Derived *>();
    assert_catches<const volatile Base * const volatile, Derived *>();
}

void f2()
{
    // Test that every combination of handler of type:
    //   cv1 Base * cv2
    // catches an exception of type:
    //   Base *
    assert_catches<               Base *               , Base *>();
    assert_catches<const          Base *               , Base *>();
    assert_catches<      volatile Base *               , Base *>();
    assert_catches<const volatile Base *               , Base *>();
    assert_catches<               Base * const         , Base *>();
    assert_catches<               Base * const volatile, Base *>();
    assert_catches<const          Base * const         , Base *>();
    assert_catches<const          Base * const volatile, Base *>();
    assert_catches<const volatile Base * const         , Base *>();
    assert_catches<const volatile Base * const volatile, Base *>();
}

void f3()
{
    // Test that every combination of handler of type:
    //   cv1 Derived * cv2
    // catches an exception of type:
    //   Derived *
    assert_catches<               Derived *               , Derived *>();
    assert_catches<const          Derived *               , Derived *>();
    assert_catches<      volatile Derived *               , Derived *>();
    assert_catches<const volatile Derived *               , Derived *>();
    assert_catches<               Derived * const         , Derived *>();
    assert_catches<               Derived * const volatile, Derived *>();
    assert_catches<const          Derived * const         , Derived *>();
    assert_catches<const          Derived * const volatile, Derived *>();
    assert_catches<const volatile Derived * const         , Derived *>();
    assert_catches<const volatile Derived * const volatile, Derived *>();
}

void f4()
{
    // Test that every combination of handler of type:
    //   cv1 Derived * cv2
    // cannot catch an exception of type:
    //   Base *
    assert_cannot_catch<               Derived *               , Base *>();
    assert_cannot_catch<const          Derived *               , Base *>();
    assert_cannot_catch<      volatile Derived *               , Base *>();
    assert_cannot_catch<const volatile Derived *               , Base *>();
    assert_cannot_catch<               Derived * const         , Base *>();
    assert_cannot_catch<               Derived * const volatile, Base *>();
    assert_cannot_catch<const          Derived * const         , Base *>();
    assert_cannot_catch<const          Derived * const volatile, Base *>();
    assert_cannot_catch<const volatile Derived * const         , Base *>();
    assert_cannot_catch<const volatile Derived * const volatile, Base *>();
}

void f5()
{
    // Note that it is a C++ ABI defect that these do catch
    //
    // Test that every combination of handler of type:
    //   cv1 Base * cv2 &
    // catches an exception of type:
    //   Derived *
    assert_catches<               Base *                &, Derived *>();
    assert_catches<const          Base *                &, Derived *>();
    assert_catches<      volatile Base *                &, Derived *>();
    assert_catches<const volatile Base *                &, Derived *>();
    assert_catches<               Base * const          &, Derived *>();
    assert_catches<               Base * const volatile &, Derived *>();
    assert_catches<const          Base * const          &, Derived *>();
    assert_catches<const          Base * const volatile &, Derived *>();
    assert_catches<const volatile Base * const          &, Derived *>();
    assert_catches<const volatile Base * const volatile &, Derived *>();
}

void f6()
{
    // Test that every combination of handler of type:
    //   cv1 Base * cv2 &
    // catches an exception of type:
    //   Base *
    assert_catches<               Base *                &, Base *>();
    assert_catches<const          Base *                &, Base *>();
    assert_catches<      volatile Base *                &, Base *>();
    assert_catches<const volatile Base *                &, Base *>();
    assert_catches<               Base * const          &, Base *>();
    assert_catches<               Base * const volatile &, Base *>();
    assert_catches<const          Base * const          &, Base *>();
    assert_catches<const          Base * const volatile &, Base *>();
    assert_catches<const volatile Base * const          &, Base *>();
    assert_catches<const volatile Base * const volatile &, Base *>();
}

void f7()
{
    // Test that every combination of handler of type:
    //   cv1 Derived * cv2 &
    // cannot catch an exception of type:
    //   Base *
    assert_cannot_catch<               Derived *                &, Base *>();
    assert_cannot_catch<const          Derived *                &, Base *>();
    assert_cannot_catch<      volatile Derived *                &, Base *>();
    assert_cannot_catch<const volatile Derived *                &, Base *>();
    assert_cannot_catch<               Derived * const          &, Base *>();
    assert_cannot_catch<               Derived * const volatile &, Base *>();
    assert_cannot_catch<const          Derived * const          &, Base *>();
    assert_cannot_catch<const          Derived * const volatile &, Base *>();
    assert_cannot_catch<const volatile Derived * const          &, Base *>();
    assert_cannot_catch<const volatile Derived * const volatile &, Base *>();
}

void f8()
{
    // Test case derived from the discussion here:
    //   https://gcc.gnu.org/ml/gcc-patches/2009-08/msg00264.html
    // Specifically:
    //   "This [test exposes a] corner case of the ARM C++ ABI. The generic C++
    //    ABI also gets this wrong, because I failed to notice the subtlety here.
    //     The issue is that 15.3/3 3rd bullet says:
    //        "the handler is of type cv1 T* cv2 and E is a pointer type that
    //         can be converted to the type of the handler by either or both of:
    //         a standard pointer conversion (4.10) not involving conversions to
    //         pointers to private or protected or ambiguous classes"
    //    Notice that the handlers of type "cv1 T*cv2&" are not allowed such
    //    freedom to find a base class. The ABI error is that we treat handlers
    //    of reference type exactly the same as the corresponding hander of
    //    non-reference type. Elsewhere in the exception handling this makes no
    //    difference (for instance bullet 1 explicitly says 'cv T or cv T&')."
    //
    // See also: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#388
    //
    //  TL;DR: it is a C++ ABI defect that these do catch in the genetic C++ ABI
    //         but ARM EABI gets it right, and there, these should not catch.

#if !defined(__ARM_EABI__)
    // Test that every combination of handler of type:
    //   cv1 Derived * cv2 &
    // catches an exception of type:
    //   Derived *
    assert_catches<               Derived *                &, Derived *>();
    assert_catches<const          Derived *                &, Derived *>();
    assert_catches<      volatile Derived *                &, Derived *>();
    assert_catches<const volatile Derived *                &, Derived *>();
    assert_catches<               Derived * const          &, Derived *>();
    assert_catches<               Derived * const volatile &, Derived *>();
    assert_catches<const          Derived * const          &, Derived *>();
    assert_catches<const          Derived * const volatile &, Derived *>();
    assert_catches<const volatile Derived * const          &, Derived *>();
    assert_catches<const volatile Derived * const volatile &, Derived *>();
#else // defined(__ARM_EABI__)
    // Test that every combination of handler of type:
    //   cv1 Derived * cv2 &
    // cannot catch an exception of type:
    //   Derived *
    assert_cannot_catch<               Derived *                &, Derived *>();
    assert_cannot_catch<const          Derived *                &, Derived *>();
    assert_cannot_catch<      volatile Derived *                &, Derived *>();
    assert_cannot_catch<const volatile Derived *                &, Derived *>();
    assert_cannot_catch<               Derived * const          &, Derived *>();
    assert_cannot_catch<               Derived * const volatile &, Derived *>();
    assert_cannot_catch<const          Derived * const          &, Derived *>();
    assert_cannot_catch<const          Derived * const volatile &, Derived *>();
    assert_cannot_catch<const volatile Derived * const          &, Derived *>();
    assert_cannot_catch<const volatile Derived * const volatile &, Derived *>();
#endif // defined(__ARM_EABI__)
}

int main()
{
    f1();
    f2();
    f3();
    f4();
    f5();
    f6();
    f7();
    f8();
}
