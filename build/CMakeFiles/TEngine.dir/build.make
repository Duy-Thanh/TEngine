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
include CMakeFiles/TEngine.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/TEngine.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/TEngine.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TEngine.dir/flags.make

CMakeFiles/TEngine.dir/src/core/Application.cpp.obj: CMakeFiles/TEngine.dir/flags.make
CMakeFiles/TEngine.dir/src/core/Application.cpp.obj: CMakeFiles/TEngine.dir/includes_CXX.rsp
CMakeFiles/TEngine.dir/src/core/Application.cpp.obj: D:/OpenGL/src/core/Application.cpp
CMakeFiles/TEngine.dir/src/core/Application.cpp.obj: CMakeFiles/TEngine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\OpenGL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TEngine.dir/src/core/Application.cpp.obj"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TEngine.dir/src/core/Application.cpp.obj -MF CMakeFiles\TEngine.dir\src\core\Application.cpp.obj.d -o CMakeFiles\TEngine.dir\src\core\Application.cpp.obj -c D:\OpenGL\src\core\Application.cpp

CMakeFiles/TEngine.dir/src/core/Application.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TEngine.dir/src/core/Application.cpp.i"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\OpenGL\src\core\Application.cpp > CMakeFiles\TEngine.dir\src\core\Application.cpp.i

CMakeFiles/TEngine.dir/src/core/Application.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TEngine.dir/src/core/Application.cpp.s"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\OpenGL\src\core\Application.cpp -o CMakeFiles\TEngine.dir\src\core\Application.cpp.s

CMakeFiles/TEngine.dir/src/core/Project.cpp.obj: CMakeFiles/TEngine.dir/flags.make
CMakeFiles/TEngine.dir/src/core/Project.cpp.obj: CMakeFiles/TEngine.dir/includes_CXX.rsp
CMakeFiles/TEngine.dir/src/core/Project.cpp.obj: D:/OpenGL/src/core/Project.cpp
CMakeFiles/TEngine.dir/src/core/Project.cpp.obj: CMakeFiles/TEngine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\OpenGL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/TEngine.dir/src/core/Project.cpp.obj"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TEngine.dir/src/core/Project.cpp.obj -MF CMakeFiles\TEngine.dir\src\core\Project.cpp.obj.d -o CMakeFiles\TEngine.dir\src\core\Project.cpp.obj -c D:\OpenGL\src\core\Project.cpp

CMakeFiles/TEngine.dir/src/core/Project.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TEngine.dir/src/core/Project.cpp.i"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\OpenGL\src\core\Project.cpp > CMakeFiles\TEngine.dir\src\core\Project.cpp.i

CMakeFiles/TEngine.dir/src/core/Project.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TEngine.dir/src/core/Project.cpp.s"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\OpenGL\src\core\Project.cpp -o CMakeFiles\TEngine.dir\src\core\Project.cpp.s

CMakeFiles/TEngine.dir/src/core/Window.cpp.obj: CMakeFiles/TEngine.dir/flags.make
CMakeFiles/TEngine.dir/src/core/Window.cpp.obj: CMakeFiles/TEngine.dir/includes_CXX.rsp
CMakeFiles/TEngine.dir/src/core/Window.cpp.obj: D:/OpenGL/src/core/Window.cpp
CMakeFiles/TEngine.dir/src/core/Window.cpp.obj: CMakeFiles/TEngine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\OpenGL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/TEngine.dir/src/core/Window.cpp.obj"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TEngine.dir/src/core/Window.cpp.obj -MF CMakeFiles\TEngine.dir\src\core\Window.cpp.obj.d -o CMakeFiles\TEngine.dir\src\core\Window.cpp.obj -c D:\OpenGL\src\core\Window.cpp

CMakeFiles/TEngine.dir/src/core/Window.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TEngine.dir/src/core/Window.cpp.i"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\OpenGL\src\core\Window.cpp > CMakeFiles\TEngine.dir\src\core\Window.cpp.i

CMakeFiles/TEngine.dir/src/core/Window.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TEngine.dir/src/core/Window.cpp.s"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\OpenGL\src\core\Window.cpp -o CMakeFiles\TEngine.dir\src\core\Window.cpp.s

