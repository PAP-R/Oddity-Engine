set(BINARY ${PROJECT_NAME})

#file(GLOB_RECURSE BASE_SOURCES LIST_DIRECTORIES true base/*.h base/*.cpp)

set(BASE_SOURCES
        base/Buffer.h
        base/Camera.cpp
        base/Camera.h
        base/Control.cpp
        base/Control.h
        base/Loader.cpp
        base/Loader.h
        base/Shader.cpp
        base/Shader.h
        base/Tracer.cpp
        base/Tracer.h
        base/Window.cpp
        base/Window.h
        )

set(SOURCES OddityEngine.cpp OddityEngine.h ${BASE_SOURCES})

add_executable(${BINARY}_run main.cpp ${SOURCES})

add_library(${BINARY} STATIC ${SOURCES})

set_target_properties(${BINARY}_run ${BINARY} PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 23)