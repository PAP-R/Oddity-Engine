set(MAIN_FILE
        main.cpp)

set(COMMON_FILES
        common/loadShader.cpp
        common/loadShader.h)

set(SOURCE_FILES
        ${MAIN_FILE})

add_executable(${PROJECT_NAME}
        ${MAIN_FILE}
        ${COMMON_FILES})

#target_include_directories(${PROJECT_NAME} PUBLIC E:/VulkanSDK/1.3.211.0/Include E:/code/bin/glm/glm E:/code/bin/glfw-3.3.7.bin.WIN64/include E:/code/bin/stb)
#target_link_directories(${PROJECT_NAME} PUBLIC E:/VulkanSDK/1.3.211.0/Lib E:/code/bin/glfw-3.3.7.bin.WIN64/lib-vc2022)
#target_link_libraries(${PROJECT_NAME} PUBLIC vulkan-1.lib glfw3.lib)
