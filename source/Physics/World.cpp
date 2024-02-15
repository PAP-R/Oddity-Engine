#include "World.h"

#include "Util/Time.h"

namespace OddityEngine::Physics {
    void World::update() {
        for (auto o : objects) {
            if (!o->update()) {
                remove_object(o);
            }
        }
        time_delta = Util::Time::delta<float>();

        physics_buffer.set(0, this);

        for (GLsizei i = 0; i < objects.size(); i++) {
            object_buffer.set(i, objects[i]);
        }

        temp_object_buffer.resize(object_count() * object_count());

        temp_object_buffer.clean();

        glMemoryBarrier(GL_ALL_BARRIER_BITS);


        glUseProgram(physics_self);

        bind_buffers();

        glDispatchCompute(object_count(), 1, 1);

        glMemoryBarrier(GL_ALL_BARRIER_BITS);


        glUseProgram(physics_other);

        bind_buffers();

        glDispatchCompute(object_count(), object_count(), 1);

        glMemoryBarrier(GL_ALL_BARRIER_BITS);


        glUseProgram(physics_combine);

        bind_buffers();

        glDispatchCompute(object_count(), 1, 1);

        glMemoryBarrier(GL_ALL_BARRIER_BITS);


        auto result = object_buffer.get();

        for (GLsizei i = 0; i < objects.size(); i++) {
            objects[i]->position = result[i].position;
            objects[i]->velocity = result[i].velocity;
            objects[i]->acceleration = result[i].acceleration;

            objects[i]->angle = result[i].angle;
            objects[i]->angle_velocity = result[i].angle_velocity;
            objects[i]->angle_acceleration = result[i].angle_acceleration;


            for (size_t j = 0; j < 10; j++) objects[i]->test_value[j] = result[i].test_value[j];
            objects[i]->state = result[i].state;

            // fmt::print("[{}]\n\t[ {} | {} | {} ]\n\t[ {} | {} | {} ]\n\t[ {} | {} | {} ]\n", i, objects[i]->position.x, objects[i]->position.y, objects[i]->position.z, objects[i]->velocity.x, objects[i]->velocity.y, objects[i]->velocity.z, objects[i]->acceleration.x, objects[i]->acceleration.y, objects[i]->acceleration.z);
            // for (size_t j = 0; j < 10; j++) {
            //     if (j == 0) {
            //         fmt::print("[");
            //     }
            //     else {
            //         fmt::print("|");
            //     }
            //     fmt::print(" {}: {} ", j, objects[i]->test_value[j]);
            // }
            // fmt::print("]\n");
        }
    }

    void World::bind_buffers() {
        object_buffer.bind_base(Graphics::OBJECT);
        temp_object_buffer.bind_base(Graphics::TEMP);
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
