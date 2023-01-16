### MAIN ###
list(APPEND SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp)

### PLAYER ###
list(APPEND SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/Player.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Player.h)

### COMMON ###
list(APPEND SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/common/loadShader.cpp
        ${CMAKE_CURRENT_LIST_DIR}/common/loadShader.h
        ${CMAKE_CURRENT_LIST_DIR}/common/loadTexture.cpp
        ${CMAKE_CURRENT_LIST_DIR}/common/loadTexture.h)

target_sources(${CMAKE_PROJECT_NAME}
        PRIVATE
            ${SOURCE_FILES})
#target_include_directories(${PROJECT_NAME} PUBLIC E:/VulkanSDK/1.3.211.0/Include E:/code/bin/glm/glm E:/code/bin/glfw-3.3.7.bin.WIN64/include E:/code/bin/stb)
#target_link_directories(${PROJECT_NAME} PUBLIC E:/VulkanSDK/1.3.211.0/Lib E:/code/bin/glfw-3.3.7.bin.WIN64/lib-vc2022)
#target_link_libraries(${PROJECT_NAME} PUBLIC vulkan-1.lib glfw3.lib)
