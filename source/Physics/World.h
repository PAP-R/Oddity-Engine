#ifndef WORLD_H
#define WORLD_H

#include "Object.h"
#include "Graphics/Buffer/Buffer.h"
#include "Graphics/Shader/Program.h"
#include "Graphics/Shader/Shader.h"
#include "Util/Vector.h"
#include <Util/Operator.h>

namespace OddityEngine::Physics {
    struct alignas(8) Operation {
        GLuint operation;
        GLuint parameter_start;
    };

    struct alignas(16) Physics {
        float time_delta;
    };

    class World : public Physics {
    protected:
        Vector<Object*> objects;

        Util::Operator physics_operator;


        Graphics::Buffer<float> value_buffer = Graphics::Buffer<float>();
        Graphics::Buffer<Object_struct> object_buffer = Graphics::Buffer<Object_struct>();
        Graphics::Buffer<Object_struct> temp_object_buffer = Graphics::Buffer<Object_struct>();
        Graphics::Buffer<Physics> physics_buffer = Graphics::Buffer<Physics>();
        Graphics::Buffer<float> network_buffer = Graphics::Buffer<float>();

    public:
        void update();
        void bind_buffers();

        void add_object(Object* object);
        void remove_object(Object* object);
        Vector<Object*> get_objects();

        void update_networks();

        size_t object_count();
    };
}

#endif //WORLD_H
