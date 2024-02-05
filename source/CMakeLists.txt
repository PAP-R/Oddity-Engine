set(GRAPHICS
        Graphics/Graphics.cpp
        Graphics/Graphics.h
        Graphics/Window.cpp
        Graphics/Window.h
        Graphics/Shader/Shader.cpp
        Graphics/Shader/Shader.h
        Graphics/Shader/Program.cpp
        Graphics/Shader/Program.h
        Graphics/Buffer/Buffer.h
)

set(MATH
        Math/basics.h
        Math/random.h
)

set(UTIL
        Util/Debug.cpp
        Util/Debug.h
        Util/File.cpp
        Util/File.h
        Util/Vector.h
)

set(SOURCES
        ${GRAPHICS}
        ${MATH}
        ${UTIL}
        OddityEngine.cpp
        OddityEngine.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_executable(${PROJECT_NAME}_test ${SOURCES} main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 23)
set_target_properties(${PROJECT_NAME}_test PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 23)