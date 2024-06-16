#include "World.h"

#include "Util/Time.h"

namespace OddityEngine::Physics {
    World::World() {
        physics_operator.add_operation(OddityEngine::Graphics::Shader::read_shader("physics_operations.glsl"));
        physics_operator.add_buffer(object_buffer, Graphics::OBJECT);
        physics_operator.add_buffer(temp_object_buffer, Graphics::TEMP);
        physics_operator.add_buffer(physics_buffer, Graphics::PHYSICS);
        physics_operator.add_call(0, {1, 0});
        physics_operator.add_call(0, {1, 1});
        physics_operator.add_call(0, {1, 2});
    }

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

        physics_operator.run();


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
    }

    void World::remove_object(Object* object) {
        object->buffer_indices.erase(this);
        objects.erase(std::remove(objects.begin(), objects.end(), object));
    }

    Vector<Object*> World::get_objects() {
        return objects;
    }

    size_t World::object_count() {
        return objects.size();
    }
}
