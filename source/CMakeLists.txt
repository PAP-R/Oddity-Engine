set(SOURCES
        main.cpp
        Graphics/OpenGL/Shader/Shader.cpp
        Graphics/OpenGL/Shader/Shader.h
        Util/file.h
        Graphics/OpenGL/Shader/Program.cpp
        Graphics/OpenGL/Shader/Program.h
        OddityEngine.cpp
        OddityEngine.h
        Graphics/OpenGL/Graphics.cpp
        Graphics/OpenGL/Graphics.h
        Util/Debug.cpp
        Util/Debug.h
        Graphics/OpenGL/Window.cpp
        Graphics/OpenGL/Window.h
        Graphics/OpenGL/Render/Interface.cpp
        Graphics/OpenGL/Render/Interface.h
        Graphics/OpenGL/Buffer/Buffer.cpp
        Graphics/OpenGL/Buffer/Buffer.h
        Graphics/OpenGL/Buffer/Bufferobject.h
)

add_executable(${PROJECT_NAME}_run main.cpp ${SOURCES})

add_library(${PROJECT_NAME} STATIC ${SOURCES})

set_target_properties(${PROJECT_NAME}_run ${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 23)