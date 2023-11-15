#include "Object.h"

namespace OddityEngine {
    namespace Graphics {
        Buffer<bufferobject>* Object::active_object_buffer = nullptr;
        Buffer<glm::mat4>* Object::active_transform_buffer = nullptr;

        Buffer<bufferobject> *Object::create_object_buffer() {
            set_object_buffer(new Buffer<bufferobject>());
            return active_object_buffer;
        }

        void Object::set_object_buffer(Buffer<bufferobject> *buffer) {
            active_object_buffer = buffer;
        }

        Buffer<bufferobject> *Object::get_object_buffer() {
            return active_object_buffer;
        }

        Buffer<glm::mat4> *Object::create_transform_buffer() {
            set_transform_buffer(new Buffer<glm::mat4>());
            return active_transform_buffer;
        }

        void Object::set_transform_buffer(Buffer<glm::mat4> *buffer) {
            active_transform_buffer = buffer;
        }

        Buffer<glm::mat4> *Object::get_transform_buffer() {
            return active_transform_buffer;
        }

        void Object::bind_buffer() {
            glBindBufferBase(active_object_buffer->get_type(), OBJECT, *active_object_buffer);
            glBindBufferBase(active_transform_buffer->get_type(), TRANSFORM, *active_transform_buffer);
        }

        Object::Object(const glm::mat4 &transform, Shape *shape, Material *material) : shape(shape), material(material) {
            if (active_object_buffer == nullptr) create_object_buffer();
            if(active_transform_buffer == nullptr) create_transform_buffer();

            this->transform = new Bufferobject(active_transform_buffer, transform);

            object.set({
                    transform,
                    shape != nullptr ? shape->index() : 0,
                    material != nullptr ? material->index() : 0
            });
        }

        Object::~Object() {
            delete(transform);
        }
    } // OddityEngine
} // Graphics