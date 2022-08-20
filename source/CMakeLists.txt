

set(CHARACTER_FILES
        character/character.cpp
        character/movement.h)

set(CHECK_FILES
        checks/checks.cpp
        checks/checks.h)

set(GRAPHICS_FILES
        graphics/graphics.cpp
        graphics/graphics.h
        graphics/vulkan.cpp
        graphics/vulkan_helper.cpp
        graphics/vulkan_helper.h)

set(INPUT_FILES
        input/input.cpp
        input/input.h)

set(MAIN_FILE
        main.cpp)


set(SOURCE_FILES
        ${MAIN_FILE}
        ${CHARACTER_FILE}
        ${CHECK_FILES}
        ${GRAPHICS_FILES}
        ${INPUT_FILES})

#source_group("\\" FILES ${SOURCE_FILES})


add_executable(${PROJECT_NAME}
        ${MAIN_FILE}
        ${CHARACTER_FILE}
        ${CHECK_FILES}
        ${GRAPHICS_FILES}
        ${INPUT_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC E:/VulkanSDK/1.3.211.0/Include E:/code/bin/glm/glm E:/code/bin/glfw-3.3.7.bin.WIN64/include E:/code/bin/stb)
target_link_directories(${PROJECT_NAME} PUBLIC E:/VulkanSDK/1.3.211.0/Lib E:/code/bin/glfw-3.3.7.bin.WIN64/lib-vc2022)
target_link_libraries(${PROJECT_NAME} PUBLIC vulkan-1.lib glfw3.lib)
