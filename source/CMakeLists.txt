set(SOURCES
        graphics/Graphics.cpp
        graphics/Graphics.h
        graphics/Window.cpp
        graphics/Window.h
        OddityEngine.cpp
        OddityEngine.h
        util/Time.cpp
        util/Time.h
        graphics/render/Tracer.cpp
        graphics/render/Tracer.h
        util/Debug.cpp
        util/Debug.h
        util/Value.cpp
        util/Value.h
        util/Loader.cpp
        util/Loader.h
        graphics/Shader.cpp
        graphics/Shader.h
        graphics/buffer/Bufferobject.h
        graphics/buffer/Buffer.cpp
        graphics/buffer/Buffer.h
        graphics/Camera.cpp
        graphics/Camera.h
        object/Object.cpp
        object/Object.h
        object/Transform.cpp
        object/Transform.h
        object/Line.cpp
        object/Line.h
        graphics/render/SDF_Tracer.cpp
        graphics/render/SDF_Tracer.h
        graphics/render/Renderer.cpp
        graphics/render/Renderer.h
        graphics/render/Raysterizer.cpp
        graphics/render/Raysterizer.h
        graphics/render/Object.cpp
        graphics/render/Object.h
)

add_executable(${PROJECT_NAME}_run main.cpp ${SOURCES})

add_library(${PROJECT_NAME} STATIC ${SOURCES})

set_target_properties(${PROJECT_NAME}_run ${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 23)