set(SOURCES
        Graphics/Shader/Shader.cpp
        Graphics/Shader/Shader.h
        Util/File.h
        Graphics/Shader/Program.cpp
        Graphics/Shader/Program.h
        OddityEngine.cpp
        OddityEngine.h
        Graphics/Graphics.cpp
        Graphics/Graphics.h
        Util/Debug.cpp
        Util/Debug.h
        Graphics/Window.cpp
        Graphics/Window.h
        Graphics/Render/Interface.cpp
        Graphics/Render/Interface.h
        Graphics/Buffer/Buffer.h
        Graphics/Buffer/Bufferobject.h
        Graphics/Render/Scene.cpp
        Graphics/Render/Scene.h
        Graphics/Render/Tracer.cpp
        Graphics/Render/Tracer.h
        Graphics/Camera.cpp
        Graphics/Camera.h
        Util/Value.h
        Graphics/Loader.cpp
        Graphics/Loader.h
        Util/Transform.h
        Util/File.cpp
        Util/Input.cpp
        Util/Input.h
        Physics/Object.cpp
        Physics/Object.h
        Util/Time.cpp
        Util/Time.h
        Graphics/Render/Raysterizer.cpp
        Graphics/Render/Raysterizer.h
        Graphics/Object/Object.cpp
        Graphics/Object/Object.h
        Graphics/Object/Material.cpp
        Graphics/Object/Material.h
        Graphics/Texture/Atlas.cpp
        Graphics/Texture/Atlas.h
        Graphics/Object/Shape.cpp
        Graphics/Object/Shape.h
        Graphics/Render/LayerRayer.cpp
        Graphics/Render/LayerRayer.h
        Graphics/Object/Sphere.cpp
        Graphics/Object/Sphere.h
        Graphics/Render/OddBlock.cpp
        Graphics/Render/OddBlock.h
        "Neural Network/Internal/Layer.h"
#        Util/Matrix.h
        Util/Vector.h
        "Neural Network/Network_old.h"
        Math/basics.h
        Math/random.h
        "Neural Network/Internal/Random_evolve.h"
        Util/SortableVector.h
        Util/Vector_overload.h
        "Neural Network/Network.cpp"
        Graphics/Object/NetworkShape.cpp
        Graphics/Object/NetworkShape.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 23)
