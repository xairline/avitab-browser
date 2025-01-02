#!/bin/sh

# rm -rf build
# mkdir build

# For macOS
# cmake -DCMAKE_TOOLCHAIN_FILE=toolchain-macos.cmake -Bbuild/macos -H.

# # For Linux
# cmake -DCMAKE_TOOLCHAIN_FILE=toolchain-linux.cmake -Bbuild/linux -H.

# For Windows (using MinGW)
cmake -DCMAKE_TOOLCHAIN_FILE=toolchain-windows.cmake -Bbuild/windows -H.

# echo "Building for macOS"
# make -C build/macos
# file build/macos/mac_x64/avitab_browser.xpl
# echo "Building for Linux"
# make -C build-linux
echo "Building for Windows"
make -C build/windows