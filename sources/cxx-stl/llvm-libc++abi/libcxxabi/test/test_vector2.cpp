//===--------------------------- test_vector2.cpp -------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "cxxabi.h"

#include <iostream>
#include <cstdlib>

void my_terminate () { exit ( 0 ); }

//  Wrapper routines
void *my_alloc2 ( size_t sz ) {
    void *p = std::malloc ( sz );
//  std::printf ( "Allocated %ld bytes at %lx\n", sz, (unsigned long) p );  
    return p;
    }
    
void my_dealloc2 ( void *p ) {
//  std::printf ( "Freeing %lx\n", (unsigned long) p ); 
    std::free ( p ); 
    }

void my_dealloc3 ( void *p, size_t   sz   ) {
//  std::printf ( "Freeing %lx (size %ld)\n", (unsigned long) p, sz );  
    std::free ( p ); 
    }

__cxxabiv1::__cxa_vec_ctor_return_type my_construct ( void *p ) {
//  std::printf ( "Constructing %lx\n", (unsigned long) p );
    return (__cxxabiv1::__cxa_vec_ctor_return_type)p;
    }

__cxxabiv1::__cxa_vec_ctor_return_type my_destruct  ( void *p ) {
//  std::printf ( "Destructing  %lx\n", (unsigned long) p );
    return (__cxxabiv1::__cxa_vec_ctor_return_type)p;
    }

int gCounter;
__cxxabiv1::__cxa_vec_ctor_return_type count_construct ( void *p ) {
    ++gCounter;
    return (__cxxabiv1::__cxa_vec_ctor_return_type)p;
    }
__cxxabiv1::__cxa_vec_ctor_return_type count_destruct ( void *p ) {
    --gCounter;
    return (__cxxabiv1::__cxa_vec_ctor_return_type)p;
    }


int gConstructorCounter;
int gConstructorThrowTarget;
int gDestructorCounter;
int gDestructorThrowTarget;
__cxxabiv1::__cxa_vec_ctor_return_type throw_construct ( void *p ) {
    if ( gConstructorCounter == gConstructorThrowTarget )
        throw 1;
    ++gConstructorCounter;
    return (__cxxabiv1::__cxa_vec_ctor_return_type)p;
    }
__cxxabiv1::__cxa_vec_ctor_return_type throw_destruct  ( void *p ) {
    if ( ++gDestructorCounter == gDestructorThrowTarget  )
        throw 2;
    return (__cxxabiv1::__cxa_vec_ctor_return_type)p;
    }

struct vec_on_stack {
    void *storage;
    vec_on_stack () : storage ( __cxxabiv1::__cxa_vec_new    (            10, 40, 8, throw_construct, throw_destruct )) {}
    ~vec_on_stack () {          __cxxabiv1::__cxa_vec_delete ( storage,       40, 8,                  throw_destruct );  }
    };


//  Make sure the constructors and destructors are matched
void test_exception_in_destructor ( ) {

//  Try throwing from a destructor while unwinding the stack -- should abort
    gConstructorCounter = gDestructorCounter = 0;
    gConstructorThrowTarget = -1;
    gDestructorThrowTarget  = 5;
    try {
        vec_on_stack v;
        throw 3;
        }
    catch ( int i ) {}

    std::cerr << "should never get here" << std::endl;    
    }



int main ( int argc, char *argv [] ) {
    std::set_terminate ( my_terminate );
    test_exception_in_destructor ();
    return 1;       // we failed if we get here
    }
