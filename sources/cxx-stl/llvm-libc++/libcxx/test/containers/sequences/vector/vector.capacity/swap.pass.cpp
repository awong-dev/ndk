//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <vector>

// void swap(vector& x);

#include <vector>
#include <cassert>

#include "min_allocator.h"
<<<<<<< HEAD
=======
#include "asan_testing.h"
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx

int main()
{
    {
        std::vector<int> v1(100);
        std::vector<int> v2(200);
<<<<<<< HEAD
        v1.swap(v2);
        assert(v1.size() == 200);
        assert(v1.capacity() == 200);
        assert(v2.size() == 100);
        assert(v2.capacity() == 100);
=======
        assert(is_contiguous_container_asan_correct(v1)); 
        assert(is_contiguous_container_asan_correct(v2)); 
        v1.swap(v2);
        assert(v1.size() == 200);
        assert(v1.capacity() == 200);
        assert(is_contiguous_container_asan_correct(v1)); 
        assert(v2.size() == 100);
        assert(v2.capacity() == 100);
        assert(is_contiguous_container_asan_correct(v2)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    }
#if __cplusplus >= 201103L
    {
        std::vector<int, min_allocator<int>> v1(100);
        std::vector<int, min_allocator<int>> v2(200);
<<<<<<< HEAD
        v1.swap(v2);
        assert(v1.size() == 200);
        assert(v1.capacity() == 200);
        assert(v2.size() == 100);
        assert(v2.capacity() == 100);
=======
        assert(is_contiguous_container_asan_correct(v1)); 
        assert(is_contiguous_container_asan_correct(v2)); 
        v1.swap(v2);
        assert(v1.size() == 200);
        assert(v1.capacity() == 200);
        assert(is_contiguous_container_asan_correct(v1)); 
        assert(v2.size() == 100);
        assert(v2.capacity() == 100);
        assert(is_contiguous_container_asan_correct(v2)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    }
#endif
}
