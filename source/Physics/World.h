#ifndef WORLD_H
#define WORLD_H

#include "Object.h"
#include "Graphics/Buffer/Buffer.h"
#include "Graphics/Shader/Program.h"
#include "Graphics/Shader/Shader.h"
#include "Util/Vector.h"

namespace OddityEngine::Physics {
    struct alignas(16) Physics {
        glm::vec4 gravity = {0, -9.81, 0, 1};
        float air_density = 1;
        float ground_height = 0;
        float time_delta;
    };

    class World : public Physics {
    protected:
        Vector<Object*> objects;

        Graphics::Program physics_self = Graphics::Program({Graphics::Shader(GL_COMPUTE_SHADER, "physics_self.comp")});
        Graphics::Program physics_other = Graphics::Program({Graphics::Shader(GL_COMPUTE_SHADER, "physics_other.comp")});
        Graphics::Program physics_combine = Graphics::Program({Graphics::Shader(GL_COMPUTE_SHADER, "physics_combine.comp")});

        Graphics::Buffer<Object_struct> object_buffer = Graphics::Buffer<Object_struct>();
        Graphics::Buffer<Object_struct> temp_object_buffer = Graphics::Buffer<Object_struct>();
        Graphics::Buffer<Physics> physics_buffer = Graphics::Buffer<Physics>();

    public:
        void update();
        void bind_buffers();

        void add_object(Object* object);
        void remove_object(Object* object);

        size_t object_count();
    };
}

#endif //WORLD_H
