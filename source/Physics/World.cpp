#include "World.h"

#include "Util/Time.h"

namespace OddityEngine::Physics {
    float index_x = 1;
    float index_y = 1;
    float index_z = 0;

    void World::update() {
        for (auto o : objects) {
            if (!o->update()) {
                remove_object(o);
            }
        }

        for (GLsizei i = 0; i < objects.size(); i++) {
            // fmt::print("[{}]\n\t[ {} | {} | {} ]\n\t[ {} | {} | {} ]\n\t[ {} | {} | {} ]\n\t[ {} | {} | {} ]\n", i, objects[i]->position.x, objects[i]->position.y, objects[i]->position.z, objects[i]->velocity.x, objects[i]->velocity.y, objects[i]->velocity.z, objects[i]->acceleration.x, objects[i]->acceleration.y, objects[i]->acceleration.z, objects[i]->state, objects[i]->test_value.z, objects[i]->test_value.w);
            object_buffer.set(i, objects[i]);
        }

        auto time_delta = Util::Time::delta<float>();

        physics_buffer.set(0, &time_delta);

        glUseProgram(physics_program);

        physics_buffer.set(1, &index_x);
        physics_buffer.set(2, &index_y);
        physics_buffer.set(3, &index_z);

        bind_buffers();

        glDispatchCompute(object_count(), object_count(), 1);

        glFinish();

        auto result = object_buffer.get();

        for (GLsizei i = 0; i < objects.size(); i++) {
            objects[i]->position = result[i].position;
            objects[i]->velocity = result[i].velocity;
            objects[i]->acceleration = result[i].acceleration;

            objects[i]->angle = result[i].angle;
            objects[i]->angle_velocity = result[i].angle_velocity;
            objects[i]->angle_acceleration = result[i].angle_acceleration;

            objects[i]->test_value = result[i].test_value;
            objects[i]->state = result[i].state;
        }

        if (++index_x >= objects.size()) {
            index_x = 1;
            if (++index_y >= objects.size()) {
                index_y = 1;
            }
        }
    }

    void World::bind_buffers() {
        object_buffer.bind_base(Graphics::OBJECT);
        physics_buffer.bind_base(Graphics::PHYSICS);
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