CMakeFiles/TEngine.dir/src/animation/Export.cpp.obj: CMakeFiles/TEngine.dir/flags.make
CMakeFiles/TEngine.dir/src/animation/Export.cpp.obj: CMakeFiles/TEngine.dir/includes_CXX.rsp
CMakeFiles/TEngine.dir/src/animation/Export.cpp.obj: D:/OpenGL/src/animation/Export.cpp
CMakeFiles/TEngine.dir/src/animation/Export.cpp.obj: CMakeFiles/TEngine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\OpenGL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/TEngine.dir/src/animation/Export.cpp.obj"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TEngine.dir/src/animation/Export.cpp.obj -MF CMakeFiles\TEngine.dir\src\animation\Export.cpp.obj.d -o CMakeFiles\TEngine.dir\src\animation\Export.cpp.obj -c D:\OpenGL\src\animation\Export.cpp

CMakeFiles/TEngine.dir/src/animation/Export.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TEngine.dir/src/animation/Export.cpp.i"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\OpenGL\src\animation\Export.cpp > CMakeFiles\TEngine.dir\src\animation\Export.cpp.i

CMakeFiles/TEngine.dir/src/animation/Export.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TEngine.dir/src/animation/Export.cpp.s"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\OpenGL\src\animation\Export.cpp -o CMakeFiles\TEngine.dir\src\animation\Export.cpp.s

CMakeFiles/TEngine.dir/src/animation/Keyframe.cpp.obj: CMakeFiles/TEngine.dir/flags.make
CMakeFiles/TEngine.dir/src/animation/Keyframe.cpp.obj: CMakeFiles/TEngine.dir/includes_CXX.rsp
CMakeFiles/TEngine.dir/src/animation/Keyframe.cpp.obj: D:/OpenGL/src/animation/Keyframe.cpp
CMakeFiles/TEngine.dir/src/animation/Keyframe.cpp.obj: CMakeFiles/TEngine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\OpenGL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/TEngine.dir/src/animation/Keyframe.cpp.obj"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TEngine.dir/src/animation/Keyframe.cpp.obj -MF CMakeFiles\TEngine.dir\src\animation\Keyframe.cpp.obj.d -o CMakeFiles\TEngine.dir\src\animation\Keyframe.cpp.obj -c D:\OpenGL\src\animation\Keyframe.cpp

CMakeFiles/TEngine.dir/src/animation/Keyframe.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TEngine.dir/src/animation/Keyframe.cpp.i"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\OpenGL\src\animation\Keyframe.cpp > CMakeFiles\TEngine.dir\src\animation\Keyframe.cpp.i

CMakeFiles/TEngine.dir/src/animation/Keyframe.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TEngine.dir/src/animation/Keyframe.cpp.s"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\OpenGL\src\animation\Keyframe.cpp -o CMakeFiles\TEngine.dir\src\animation\Keyframe.cpp.s

CMakeFiles/TEngine.dir/src/animation/Timeline.cpp.obj: CMakeFiles/TEngine.dir/flags.make
CMakeFiles/TEngine.dir/src/animation/Timeline.cpp.obj: CMakeFiles/TEngine.dir/includes_CXX.rsp
CMakeFiles/TEngine.dir/src/animation/Timeline.cpp.obj: D:/OpenGL/src/animation/Timeline.cpp
CMakeFiles/TEngine.dir/src/animation/Timeline.cpp.obj: CMakeFiles/TEngine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\OpenGL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/TEngine.dir/src/animation/Timeline.cpp.obj"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TEngine.dir/src/animation/Timeline.cpp.obj -MF CMakeFiles\TEngine.dir\src\animation\Timeline.cpp.obj.d -o CMakeFiles\TEngine.dir\src\animation\Timeline.cpp.obj -c D:\OpenGL\src\animation\Timeline.cpp

CMakeFiles/TEngine.dir/src/animation/Timeline.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TEngine.dir/src/animation/Timeline.cpp.i"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\OpenGL\src\animation\Timeline.cpp > CMakeFiles\TEngine.dir\src\animation\Timeline.cpp.i

CMakeFiles/TEngine.dir/src/animation/Timeline.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TEngine.dir/src/animation/Timeline.cpp.s"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\OpenGL\src\animation\Timeline.cpp -o CMakeFiles\TEngine.dir\src\animation\Timeline.cpp.s

