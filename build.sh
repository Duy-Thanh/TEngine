#! /bin/bash

FILE_PATH="src/BuildDate.h"

DATE_TIME=$(date +"%Y-%m-%d %H:%M:%S")
DATE=$(date +"%Y%m%d")

echo "#define BUILD_DATE_TIME \"$DATE_TIME\"" > "$FILE_PATH"
echo "#define BUILD_REVISION \"$DATE\"" >> "$FILE_PATH"

rm -rf build

# Build for Linux
mkdir linux_build
cd linux_build
rm -rf imgui.ini
cmake ..
make all -j$(nproc)
cd ..
cp src/Shader/*.* linux_build

# Build for Windows
mkdir win_build
cd win_build
cmake -DCMAKE_TOOLCHAIN_FILE=../external/glfw-3.3/CMake/x86_64-w64-mingw32.cmake ..
make all -j$(nproc)
cp /usr/lib/gcc/x86_64-w64-mingw32/13-posix/libgcc_s_seh-1.dll $PWD
cp /usr/lib/gcc/x86_64-w64-mingw32/13-posix/libstdc++-6.dll $PWD
#cp /usr/lib/gcc/x86_64-w64-mingw32/13-posix/libwinpthread-1.dll bin

cd ../
cp src/Shader/*.* win_build
rm -rf imgui.ini
