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
include external/glfw-3.3/examples/CMakeFiles/splitview.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include external/glfw-3.3/examples/CMakeFiles/splitview.dir/compiler_depend.make

# Include the progress variables for this target.
include external/glfw-3.3/examples/CMakeFiles/splitview.dir/progress.make

# Include the compile flags for this target's objects.
include external/glfw-3.3/examples/CMakeFiles/splitview.dir/flags.make

external/glfw-3.3/examples/CMakeFiles/splitview.dir/splitview.c.obj: external/glfw-3.3/examples/CMakeFiles/splitview.dir/flags.make
external/glfw-3.3/examples/CMakeFiles/splitview.dir/splitview.c.obj: external/glfw-3.3/examples/CMakeFiles/splitview.dir/includes_C.rsp
external/glfw-3.3/examples/CMakeFiles/splitview.dir/splitview.c.obj: D:/OpenGL/external/glfw-3.3/examples/splitview.c
external/glfw-3.3/examples/CMakeFiles/splitview.dir/splitview.c.obj: external/glfw-3.3/examples/CMakeFiles/splitview.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\OpenGL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object external/glfw-3.3/examples/CMakeFiles/splitview.dir/splitview.c.obj"
	cd /d D:\OpenGL\build\external\glfw-3.3\examples && C:\mingw64\bin\clang.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT external/glfw-3.3/examples/CMakeFiles/splitview.dir/splitview.c.obj -MF CMakeFiles\splitview.dir\splitview.c.obj.d -o CMakeFiles\splitview.dir\splitview.c.obj -c D:\OpenGL\external\glfw-3.3\examples\splitview.c

external/glfw-3.3/examples/CMakeFiles/splitview.dir/splitview.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/splitview.dir/splitview.c.i"
	cd /d D:\OpenGL\build\external\glfw-3.3\examples && C:\mingw64\bin\clang.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\OpenGL\external\glfw-3.3\examples\splitview.c > CMakeFiles\splitview.dir\splitview.c.i

external/glfw-3.3/examples/CMakeFiles/splitview.dir/splitview.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/splitview.dir/splitview.c.s"
	cd /d D:\OpenGL\build\external\glfw-3.3\examples && C:\mingw64\bin\clang.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\OpenGL\external\glfw-3.3\examples\splitview.c -o CMakeFiles\splitview.dir\splitview.c.s

external/glfw-3.3/examples/CMakeFiles/splitview.dir/glfw.rc.obj: external/glfw-3.3/examples/CMakeFiles/splitview.dir/flags.make
external/glfw-3.3/examples/CMakeFiles/splitview.dir/glfw.rc.obj: D:/OpenGL/external/glfw-3.3/examples/glfw.rc
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\OpenGL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building RC object external/glfw-3.3/examples/CMakeFiles/splitview.dir/glfw.rc.obj"
	cd /d D:\OpenGL\build\external\glfw-3.3\examples && C:\mingw64\bin\windres.exe -O coff $(RC_DEFINES) $(RC_INCLUDES) $(RC_FLAGS) D:\OpenGL\external\glfw-3.3\examples\glfw.rc CMakeFiles\splitview.dir\glfw.rc.obj

external/glfw-3.3/examples/CMakeFiles/splitview.dir/__/deps/glad_gl.c.obj: external/glfw-3.3/examples/CMakeFiles/splitview.dir/flags.make
external/glfw-3.3/examples/CMakeFiles/splitview.dir/__/deps/glad_gl.c.obj: external/glfw-3.3/examples/CMakeFiles/splitview.dir/includes_C.rsp
external/glfw-3.3/examples/CMakeFiles/splitview.dir/__/deps/glad_gl.c.obj: D:/OpenGL/external/glfw-3.3/deps/glad_gl.c
external/glfw-3.3/examples/CMakeFiles/splitview.dir/__/deps/glad_gl.c.obj: external/glfw-3.3/examples/CMakeFiles/splitview.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\OpenGL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object external/glfw-3.3/examples/CMakeFiles/splitview.dir/__/deps/glad_gl.c.obj"
	cd /d D:\OpenGL\build\external\glfw-3.3\examples && C:\mingw64\bin\clang.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT external/glfw-3.3/examples/CMakeFiles/splitview.dir/__/deps/glad_gl.c.obj -MF CMakeFiles\splitview.dir\__\deps\glad_gl.c.obj.d -o CMakeFiles\splitview.dir\__\deps\glad_gl.c.obj -c D:\OpenGL\external\glfw-3.3\deps\glad_gl.c

