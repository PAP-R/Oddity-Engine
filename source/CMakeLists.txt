set(WINDOW
        Window/Graphics.cpp
        Window/Graphics.h
        Window/Window.cpp
        Window/Window.h
        Window/Event.cpp
        Window/Event.h
)

set(VULKAN
        Vulkan/Vulkan.cpp
        Vulkan/Vulkan.h

)

set(INPUT

)

set(MATH

)

set(PHYSICS

)

set(UTIL
        Util/Debug.cpp
        Util/Debug.h
        Util/Time.cpp
        Util/Time.h
        Util/Trie.h
        Util/Updateable.h
        Util/Updateable.cpp
        Util/Syncable.cpp
        Util/Syncable.h
)

set(SOURCES
        OddityEngine.cpp
        OddityEngine.h
        ${WINDOW}
        ${VULKAN}
        ${INPUT}
        ${MATH}
        ${PHYSICS}
        ${UTIL}
)


add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${LIBS})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23)


add_executable(${PROJECT_NAME}_test main.cpp)

target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME}_test PROPERTIES CXX_STANDARD 23)