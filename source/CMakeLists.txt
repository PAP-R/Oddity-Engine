set(SOURCES
        main.cpp
        Graphics/Shader/Shader.cpp
        Graphics/Shader/Shader.h
        Util/file.h
        Graphics/Shader/Program.cpp
        Graphics/Shader/Program.h
)

add_executable(${PROJECT_NAME}_run main.cpp ${SOURCES})

add_library(${PROJECT_NAME} STATIC ${SOURCES})

set_target_properties(${PROJECT_NAME}_run ${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 23)