CMakeFiles/TEngine.dir/src/renderer/Material.cpp.obj: CMakeFiles/TEngine.dir/flags.make
CMakeFiles/TEngine.dir/src/renderer/Material.cpp.obj: CMakeFiles/TEngine.dir/includes_CXX.rsp
CMakeFiles/TEngine.dir/src/renderer/Material.cpp.obj: D:/OpenGL/src/renderer/Material.cpp
CMakeFiles/TEngine.dir/src/renderer/Material.cpp.obj: CMakeFiles/TEngine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\OpenGL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/TEngine.dir/src/renderer/Material.cpp.obj"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TEngine.dir/src/renderer/Material.cpp.obj -MF CMakeFiles\TEngine.dir\src\renderer\Material.cpp.obj.d -o CMakeFiles\TEngine.dir\src\renderer\Material.cpp.obj -c D:\OpenGL\src\renderer\Material.cpp

CMakeFiles/TEngine.dir/src/renderer/Material.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TEngine.dir/src/renderer/Material.cpp.i"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\OpenGL\src\renderer\Material.cpp > CMakeFiles\TEngine.dir\src\renderer\Material.cpp.i

CMakeFiles/TEngine.dir/src/renderer/Material.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TEngine.dir/src/renderer/Material.cpp.s"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\OpenGL\src\renderer\Material.cpp -o CMakeFiles\TEngine.dir\src\renderer\Material.cpp.s

CMakeFiles/TEngine.dir/src/renderer/Shader.cpp.obj: CMakeFiles/TEngine.dir/flags.make
CMakeFiles/TEngine.dir/src/renderer/Shader.cpp.obj: CMakeFiles/TEngine.dir/includes_CXX.rsp
CMakeFiles/TEngine.dir/src/renderer/Shader.cpp.obj: D:/OpenGL/src/renderer/Shader.cpp
CMakeFiles/TEngine.dir/src/renderer/Shader.cpp.obj: CMakeFiles/TEngine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\OpenGL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/TEngine.dir/src/renderer/Shader.cpp.obj"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TEngine.dir/src/renderer/Shader.cpp.obj -MF CMakeFiles\TEngine.dir\src\renderer\Shader.cpp.obj.d -o CMakeFiles\TEngine.dir\src\renderer\Shader.cpp.obj -c D:\OpenGL\src\renderer\Shader.cpp

CMakeFiles/TEngine.dir/src/renderer/Shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TEngine.dir/src/renderer/Shader.cpp.i"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\OpenGL\src\renderer\Shader.cpp > CMakeFiles\TEngine.dir\src\renderer\Shader.cpp.i

CMakeFiles/TEngine.dir/src/renderer/Shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TEngine.dir/src/renderer/Shader.cpp.s"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\OpenGL\src\renderer\Shader.cpp -o CMakeFiles\TEngine.dir\src\renderer\Shader.cpp.s

CMakeFiles/TEngine.dir/src/renderer/Renderer.cpp.obj: CMakeFiles/TEngine.dir/flags.make
CMakeFiles/TEngine.dir/src/renderer/Renderer.cpp.obj: CMakeFiles/TEngine.dir/includes_CXX.rsp
CMakeFiles/TEngine.dir/src/renderer/Renderer.cpp.obj: D:/OpenGL/src/renderer/Renderer.cpp
CMakeFiles/TEngine.dir/src/renderer/Renderer.cpp.obj: CMakeFiles/TEngine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\OpenGL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/TEngine.dir/src/renderer/Renderer.cpp.obj"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TEngine.dir/src/renderer/Renderer.cpp.obj -MF CMakeFiles\TEngine.dir\src\renderer\Renderer.cpp.obj.d -o CMakeFiles\TEngine.dir\src\renderer\Renderer.cpp.obj -c D:\OpenGL\src\renderer\Renderer.cpp

CMakeFiles/TEngine.dir/src/renderer/Renderer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TEngine.dir/src/renderer/Renderer.cpp.i"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\OpenGL\src\renderer\Renderer.cpp > CMakeFiles\TEngine.dir\src\renderer\Renderer.cpp.i

