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
        Graphics/Render/Interface.cpp
        Graphics/Render/Interface.h
        Graphics/Render/Scene.cpp
        Graphics/Render/Scene.h
        Graphics/Render/TestRenderer.cpp
        Graphics/Render/TestRenderer.h
        Graphics/Camera.cpp
        Graphics/Camera.h
        Util/Event_Poller.cpp
        Util/Event_Poller.h
        Graphics/Render/ComputeRenderer.cpp
        Graphics/Render/ComputeRenderer.h
        Graphics/Object.cpp
        Graphics/Object.h
        Graphics/Render/Layered.cpp
        Graphics/Render/Layered.h
)

set(MATH
        Math/basics.h
        Math/random.h
        Math/Vector.h
)

set(NEURAL_NETWORK
        NeuralNetwork/Network.cpp
        NeuralNetwork/Network.h
        NeuralNetwork/Internal/Layer.h
        NeuralNetwork/Internal/Random_evolve.h
)

set(PHYSICS
        Physics/Object.cpp
        Physics/Object.h
        Physics/World.cpp
        Physics/World.h
)

set(UTIL
        Util/Debug.cpp
        Util/Debug.h
        Util/File.cpp
        Util/File.h
        Util/Vector.h
        Util/SortableVector.h
        Util/Time.cpp
        Util/Time.h
        Util/Eventable.h
)

set(SOURCES
        ${GRAPHICS}
        ${MATH}
        ${NEURAL_NETWORK}
        ${PHYSICS}
        ${UTIL}
        OddityEngine.cpp
        OddityEngine.h
        Player.cpp
        Player.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_executable(${PROJECT_NAME}_test ${SOURCES} main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 23)
set_target_properties(${PROJECT_NAME}_test PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 23)

add_custom_command(TARGET ${PROJECT_NAME}_test PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/shaders ${OUTPUT_DIR}/shaders
)