set(GRAPHICS_OPENGL
        Graphics/Graphics.cpp
        Graphics/Graphics.h
        Graphics/OpenGL/Window.cpp
        Graphics/OpenGL/Window.h
        Graphics/OpenGL/Shader/Shader.cpp
        Graphics/OpenGL/Shader/Shader.h
        Graphics/OpenGL/Shader/Program.cpp
        Graphics/OpenGL/Shader/Program.h
        Graphics/OpenGL/Buffer/Buffer.h
        Graphics/OpenGL/Render/Interface.cpp
        Graphics/OpenGL/Render/Interface.h
        Graphics/OpenGL/Render/Scene.cpp
        Graphics/OpenGL/Render/Scene.h
        Graphics/OpenGL/Render/TestRenderer.cpp
        Graphics/OpenGL/Render/TestRenderer.h
        Graphics/OpenGL/Camera.cpp
        Graphics/OpenGL/Camera.h
        Graphics/OpenGL/Render/ComputeRenderer.cpp
        Graphics/OpenGL/Render/ComputeRenderer.h
        Graphics/OpenGL/Render/Layered.cpp
        Graphics/OpenGL/Render/Layered.h
        Graphics/OpenGL/Buffer/BufferObject.h
)

set(GRAPHICS_VULKAN
        Graphics/Vulkan/Window.cpp
        Graphics/Vulkan/Window.h
)

set(GRAPHICS
        ${GRAPHICS_OPENGL}
        ${GRAPHICS_VULKAN}
        Graphics/Window.cpp
        Graphics/Window.h
        Graphics/Scene.cpp
        Graphics/Scene.h
)

set(INPUT
        Input/Event_Poller.cpp
        Input/Event_Poller.h
        Util/Eventable.h
        Input/Input.cpp
        Input/Input.h
        Input/Keyboard.cpp
        Input/Keyboard.h
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
        Util/Operator.cpp
        Util/Operator.h
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
        Util/Commander.cpp
        Util/Commander.h
        Util/Trie.h
        Util/CombinedIndex.h
        Util/Pawn.cpp
        Util/Pawn.h
#        Util/Regex.cpp
#        Util/Regex.h
)

set(SOURCES
        ${GRAPHICS}
        ${INPUT}
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
add_executable(${PROJECT_NAME}_test_recompile ${SOURCES} test/recompile.cpp)
add_executable(${PROJECT_NAME}_test_vulkan ${SOURCES} test/vulkan.cpp)

set(TARGETS
        ${PROJECT_NAME}
        ${PROJECT_NAME}_test
        ${PROJECT_NAME}_test_recompile
        ${PROJECT_NAME}_test_vulkan
)

set_target_properties(${TARGETS} PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 23)

add_custom_command(TARGET ${PROJECT_NAME}_test PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/shadersOpenGL ${OUTPUT_DIR}/shaders
)