CMakeFiles/TEngine.dir/src/renderer/Renderer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TEngine.dir/src/renderer/Renderer.cpp.s"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\OpenGL\src\renderer\Renderer.cpp -o CMakeFiles\TEngine.dir\src\renderer\Renderer.cpp.s

CMakeFiles/TEngine.dir/src/scene/Scene.cpp.obj: CMakeFiles/TEngine.dir/flags.make
CMakeFiles/TEngine.dir/src/scene/Scene.cpp.obj: CMakeFiles/TEngine.dir/includes_CXX.rsp
CMakeFiles/TEngine.dir/src/scene/Scene.cpp.obj: D:/OpenGL/src/scene/Scene.cpp
CMakeFiles/TEngine.dir/src/scene/Scene.cpp.obj: CMakeFiles/TEngine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\OpenGL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/TEngine.dir/src/scene/Scene.cpp.obj"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TEngine.dir/src/scene/Scene.cpp.obj -MF CMakeFiles\TEngine.dir\src\scene\Scene.cpp.obj.d -o CMakeFiles\TEngine.dir\src\scene\Scene.cpp.obj -c D:\OpenGL\src\scene\Scene.cpp

CMakeFiles/TEngine.dir/src/scene/Scene.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TEngine.dir/src/scene/Scene.cpp.i"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\OpenGL\src\scene\Scene.cpp > CMakeFiles\TEngine.dir\src\scene\Scene.cpp.i

CMakeFiles/TEngine.dir/src/scene/Scene.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TEngine.dir/src/scene/Scene.cpp.s"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\OpenGL\src\scene\Scene.cpp -o CMakeFiles\TEngine.dir\src\scene\Scene.cpp.s

CMakeFiles/TEngine.dir/src/scene/Camera.cpp.obj: CMakeFiles/TEngine.dir/flags.make
CMakeFiles/TEngine.dir/src/scene/Camera.cpp.obj: CMakeFiles/TEngine.dir/includes_CXX.rsp
CMakeFiles/TEngine.dir/src/scene/Camera.cpp.obj: D:/OpenGL/src/scene/Camera.cpp
CMakeFiles/TEngine.dir/src/scene/Camera.cpp.obj: CMakeFiles/TEngine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\OpenGL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/TEngine.dir/src/scene/Camera.cpp.obj"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TEngine.dir/src/scene/Camera.cpp.obj -MF CMakeFiles\TEngine.dir\src\scene\Camera.cpp.obj.d -o CMakeFiles\TEngine.dir\src\scene\Camera.cpp.obj -c D:\OpenGL\src\scene\Camera.cpp

CMakeFiles/TEngine.dir/src/scene/Camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TEngine.dir/src/scene/Camera.cpp.i"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\OpenGL\src\scene\Camera.cpp > CMakeFiles\TEngine.dir\src\scene\Camera.cpp.i

CMakeFiles/TEngine.dir/src/scene/Camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TEngine.dir/src/scene/Camera.cpp.s"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\OpenGL\src\scene\Camera.cpp -o CMakeFiles\TEngine.dir\src\scene\Camera.cpp.s

CMakeFiles/TEngine.dir/src/scene/Model.cpp.obj: CMakeFiles/TEngine.dir/flags.make
CMakeFiles/TEngine.dir/src/scene/Model.cpp.obj: CMakeFiles/TEngine.dir/includes_CXX.rsp
CMakeFiles/TEngine.dir/src/scene/Model.cpp.obj: D:/OpenGL/src/scene/Model.cpp
CMakeFiles/TEngine.dir/src/scene/Model.cpp.obj: CMakeFiles/TEngine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\OpenGL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/TEngine.dir/src/scene/Model.cpp.obj"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TEngine.dir/src/scene/Model.cpp.obj -MF CMakeFiles\TEngine.dir\src\scene\Model.cpp.obj.d -o CMakeFiles\TEngine.dir\src\scene\Model.cpp.obj -c D:\OpenGL\src\scene\Model.cpp

CMakeFiles/TEngine.dir/src/scene/Model.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TEngine.dir/src/scene/Model.cpp.i"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\OpenGL\src\scene\Model.cpp > CMakeFiles\TEngine.dir\src\scene\Model.cpp.i

CMakeFiles/TEngine.dir/src/scene/Model.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TEngine.dir/src/scene/Model.cpp.s"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\OpenGL\src\scene\Model.cpp -o CMakeFiles\TEngine.dir\src\scene\Model.cpp.s

