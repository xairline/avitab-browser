#!/bin/sh

# rm -rf build
# mkdir build
cmake -DCMAKE_TOOLCHAIN_FILE=toolchain-macos.cmake -Bbuild/macos -H.
# cmake -DCMAKE_TOOLCHAIN_FILE=toolchain-linux.cmake -Bbuild/linux -H.
cmake -DCMAKE_TOOLCHAIN_FILE=toolchain-windows.cmake -Bbuild/windows -H.

echo "Building for macOS..."
make -C build/macos
file build/macos/mac_x64/avitab-browser.xpl

echo "Building for Linux..."
# make -C build/linux

echo "Building for Windows..."
make -C build/windows
file build/windows/win_x64/avitab-browser.xpl

echo "Done building all platforms."