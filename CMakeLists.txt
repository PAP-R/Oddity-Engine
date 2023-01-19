cmake_minimum_required(VERSION 3.23)
project(OddityEngine)

add_executable(${PROJECT_NAME} source/main.cpp source/base/Object.cpp source/base/Object.h source/base/Cube.h source/base/Cube.cpp source/base/Point.h source/base/Point.cpp)

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 23)

find_package(OpenGL REQUIRED)

add_subdirectory(external)

include_directories(
        external/glew-2.1.0/include
        external/glfw/include
        external/glm
        external/imgui
        .
)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        libglew_static
)

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)

add_subdirectory(source)

target_link_libraries(${PROJECT_NAME}
        ${ALL_LIBS}
)