CMakeFiles/TEngine.dir/src/ui/UI.cpp.obj: CMakeFiles/TEngine.dir/flags.make
CMakeFiles/TEngine.dir/src/ui/UI.cpp.obj: CMakeFiles/TEngine.dir/includes_CXX.rsp
CMakeFiles/TEngine.dir/src/ui/UI.cpp.obj: D:/OpenGL/src/ui/UI.cpp
CMakeFiles/TEngine.dir/src/ui/UI.cpp.obj: CMakeFiles/TEngine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\OpenGL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/TEngine.dir/src/ui/UI.cpp.obj"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TEngine.dir/src/ui/UI.cpp.obj -MF CMakeFiles\TEngine.dir\src\ui\UI.cpp.obj.d -o CMakeFiles\TEngine.dir\src\ui\UI.cpp.obj -c D:\OpenGL\src\ui\UI.cpp

CMakeFiles/TEngine.dir/src/ui/UI.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TEngine.dir/src/ui/UI.cpp.i"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\OpenGL\src\ui\UI.cpp > CMakeFiles\TEngine.dir\src\ui\UI.cpp.i

CMakeFiles/TEngine.dir/src/ui/UI.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TEngine.dir/src/ui/UI.cpp.s"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\OpenGL\src\ui\UI.cpp -o CMakeFiles\TEngine.dir\src\ui\UI.cpp.s

CMakeFiles/TEngine.dir/src/ui/TimelineUI.cpp.obj: CMakeFiles/TEngine.dir/flags.make
CMakeFiles/TEngine.dir/src/ui/TimelineUI.cpp.obj: CMakeFiles/TEngine.dir/includes_CXX.rsp
CMakeFiles/TEngine.dir/src/ui/TimelineUI.cpp.obj: D:/OpenGL/src/ui/TimelineUI.cpp
CMakeFiles/TEngine.dir/src/ui/TimelineUI.cpp.obj: CMakeFiles/TEngine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\OpenGL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/TEngine.dir/src/ui/TimelineUI.cpp.obj"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TEngine.dir/src/ui/TimelineUI.cpp.obj -MF CMakeFiles\TEngine.dir\src\ui\TimelineUI.cpp.obj.d -o CMakeFiles\TEngine.dir\src\ui\TimelineUI.cpp.obj -c D:\OpenGL\src\ui\TimelineUI.cpp

CMakeFiles/TEngine.dir/src/ui/TimelineUI.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TEngine.dir/src/ui/TimelineUI.cpp.i"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\OpenGL\src\ui\TimelineUI.cpp > CMakeFiles\TEngine.dir\src\ui\TimelineUI.cpp.i

CMakeFiles/TEngine.dir/src/ui/TimelineUI.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TEngine.dir/src/ui/TimelineUI.cpp.s"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\OpenGL\src\ui\TimelineUI.cpp -o CMakeFiles\TEngine.dir\src\ui\TimelineUI.cpp.s

CMakeFiles/TEngine.dir/src/Main.cpp.obj: CMakeFiles/TEngine.dir/flags.make
CMakeFiles/TEngine.dir/src/Main.cpp.obj: CMakeFiles/TEngine.dir/includes_CXX.rsp
CMakeFiles/TEngine.dir/src/Main.cpp.obj: D:/OpenGL/src/Main.cpp
CMakeFiles/TEngine.dir/src/Main.cpp.obj: CMakeFiles/TEngine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\OpenGL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/TEngine.dir/src/Main.cpp.obj"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TEngine.dir/src/Main.cpp.obj -MF CMakeFiles\TEngine.dir\src\Main.cpp.obj.d -o CMakeFiles\TEngine.dir\src\Main.cpp.obj -c D:\OpenGL\src\Main.cpp

CMakeFiles/TEngine.dir/src/Main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TEngine.dir/src/Main.cpp.i"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\OpenGL\src\Main.cpp > CMakeFiles\TEngine.dir\src\Main.cpp.i

CMakeFiles/TEngine.dir/src/Main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TEngine.dir/src/Main.cpp.s"
	C:\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\OpenGL\src\Main.cpp -o CMakeFiles\TEngine.dir\src\Main.cpp.s

