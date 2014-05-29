//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <deque>

// void push_back(const value_type& x);

#include <deque>
#include <cassert>

// Flag that makes the copy constructor for CMyClass throw an exception
static bool gCopyConstructorShouldThow = false;


class CMyClass {
<<<<<<< HEAD
    public: CMyClass();
    public: CMyClass(const CMyClass& iOther);
    public: ~CMyClass();

    private: int fMagicValue;

=======
    public: CMyClass(int tag);
    public: CMyClass(const CMyClass& iOther);
    public: ~CMyClass();

    bool equal(const CMyClass &rhs) const
        { return fTag == rhs.fTag && fMagicValue == rhs.fMagicValue; }
    private:
        int fMagicValue;
        int fTag;
        
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    private: static int kStartedConstructionMagicValue;
    private: static int kFinishedConstructionMagicValue;
};

// Value for fMagicValue when the constructor has started running, but not yet finished
int CMyClass::kStartedConstructionMagicValue = 0;
// Value for fMagicValue when the constructor has finished running
int CMyClass::kFinishedConstructionMagicValue = 12345;

<<<<<<< HEAD
CMyClass::CMyClass() :
    fMagicValue(kStartedConstructionMagicValue)
=======
CMyClass::CMyClass(int tag) :
    fMagicValue(kStartedConstructionMagicValue), fTag(tag)
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
{
    // Signal that the constructor has finished running
    fMagicValue = kFinishedConstructionMagicValue;
}

<<<<<<< HEAD
CMyClass::CMyClass(const CMyClass& /*iOther*/) :
    fMagicValue(kStartedConstructionMagicValue)
=======
CMyClass::CMyClass(const CMyClass& iOther) :
    fMagicValue(kStartedConstructionMagicValue), fTag(iOther.fTag)
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
{
    // If requested, throw an exception _before_ setting fMagicValue to kFinishedConstructionMagicValue
    if (gCopyConstructorShouldThow) {
        throw std::exception();
    }
    // Signal that the constructor has finished running
    fMagicValue = kFinishedConstructionMagicValue;
}

CMyClass::~CMyClass() {
    // Only instances for which the constructor has finished running should be destructed
    assert(fMagicValue == kFinishedConstructionMagicValue);
}

<<<<<<< HEAD
int main()
{
    CMyClass instance;
    std::deque<CMyClass> vec;

    vec.push_back(instance);
=======
bool operator==(const CMyClass &lhs, const CMyClass &rhs) { return lhs.equal(rhs); }

int main()
{
    CMyClass instance(42);
    std::deque<CMyClass> vec;

    vec.push_back(instance);
    std::deque<CMyClass> vec2(vec);
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx

    gCopyConstructorShouldThow = true;
    try {
        vec.push_back(instance);
    }
    catch (...) {
<<<<<<< HEAD
=======
        assert(vec==vec2);
>>>>>>> 1aeedfd... Pulled ToT libc++ to sources/cxx-stl/llvm-libc++/libcxx
    }
}
