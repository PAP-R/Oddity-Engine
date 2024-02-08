#include "World.h"

#include "Util/Time.h"

namespace OddityEngine::Physics {
    void World::update() {
        for (auto o : objects) {
            if (!o->update()) {
                remove_object(o);
            }
        }

        glUseProgram(physics_program);

        bind_buffers();

        glDispatchCompute(objects.size(), objects.size(), 1);

        glMemoryBarrier(GL_ALL_BARRIER_BITS);

        auto result = object_buffer.get();

        for (GLsizei i = 0; i < objects.size(); i++) {
            objects[i]->position = result[i].position;
            objects[i]->velocity = result[i].velocity;
            objects[i]->acceleration = result[i].acceleration;
            objects[i]->angle = result[i].angle;
            objects[i]->angle_velocity = result[i].angle_velocity;
            objects[i]->angle_acceleration = result[i].angle_acceleration;
        }
    }

    void World::bind_buffers() {
        for (GLsizei i = 0; i < objects.size(); i++) {
            object_buffer.set(i, objects[i]);
        }

        object_buffer.multiply(object_count(), object_count());

        auto time_delta = Util::Time::delta();

        time_buffer.set(0, &time_delta);

        object_buffer.bind_base(Graphics::PHYSICS);
        time_buffer.bind_base(Graphics::TIME);
    }

    void World::add_object(Object* object) {
        objects.push_back(object);
    }

    void World::remove_object(Object* object) {
        objects.erase(std::remove(objects.begin(), objects.end(), object));
    }

    size_t World::object_count() {
        return objects.size();
    }
}
