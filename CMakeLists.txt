cmake_minimum_required(VERSION 3.23)
project(OddityEngine)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)
#find_package(Boost REQUIRED)

include_directories(
        external/glew/include
        external/glfw/include
        external/glm
        external/imgui
        external/fmt/include
        .
)

add_subdirectory(external)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        libglew_static
        IMGUI
        fmt::fmt
        Boost::container
)

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)

add_subdirectory(source)
#add_subdirectory(test)

target_link_libraries(${PROJECT_NAME}_run
        ${ALL_LIBS}
)

target_link_libraries(${PROJECT_NAME}_lib
        ${ALL_LIBS}
        )