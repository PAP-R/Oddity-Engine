#ifndef WORLD_H
#define WORLD_H

#include "Object.h"
#include "Graphics/Buffer/Buffer.h"
#include "Graphics/Shader/Program.h"
#include "Graphics/Shader/Shader.h"
#include "Util/Vector.h"

namespace OddityEngine::Physics {
    class World {
    protected:
        Vector<Object*> objects;

        Graphics::Program physics_program = Graphics::Program({Graphics::Shader(GL_COMPUTE_SHADER, "physics.comp")});
        Graphics::Buffer<Object_struct> object_buffer = Graphics::Buffer<Object_struct>();
        Graphics::Buffer<float> time_buffer = Graphics::Buffer<float>();

    public:
        void update();
        void bind_buffers();

        void add_object(Object* object);
        void remove_object(Object* object);

        size_t object_count();
    };
}

#endif //WORLD_H