# Object files for target TEngine
TEngine_OBJECTS = \
"CMakeFiles/TEngine.dir/src/core/Application.cpp.obj" \
"CMakeFiles/TEngine.dir/src/core/Project.cpp.obj" \
"CMakeFiles/TEngine.dir/src/core/Window.cpp.obj" \
"CMakeFiles/TEngine.dir/src/animation/Export.cpp.obj" \
"CMakeFiles/TEngine.dir/src/animation/Keyframe.cpp.obj" \
"CMakeFiles/TEngine.dir/src/animation/Timeline.cpp.obj" \
"CMakeFiles/TEngine.dir/src/renderer/Material.cpp.obj" \
"CMakeFiles/TEngine.dir/src/renderer/Shader.cpp.obj" \
"CMakeFiles/TEngine.dir/src/renderer/Renderer.cpp.obj" \
"CMakeFiles/TEngine.dir/src/scene/Scene.cpp.obj" \
"CMakeFiles/TEngine.dir/src/scene/Camera.cpp.obj" \
"CMakeFiles/TEngine.dir/src/scene/Model.cpp.obj" \
"CMakeFiles/TEngine.dir/src/ui/UI.cpp.obj" \
"CMakeFiles/TEngine.dir/src/ui/TimelineUI.cpp.obj" \
"CMakeFiles/TEngine.dir/src/Main.cpp.obj"

# External object files for target TEngine
TEngine_EXTERNAL_OBJECTS =

TEngine.exe: CMakeFiles/TEngine.dir/src/core/Application.cpp.obj
TEngine.exe: CMakeFiles/TEngine.dir/src/core/Project.cpp.obj
TEngine.exe: CMakeFiles/TEngine.dir/src/core/Window.cpp.obj
TEngine.exe: CMakeFiles/TEngine.dir/src/animation/Export.cpp.obj
TEngine.exe: CMakeFiles/TEngine.dir/src/animation/Keyframe.cpp.obj
TEngine.exe: CMakeFiles/TEngine.dir/src/animation/Timeline.cpp.obj
TEngine.exe: CMakeFiles/TEngine.dir/src/renderer/Material.cpp.obj
TEngine.exe: CMakeFiles/TEngine.dir/src/renderer/Shader.cpp.obj
TEngine.exe: CMakeFiles/TEngine.dir/src/renderer/Renderer.cpp.obj
TEngine.exe: CMakeFiles/TEngine.dir/src/scene/Scene.cpp.obj
TEngine.exe: CMakeFiles/TEngine.dir/src/scene/Camera.cpp.obj
TEngine.exe: CMakeFiles/TEngine.dir/src/scene/Model.cpp.obj
TEngine.exe: CMakeFiles/TEngine.dir/src/ui/UI.cpp.obj
TEngine.exe: CMakeFiles/TEngine.dir/src/ui/TimelineUI.cpp.obj
TEngine.exe: CMakeFiles/TEngine.dir/src/Main.cpp.obj
TEngine.exe: CMakeFiles/TEngine.dir/build.make
TEngine.exe: external/glfw-3.3/src/libglfw3.a
TEngine.exe: external/libGLEW_210.a
TEngine.exe: libImGui.a
TEngine.exe: external/assimp-3.0.1270/code/libassimp.a
TEngine.exe: _deps/freetype-build/libfreetyped.a
TEngine.exe: _deps/lunasvg-build/liblunasvg.a
TEngine.exe: external/assimp-3.0.1270/contrib/zlib/libzlib.a
TEngine.exe: CMakeFiles/TEngine.dir/linkLibs.rsp
TEngine.exe: CMakeFiles/TEngine.dir/objects1.rsp
TEngine.exe: CMakeFiles/TEngine.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\OpenGL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Linking CXX executable TEngine.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\TEngine.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TEngine.dir/build: TEngine.exe
.PHONY : CMakeFiles/TEngine.dir/build

CMakeFiles/TEngine.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\TEngine.dir\cmake_clean.cmake
.PHONY : CMakeFiles/TEngine.dir/clean

CMakeFiles/TEngine.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\OpenGL D:\OpenGL D:\OpenGL\build D:\OpenGL\build D:\OpenGL\build\CMakeFiles\TEngine.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/TEngine.dir/depend

