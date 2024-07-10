cmake_minimum_required(VERSION 3.29.3)
project(OddityEngine)

message("Using compiler ${CMAKE_CXX_COMPILER_ID}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/[output])

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


find_package(Vulkan REQUIRED)
message("Found Vulkan version ${Vulkan_VERSION}")

find_package(SDL2 REQUIRED)
message("Found SDL2 version ${SDL2_VERSION}")


include_directories(
        ${Vulkan_INCLUDE_DIRS}
        ${SDL2_INCLUDE_DIRS}
        libs/steam/public/steam
        external/glm
        external/fmt/include
        source
)


add_subdirectory(external)

include(libs/steam/steam.cmake)


set(LIBS
        ${Vulkan_LIBRARIES}
        ${SDL2_LIBRARIES}
        ${STEAM_LIBRARIES}
        glm
        fmt
)


add_subdirectory(source)
