#!/bin/bash

set -e

cd ../lib
export OPTIONS=
export AR=arm-none-eabi-ar
export RANLIB=arm-none-eabi-ranlib
export TRIPLE=armv4-none-eabi
export CC=/Users/jroelofs/workdir/mentor/cs-llvm/build/Release+Asserts/bin/clang
export CXX=/Users/jroelofs/workdir/mentor/cs-llvm/build/Release+Asserts/bin/clang++
export SYSROOT=/Users/jroelofs/workdir/mentor/cs-llvm/build/Release+Asserts/SDKs/eabi-elf/codesourcery/arm
rm libc++abi.a || true
rm /Users/jroelofs/workdir/mentor/cs-llvm/build/Release+Asserts/SDKs/eabi-elf/codesourcery/arm/usr/lib/armv4t/libc++abi.a || true
./buildit
cp libc++abi.a /Users/jroelofs/workdir/mentor/cs-llvm/build/Release+Asserts/SDKs/eabi-elf/codesourcery/arm/usr/lib/armv4
cp libc++abi.a /Users/jroelofs/workdir/mentor/cs-llvm/build/Release+Asserts/SDKs/eabi-elf/codesourcery/arm/usr/lib/armv4t

cd ../test
export OPTIONS="-target arm-none-eabi -march=armv4 -T generic-hosted-llvm.ld --sysroot=/Users/jroelofs/workdir/mentor/cs-llvm/build/Release+Asserts/SDKs/eabi-elf/codesourcery/arm -std=c++11 -g -O0 -fno-integrated-as"
./testit
