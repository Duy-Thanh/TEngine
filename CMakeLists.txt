# CMake entry point
cmake_minimum_required (VERSION 3.12.0)
project (TEngine)

find_package(OpenGL REQUIRED)

if (CMAKE_CROSSCOMPILING)
    message(STATUS "Cross-compiling enabled")
endif()

if (NOT CMAKE_CROSSCOMPILING AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(X11 REQUIRED)
endif()

# Check for MSVC and adjust compiler flags
if (MSVC)
    message(STATUS "Building with MSVC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3 /MP /O2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20 /W3 /MP /O2")
else()
    if (CMAKE_CROSSCOMPILING)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_WIN32_WINNT=0x0601 -DWIN32_WINNT=0x0601 -DWINVER=0x0601 -fstack-protector-strong -fpic -fpie -pie -Wdeprecated-declarations -mavx2 -Ofast --static -fcf-protection=full -mshstk")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32_WINNT=0x0601 -DWIN32_WINNT=0x0601 -DWINVER=0x0601 -fstack-protector-strong -fpic -fpie -pie -std=gnu++23 -mavx2 -Wdeprecated-declarations --static -Ofast -fcf-protection=full -mshstk")
    else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_FORTIFY_SOURCE=2 -D_WIN32_WINNT=0x0601 -DWIN32_WINNT=0x0601 -DWINVER=0x0601 -ftree-vectorize -fopt-info-vec -funroll-loops -fomit-frame-pointer -fstack-protector-strong -fpic -fpie -pie -Wdeprecated-declarations -pthread -mavx2 -Ofast --static -fcf-protection=full -mshstk -march=native")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2 -D_WIN32_WINNT=0x0601 -DWIN32_WINNT=0x0601 -DWINVER=0x0601 -ftree-vectorize -fopt-info-vec -funroll-loops -fomit-frame-pointer -fstack-protector-strong -fpic -fpie -pie -std=gnu++23 -Wdeprecated-declarations -pthread -mavx2 -Ofast --static -fcf-protection=full -mshstk -march=native")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--nxcompat -flto -Wl,--gc-sections -Wl,-Map=memory_map.map")
    endif()
endif()

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

# Compile external dependencies
add_subdirectory (external)

# On Visual Studio, enable multi-process compilation
if (MSVC)
    cmake_policy(SET CMP0026 OLD)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
    include(CreateLaunchers)
    include(MSVCMultipleProcessCompile) # /MP
endif()

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)

include_directories(
    external/AntTweakBar-1.16/include/
    external/glfw-3.3/include/
    external/glm-0.9.9.7/
    external/glew-2.1.0/include/
    external/assimp-3.0.1270/include/
    external/bullet-2.81-rev2613/src/
    ${freetype_SOURCE_DIR}/include
    ${freetype_BINARY_DIR}/include
    ${lunasvg_SOURCE_DIR}/include
    ${lunasvg_BINARY_DIR}/include  
    external/imgui-1.91.0/
    external/imgui-1.91.0/backends
    external/imgui-1.91.0/misc/freetype
    external/stb/include
    external/json/include
    external/crash-handler/
    .
)

# Add Dear ImGui source files
set(IMGUI_SOURCE
    external/imgui-1.91.0/imgui.cpp
    external/imgui-1.91.0/imgui_demo.cpp
    external/imgui-1.91.0/imgui_draw.cpp
    external/imgui-1.91.0/imgui_tables.cpp
    external/imgui-1.91.0/imgui_widgets.cpp
    external/imgui-1.91.0/backends/imgui_impl_glfw.cpp
    external/imgui-1.91.0/backends/imgui_impl_opengl3.cpp
    external/imgui-1.91.0/misc/freetype/imgui_freetype.cpp
)

add_library(ImGui STATIC ${IMGUI_SOURCE})

if (NOT CMAKE_CROSSCOMPILING AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        GLEW_210
        ImGui
        assimp
        freetype
        lunasvg
    )
else()
    set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        GLEW_210
        ImGui
        assimp
        dbghelp
        psapi
        freetype
        lunasvg
    )
endif()

if (NOT CMAKE_CROSSCOMPILING AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND ALL_LIBS ${X11_LIBRARIES} Xxf86vm)
endif()

# Add MSVC-specific libraries
if (MSVC)
    list(APPEND ALL_LIBS opengl32.lib glfw.lib GLEW_210.lib ImGui.lib)
endif()

if (NOT CMAKE_CROSSCOMPILING AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
    )
else()
    add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
    )
endif()

# TEngine build target
add_executable(TEngine
    src/Main.cpp
)

target_compile_definitions(TEngine PUBLIC _WIN32_WINNT=0x0601 WIN32_WINNT=0x0601 WINVER=0x0601)

target_link_libraries(TEngine
    ${ALL_LIBS}
)

# Xcode and Visual working directories
set_target_properties(TEngine PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/")
# create_target_launcher(TEngine WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")
