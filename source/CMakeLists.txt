set(SOURCES
        graphics/Graphics.cpp
        graphics/Graphics.h
        graphics/Window.cpp
        graphics/Window.h
        OddityEngine.cpp
        OddityEngine.h
        util/Time.cpp
        util/Time.h
        graphics/Tracer.cpp
        graphics/Tracer.h
        util/Debug.cpp
        util/Debug.h
        util/Value.cpp
        util/Value.h
        graphics/Shader.cpp
        graphics/Shader.h
        graphics/buffer/Bufferobject.cpp
        graphics/buffer/Bufferobject.h
        graphics/buffer/Buffer.cpp
        graphics/buffer/Buffer.h
        graphics/Camera.cpp
        graphics/Camera.h
)

add_executable(${PROJECT_NAME}_run main.cpp ${SOURCES})

add_library(${PROJECT_NAME} STATIC ${SOURCES})

set_target_properties(${PROJECT_NAME}_run ${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 23)