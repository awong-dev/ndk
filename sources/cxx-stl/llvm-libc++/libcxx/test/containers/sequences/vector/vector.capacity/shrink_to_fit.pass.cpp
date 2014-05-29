//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <vector>

// void shrink_to_fit();

#include <vector>
#include <cassert>
#include "../../../stack_allocator.h"
#include "min_allocator.h"
<<<<<<< HEAD
=======
#include "asan_testing.h"
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx

int main()
{
    {
        std::vector<int> v(100);
        v.push_back(1);
<<<<<<< HEAD
        v.shrink_to_fit();
        assert(v.capacity() == 101);
        assert(v.size() == 101);
=======
        assert(is_contiguous_container_asan_correct(v)); 
        v.shrink_to_fit();
        assert(v.capacity() == 101);
        assert(v.size() == 101);
        assert(is_contiguous_container_asan_correct(v)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    }
    {
        std::vector<int, stack_allocator<int, 401> > v(100);
        v.push_back(1);
<<<<<<< HEAD
        v.shrink_to_fit();
        assert(v.capacity() == 101);
        assert(v.size() == 101);
=======
        assert(is_contiguous_container_asan_correct(v)); 
        v.shrink_to_fit();
        assert(v.capacity() == 101);
        assert(v.size() == 101);
        assert(is_contiguous_container_asan_correct(v)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    }
#ifndef _LIBCPP_NO_EXCEPTIONS
    {
        std::vector<int, stack_allocator<int, 400> > v(100);
        v.push_back(1);
<<<<<<< HEAD
        v.shrink_to_fit();
        assert(v.capacity() == 200);
        assert(v.size() == 101);
=======
        assert(is_contiguous_container_asan_correct(v)); 
        v.shrink_to_fit();
        assert(v.capacity() == 200);
        assert(v.size() == 101);
        assert(is_contiguous_container_asan_correct(v)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    }
#endif
#if __cplusplus >= 201103L
    {
        std::vector<int, min_allocator<int>> v(100);
        v.push_back(1);
<<<<<<< HEAD
        v.shrink_to_fit();
        assert(v.capacity() == 101);
        assert(v.size() == 101);
=======
        assert(is_contiguous_container_asan_correct(v)); 
        v.shrink_to_fit();
        assert(v.capacity() == 101);
        assert(v.size() == 101);
        assert(is_contiguous_container_asan_correct(v)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    }
#endif
}
