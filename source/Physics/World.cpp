#include "World.h"

#include "Util/Time.h"

namespace OddityEngine::Physics {
    void World::update() {
        for (auto o : objects) {
            if (!o->update(this)) {
                remove_object(o);
            }
        }

        time_delta = Util::Time::delta<float>();

        physics_buffer.set(0, this);

        for (GLsizei i = 0; i < objects.size(); i++) {
            object_buffer.set(i, objects[i]);
        }

        temp_object_buffer.resize(object_count() * object_count());

        glUseProgram(physics_self);

        bind_buffers();

        glDispatchCompute(object_count(), 1, 1);

        glMemoryBarrier(GL_ALL_BARRIER_BITS);
        glFinish();


        glUseProgram(physics_other);

        bind_buffers();

        glDispatchCompute(object_count(), object_count(), 1);

        glMemoryBarrier(GL_ALL_BARRIER_BITS);
        glFinish();


        glUseProgram(physics_combine);

        bind_buffers();

        glDispatchCompute(object_count(), 1, 1);

        glMemoryBarrier(GL_ALL_BARRIER_BITS);
        glFinish();


        auto result = object_buffer.get();

        for (GLsizei i = 0; i < object_count(); i++) {
            *objects[i] = result[i];
        }
    }

    void World::bind_buffers() {
        object_buffer.bind_base(Graphics::OBJECT);
        temp_object_buffer.bind_base(Graphics::TEMP);
        physics_buffer.bind_base(Graphics::PHYSICS);
        network_buffer.bind_base(Graphics::NETWORK);
    }

    void World::add_object(Object* object) {
        object->buffer_indices.insert(std::make_pair(this, objects.size()));
        objects.push_back(object);
        update_networks();
    }

    void World::remove_object(Object* object) {
        object->buffer_indices.erase(this);
        objects.erase(std::remove(objects.begin(), objects.end(), object));
        update_networks();
    }

    Vector<Object*> World::get_objects() {
        return objects;
    }

    void World::update_networks() {
        network_buffer.clear();

        for (auto o : objects) {
            auto net = o->net.front().to_csv();
            o->network_index = *network_buffer.insert_back(net.size(), net.data())[0];
        }
    }

    size_t World::object_count() {
        return objects.size();
    }
}
