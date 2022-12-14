cmake_minimum_required(VERSION 3.23)
project(OddityEngine)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)

add_subdirectory(external)

include_directories(
        external/glew-2.1.0/include
        external/glfw-3.3.8/include
        external/glm
        .
)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        GLEW_1130
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