# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.30

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\mingw64\bin\cmake.exe

# The command to remove a file.
RM = C:\mingw64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\OpenGL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\OpenGL\build

# Include any dependencies generated for this target.
include external/CMakeFiles/GLEW_210.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include external/CMakeFiles/GLEW_210.dir/compiler_depend.make

# Include the progress variables for this target.
include external/CMakeFiles/GLEW_210.dir/progress.make

# Include the compile flags for this target's objects.
include external/CMakeFiles/GLEW_210.dir/flags.make

external/CMakeFiles/GLEW_210.dir/glew-2.1.0/src/glew.c.obj: external/CMakeFiles/GLEW_210.dir/flags.make
external/CMakeFiles/GLEW_210.dir/glew-2.1.0/src/glew.c.obj: external/CMakeFiles/GLEW_210.dir/includes_C.rsp
external/CMakeFiles/GLEW_210.dir/glew-2.1.0/src/glew.c.obj: D:/OpenGL/external/glew-2.1.0/src/glew.c
external/CMakeFiles/GLEW_210.dir/glew-2.1.0/src/glew.c.obj: external/CMakeFiles/GLEW_210.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\OpenGL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object external/CMakeFiles/GLEW_210.dir/glew-2.1.0/src/glew.c.obj"
	cd /d D:\OpenGL\build\external && C:\mingw64\bin\clang.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT external/CMakeFiles/GLEW_210.dir/glew-2.1.0/src/glew.c.obj -MF CMakeFiles\GLEW_210.dir\glew-2.1.0\src\glew.c.obj.d -o CMakeFiles\GLEW_210.dir\glew-2.1.0\src\glew.c.obj -c D:\OpenGL\external\glew-2.1.0\src\glew.c

external/CMakeFiles/GLEW_210.dir/glew-2.1.0/src/glew.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/GLEW_210.dir/glew-2.1.0/src/glew.c.i"
	cd /d D:\OpenGL\build\external && C:\mingw64\bin\clang.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\OpenGL\external\glew-2.1.0\src\glew.c > CMakeFiles\GLEW_210.dir\glew-2.1.0\src\glew.c.i

external/CMakeFiles/GLEW_210.dir/glew-2.1.0/src/glew.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/GLEW_210.dir/glew-2.1.0/src/glew.c.s"
	cd /d D:\OpenGL\build\external && C:\mingw64\bin\clang.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\OpenGL\external\glew-2.1.0\src\glew.c -o CMakeFiles\GLEW_210.dir\glew-2.1.0\src\glew.c.s

# Object files for target GLEW_210
GLEW_210_OBJECTS = \
"CMakeFiles/GLEW_210.dir/glew-2.1.0/src/glew.c.obj"

# External object files for target GLEW_210
GLEW_210_EXTERNAL_OBJECTS =

external/libGLEW_210.a: external/CMakeFiles/GLEW_210.dir/glew-2.1.0/src/glew.c.obj
external/libGLEW_210.a: external/CMakeFiles/GLEW_210.dir/build.make
external/libGLEW_210.a: external/CMakeFiles/GLEW_210.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\OpenGL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libGLEW_210.a"
	cd /d D:\OpenGL\build\external && $(CMAKE_COMMAND) -P CMakeFiles\GLEW_210.dir\cmake_clean_target.cmake
	cd /d D:\OpenGL\build\external && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\GLEW_210.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
external/CMakeFiles/GLEW_210.dir/build: external/libGLEW_210.a
.PHONY : external/CMakeFiles/GLEW_210.dir/build

external/CMakeFiles/GLEW_210.dir/clean:
	cd /d D:\OpenGL\build\external && $(CMAKE_COMMAND) -P CMakeFiles\GLEW_210.dir\cmake_clean.cmake
.PHONY : external/CMakeFiles/GLEW_210.dir/clean

external/CMakeFiles/GLEW_210.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\OpenGL D:\OpenGL\external D:\OpenGL\build D:\OpenGL\build\external D:\OpenGL\build\external\CMakeFiles\GLEW_210.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : external/CMakeFiles/GLEW_210.dir/depend

