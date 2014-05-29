//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <vector>

// template <class T, class Alloc>
//   void swap(vector<T,Alloc>& x, vector<T,Alloc>& y);

#include <vector>
#include <cassert>
#include "test_allocator.h"
#include "min_allocator.h"
<<<<<<< HEAD
=======
#include "asan_testing.h"
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx

int main()
{
    {
        int a1[] = {1, 3, 7, 9, 10};
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        std::vector<int> c1(a1, a1+sizeof(a1)/sizeof(a1[0]));
        std::vector<int> c2(a2, a2+sizeof(a2)/sizeof(a2[0]));
<<<<<<< HEAD
        swap(c1, c2);
        assert(c1 == std::vector<int>(a2, a2+sizeof(a2)/sizeof(a2[0])));
        assert(c2 == std::vector<int>(a1, a1+sizeof(a1)/sizeof(a1[0])));
=======
        assert(is_contiguous_container_asan_correct(c1)); 
        assert(is_contiguous_container_asan_correct(c2)); 
        swap(c1, c2);
        assert(c1 == std::vector<int>(a2, a2+sizeof(a2)/sizeof(a2[0])));
        assert(c2 == std::vector<int>(a1, a1+sizeof(a1)/sizeof(a1[0])));
        assert(is_contiguous_container_asan_correct(c1)); 
        assert(is_contiguous_container_asan_correct(c2)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    }
    {
        int a1[] = {1, 3, 7, 9, 10};
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        std::vector<int> c1(a1, a1);
        std::vector<int> c2(a2, a2+sizeof(a2)/sizeof(a2[0]));
<<<<<<< HEAD
=======
        assert(is_contiguous_container_asan_correct(c1)); 
        assert(is_contiguous_container_asan_correct(c2)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
        swap(c1, c2);
        assert(c1 == std::vector<int>(a2, a2+sizeof(a2)/sizeof(a2[0])));
        assert(c2.empty());
        assert(distance(c2.begin(), c2.end()) == 0);
<<<<<<< HEAD
=======
        assert(is_contiguous_container_asan_correct(c1)); 
        assert(is_contiguous_container_asan_correct(c2)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    }
    {
        int a1[] = {1, 3, 7, 9, 10};
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        std::vector<int> c1(a1, a1+sizeof(a1)/sizeof(a1[0]));
        std::vector<int> c2(a2, a2);
<<<<<<< HEAD
=======
        assert(is_contiguous_container_asan_correct(c1)); 
        assert(is_contiguous_container_asan_correct(c2)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
        swap(c1, c2);
        assert(c1.empty());
        assert(distance(c1.begin(), c1.end()) == 0);
        assert(c2 == std::vector<int>(a1, a1+sizeof(a1)/sizeof(a1[0])));
<<<<<<< HEAD
=======
        assert(is_contiguous_container_asan_correct(c1)); 
        assert(is_contiguous_container_asan_correct(c2)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    }
    {
        int a1[] = {1, 3, 7, 9, 10};
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        std::vector<int> c1(a1, a1);
        std::vector<int> c2(a2, a2);
<<<<<<< HEAD
=======
        assert(is_contiguous_container_asan_correct(c1)); 
        assert(is_contiguous_container_asan_correct(c2)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
        swap(c1, c2);
        assert(c1.empty());
        assert(distance(c1.begin(), c1.end()) == 0);
        assert(c2.empty());
        assert(distance(c2.begin(), c2.end()) == 0);
<<<<<<< HEAD
=======
        assert(is_contiguous_container_asan_correct(c1)); 
        assert(is_contiguous_container_asan_correct(c2)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    }
#ifndef _LIBCPP_DEBUG_LEVEL
// This test known to result in undefined behavior detected by _LIBCPP_DEBUG_LEVEL >= 1
    {
        int a1[] = {1, 3, 7, 9, 10};
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        typedef test_allocator<int> A;
        std::vector<int, A> c1(a1, a1+sizeof(a1)/sizeof(a1[0]), A(1));
        std::vector<int, A> c2(a2, a2+sizeof(a2)/sizeof(a2[0]), A(2));
        swap(c1, c2);
        assert((c1 == std::vector<int, A>(a2, a2+sizeof(a2)/sizeof(a2[0]))));
        assert(c1.get_allocator() == A(1));
        assert((c2 == std::vector<int, A>(a1, a1+sizeof(a1)/sizeof(a1[0]))));
        assert(c2.get_allocator() == A(2));
    }
#endif
    {
        int a1[] = {1, 3, 7, 9, 10};
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        typedef other_allocator<int> A;
        std::vector<int, A> c1(a1, a1+sizeof(a1)/sizeof(a1[0]), A(1));
        std::vector<int, A> c2(a2, a2+sizeof(a2)/sizeof(a2[0]), A(2));
<<<<<<< HEAD
=======
        assert(is_contiguous_container_asan_correct(c1)); 
        assert(is_contiguous_container_asan_correct(c2)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
        swap(c1, c2);
        assert((c1 == std::vector<int, A>(a2, a2+sizeof(a2)/sizeof(a2[0]))));
        assert(c1.get_allocator() == A(2));
        assert((c2 == std::vector<int, A>(a1, a1+sizeof(a1)/sizeof(a1[0]))));
        assert(c2.get_allocator() == A(1));
<<<<<<< HEAD
=======
        assert(is_contiguous_container_asan_correct(c1)); 
        assert(is_contiguous_container_asan_correct(c2)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    }
#if __cplusplus >= 201103L
    {
        int a1[] = {1, 3, 7, 9, 10};
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        std::vector<int, min_allocator<int>> c1(a1, a1+sizeof(a1)/sizeof(a1[0]));
        std::vector<int, min_allocator<int>> c2(a2, a2+sizeof(a2)/sizeof(a2[0]));
<<<<<<< HEAD
        swap(c1, c2);
        assert((c1 == std::vector<int, min_allocator<int>>(a2, a2+sizeof(a2)/sizeof(a2[0]))));
        assert((c2 == std::vector<int, min_allocator<int>>(a1, a1+sizeof(a1)/sizeof(a1[0]))));
=======
        assert(is_contiguous_container_asan_correct(c1)); 
        assert(is_contiguous_container_asan_correct(c2)); 
        swap(c1, c2);
        assert((c1 == std::vector<int, min_allocator<int>>(a2, a2+sizeof(a2)/sizeof(a2[0]))));
        assert((c2 == std::vector<int, min_allocator<int>>(a1, a1+sizeof(a1)/sizeof(a1[0]))));
        assert(is_contiguous_container_asan_correct(c1)); 
        assert(is_contiguous_container_asan_correct(c2)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    }
    {
        int a1[] = {1, 3, 7, 9, 10};
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        std::vector<int, min_allocator<int>> c1(a1, a1);
        std::vector<int, min_allocator<int>> c2(a2, a2+sizeof(a2)/sizeof(a2[0]));
<<<<<<< HEAD
=======
        assert(is_contiguous_container_asan_correct(c1)); 
        assert(is_contiguous_container_asan_correct(c2)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
        swap(c1, c2);
        assert((c1 == std::vector<int, min_allocator<int>>(a2, a2+sizeof(a2)/sizeof(a2[0]))));
        assert(c2.empty());
        assert(distance(c2.begin(), c2.end()) == 0);
<<<<<<< HEAD
=======
        assert(is_contiguous_container_asan_correct(c1)); 
        assert(is_contiguous_container_asan_correct(c2)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    }
    {
        int a1[] = {1, 3, 7, 9, 10};
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        std::vector<int, min_allocator<int>> c1(a1, a1+sizeof(a1)/sizeof(a1[0]));
        std::vector<int, min_allocator<int>> c2(a2, a2);
<<<<<<< HEAD
=======
        assert(is_contiguous_container_asan_correct(c1)); 
        assert(is_contiguous_container_asan_correct(c2)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
        swap(c1, c2);
        assert(c1.empty());
        assert(distance(c1.begin(), c1.end()) == 0);
        assert((c2 == std::vector<int, min_allocator<int>>(a1, a1+sizeof(a1)/sizeof(a1[0]))));
<<<<<<< HEAD
=======
        assert(is_contiguous_container_asan_correct(c1)); 
        assert(is_contiguous_container_asan_correct(c2)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    }
    {
        int a1[] = {1, 3, 7, 9, 10};
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        std::vector<int, min_allocator<int>> c1(a1, a1);
        std::vector<int, min_allocator<int>> c2(a2, a2);
<<<<<<< HEAD
=======
        assert(is_contiguous_container_asan_correct(c1)); 
        assert(is_contiguous_container_asan_correct(c2)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
        swap(c1, c2);
        assert(c1.empty());
        assert(distance(c1.begin(), c1.end()) == 0);
        assert(c2.empty());
        assert(distance(c2.begin(), c2.end()) == 0);
<<<<<<< HEAD
=======
        assert(is_contiguous_container_asan_correct(c1)); 
        assert(is_contiguous_container_asan_correct(c2)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    }
#ifndef _LIBCPP_DEBUG_LEVEL
// This test known to result in undefined behavior detected by _LIBCPP_DEBUG_LEVEL >= 1
    {
        int a1[] = {1, 3, 7, 9, 10};
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        typedef min_allocator<int> A;
        std::vector<int, A> c1(a1, a1+sizeof(a1)/sizeof(a1[0]), A());
        std::vector<int, A> c2(a2, a2+sizeof(a2)/sizeof(a2[0]), A());
<<<<<<< HEAD
=======
        assert(is_contiguous_container_asan_correct(c1)); 
        assert(is_contiguous_container_asan_correct(c2)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
        swap(c1, c2);
        assert((c1 == std::vector<int, A>(a2, a2+sizeof(a2)/sizeof(a2[0]))));
        assert(c1.get_allocator() == A());
        assert((c2 == std::vector<int, A>(a1, a1+sizeof(a1)/sizeof(a1[0]))));
        assert(c2.get_allocator() == A());
<<<<<<< HEAD
=======
        assert(is_contiguous_container_asan_correct(c1)); 
        assert(is_contiguous_container_asan_correct(c2)); 
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    }
#endif
#endif
}
