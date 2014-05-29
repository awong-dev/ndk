//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <iterator>

// reverse_iterator

// reference operator*() const;

// Be sure to respect LWG 198:
//    http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#198
<<<<<<< HEAD
=======
// LWG 198 was superseded by LWG 2360
//    http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#2360
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx

#include <iterator>
#include <cassert>

class A
{
    int data_;
public:
    A() : data_(1) {}
    ~A() {data_ = -1;}

    friend bool operator==(const A& x, const A& y)
        {return x.data_ == y.data_;}
};

template <class It>
<<<<<<< HEAD
class weird_iterator
{
    It it_;
public:
    typedef It                              value_type;
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef std::ptrdiff_t                  difference_type;
    typedef It*                             pointer;
    typedef It&                             reference;

    weird_iterator() {}
    explicit weird_iterator(It it) : it_(it) {}
    ~weird_iterator() {it_ = It();}

    reference operator*() {return it_;}

    weird_iterator& operator--() {return *this;}
};

template <class It>
=======
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
void
test(It i, typename std::iterator_traits<It>::value_type x)
{
    std::reverse_iterator<It> r(i);
    assert(*r == x);
}

int main()
{
<<<<<<< HEAD
    test(weird_iterator<A>(A()), A());
=======
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    A a;
    test(&a+1, A());
}
