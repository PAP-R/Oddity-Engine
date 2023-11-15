cmake_minimum_required(VERSION 3.23)
project(OddityEngine)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

find_package(OpenGL REQUIRED)

include_directories(
        external/glew/include
        external/glfw/include
        external/glm
        external/imgui
        external/fmt/include
        source
)

add_subdirectory(external)

add_subdirectory(source)

set(LIBS
        ${OPENGL_LIBRARY}
        glm
        glfw
        libglew_static
        IMGUI
        fmt::fmt
)

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)

target_link_libraries(${PROJECT_NAME}
        ${LIBS}
)