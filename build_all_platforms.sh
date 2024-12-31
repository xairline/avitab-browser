#!/bin/sh

rm -rf build CMakeCache.txt CMakeFiles Makefile cmake_install.cmake
mkdir build
cmake . -Bbuild
make -C build
