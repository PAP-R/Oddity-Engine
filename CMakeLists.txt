cmake_minimum_required(VERSION 3.22)
project(OddityEngine)

set(OUTPUT_DIR ${PROJECT_SOURCE_DIR}/[output])

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

find_package(OpenGL REQUIRED)

find_package(SDL2 REQUIRED)

include_directories(
        source
        external/glew/include
        external/glm
        external/imgui
        external/fmt/include
        ${SDL2_INCLUDE_DIRS}
        libs/steam/public/steam
)

add_subdirectory(external)

add_subdirectory(libs/steam)

set(LIBS
        ${OPENGL_LIBRARY}
        glm
        libglew_static
        IMGUI
        ${SDL2_LIBRARIES}
        fmt::fmt
        ${STEAM_LIBRARY}
)

#add_definitions(
#        -DTW_STATIC
#        -DTW_NO_LIB_PRAGMA
#        -DTW_NO_DIRECT3D
#        -DGLEW_STATIC
#        -D_CRT_SECURE_NO_WARNINGS
#)


set(RECOURSES
        shaders
        models
        networks
)

add_subdirectory(source)


target_link_libraries(${PROJECT_NAME}
        ${LIBS}
)

target_link_libraries(${PROJECT_NAME}_test
        ${LIBS}
)

file(INSTALL ${RECOURSES} DESTINATION ${OUTPUT_DIR})