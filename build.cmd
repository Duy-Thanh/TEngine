set CURRENT_DIRECTORY=%~dp0

mkdir %CURRENT_DIRECTORY%win_build

cd %CURRENT_DIRECTORY%win_build
@REM cmake -G "Ninja" -DCMAKE_C_COMPILER="%CURRENT_DIRECTORY%clang+llvm-19.1.3-x86_64-pc-windows-msvc\bin\clang.exe" -DCMAKE_CXX_COMPILER="%CURRENT_DIRECTORY%clang+llvm-19.1.3-x86_64-pc-windows-msvc\bin\clang++.exe" ..
cmake -G "Ninja" -DCMAKE_C_COMPILER="C:\mingw64\bin\gcc.exe" -DCMAKE_CXX_COMPILER="C:\mingw64\bin\g++.exe" ..
ninja

cd %CURRENT_DIRECTORY%
pause
exit
