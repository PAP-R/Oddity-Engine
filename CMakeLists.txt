cmake_minimum_required(VERSION 3.29.3)
project(OddityEngine)

set(OUTPUT_DIR ${PROJECT_SOURCE_DIR}/[output])

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

find_package(OpenGL REQUIRED)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_DEBUG ON)
find_package(Boost 1.84.0 REQUIRED)

message("Found boost components ${Boost_INCLUDE_DIRS}")

find_package(Vulkan REQUIRED)

find_package(SDL2 REQUIRED)
message("found sdl version ${SDL2_VERSION}")

include_directories(
        source
        ${Boost_INCLUDE_DIRS}
        external/glew/include
        external/glm
        external/imgui
        external/fmt/include
        ${SDL2_INCLUDE_DIRS}
        libs/steam/public/steam
        ${Vulkan_INCLUDE_DIRS}
)

add_subdirectory(external)

add_subdirectory(libs/steam)

set(LIBS
        ${OPENGL_LIBRARY}
#        Boost::xpressive
        glm
        libglew_static
        IMGUI
        ${SDL2_LIBRARIES}
        fmt::fmt
        ${STEAM_LIBRARY}
        ${Vulkan_LIBRARIES}
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

target_link_libraries(${PROJECT_NAME}_test_recompile
        ${LIBS}
)

target_link_libraries(${PROJECT_NAME}_test_vulkan
        ${LIBS}
)

file(INSTALL ${RECOURSES} DESTINATION ${OUTPUT_DIR})