external/glfw-3.3/examples/CMakeFiles/splitview.dir/__/deps/glad_gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/splitview.dir/__/deps/glad_gl.c.i"
	cd /d D:\OpenGL\build\external\glfw-3.3\examples && C:\mingw64\bin\clang.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\OpenGL\external\glfw-3.3\deps\glad_gl.c > CMakeFiles\splitview.dir\__\deps\glad_gl.c.i

external/glfw-3.3/examples/CMakeFiles/splitview.dir/__/deps/glad_gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/splitview.dir/__/deps/glad_gl.c.s"
	cd /d D:\OpenGL\build\external\glfw-3.3\examples && C:\mingw64\bin\clang.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\OpenGL\external\glfw-3.3\deps\glad_gl.c -o CMakeFiles\splitview.dir\__\deps\glad_gl.c.s

# Object files for target splitview
splitview_OBJECTS = \
"CMakeFiles/splitview.dir/splitview.c.obj" \
"CMakeFiles/splitview.dir/glfw.rc.obj" \
"CMakeFiles/splitview.dir/__/deps/glad_gl.c.obj"

# External object files for target splitview
splitview_EXTERNAL_OBJECTS =

external/glfw-3.3/examples/splitview.exe: external/glfw-3.3/examples/CMakeFiles/splitview.dir/splitview.c.obj
external/glfw-3.3/examples/splitview.exe: external/glfw-3.3/examples/CMakeFiles/splitview.dir/glfw.rc.obj
external/glfw-3.3/examples/splitview.exe: external/glfw-3.3/examples/CMakeFiles/splitview.dir/__/deps/glad_gl.c.obj
external/glfw-3.3/examples/splitview.exe: external/glfw-3.3/examples/CMakeFiles/splitview.dir/build.make
external/glfw-3.3/examples/splitview.exe: external/glfw-3.3/src/libglfw3.a
external/glfw-3.3/examples/splitview.exe: external/glfw-3.3/examples/CMakeFiles/splitview.dir/linkLibs.rsp
external/glfw-3.3/examples/splitview.exe: external/glfw-3.3/examples/CMakeFiles/splitview.dir/objects1.rsp
external/glfw-3.3/examples/splitview.exe: external/glfw-3.3/examples/CMakeFiles/splitview.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\OpenGL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable splitview.exe"
	cd /d D:\OpenGL\build\external\glfw-3.3\examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\splitview.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
external/glfw-3.3/examples/CMakeFiles/splitview.dir/build: external/glfw-3.3/examples/splitview.exe
.PHONY : external/glfw-3.3/examples/CMakeFiles/splitview.dir/build

external/glfw-3.3/examples/CMakeFiles/splitview.dir/clean:
	cd /d D:\OpenGL\build\external\glfw-3.3\examples && $(CMAKE_COMMAND) -P CMakeFiles\splitview.dir\cmake_clean.cmake
.PHONY : external/glfw-3.3/examples/CMakeFiles/splitview.dir/clean

external/glfw-3.3/examples/CMakeFiles/splitview.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\OpenGL D:\OpenGL\external\glfw-3.3\examples D:\OpenGL\build D:\OpenGL\build\external\glfw-3.3\examples D:\OpenGL\build\external\glfw-3.3\examples\CMakeFiles\splitview.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : external/glfw-3.3/examples/CMakeFiles/splitview.dir/depend
