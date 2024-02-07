#include "World.h"

#include "Util/Time.h"

namespace OddityEngine::Physics {
    void World::update() {
        for (GLsizei i = 0; i < objects.size(); i++) {
            if (objects[i]->state & TO_DELETE) {
                remove_object(objects[i]);
            }
            else {
                object_buffer.set(i, objects[i]);
            }
        }

        auto time_delta = Util::Time::delta();

        time_buffer.set(0, &time_delta);

        glUseProgram(physics_program);

        glBindBufferBase(object_buffer.get_type(), Graphics::PHYSICS, object_buffer);
        glBindBufferBase(time_buffer.get_type(), Graphics::TIME, time_buffer);

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

    void World::add_object(Object* object) {
        objects.push_back(object);
    }

    void World::remove_object(Object* object) {
        objects.erase(std::remove(objects.begin(), objects.end(), object));
    }
}
