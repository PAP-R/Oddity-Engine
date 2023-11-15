#ifndef ODDITY_GRAPHICS_OBJECT_H
#define ODDITY_GRAPHICS_OBJECT_H

#include <glm/glm.hpp>

#include <Graphics/Buffer/Buffer.h>
#include <Graphics/Buffer/Bufferobject.h>
#include "Shape.h"
#include "Material.h"

namespace OddityEngine {
    namespace Graphics {
        struct alignas(16) bufferobject {
            glm::mat4 transform;
            GLuint shape_index;
            GLuint material_index;
        };

        class Object {
        protected:
            static Buffer<bufferobject>* active_object_buffer;
            static Buffer<glm::mat4>* active_transform_buffer;

        public:
            static Buffer<bufferobject>* create_object_buffer();
            static void set_object_buffer(Buffer<bufferobject>* buffer);
            static Buffer<bufferobject>* get_object_buffer();

            static Buffer<glm::mat4>* create_transform_buffer();
            static void set_transform_buffer(Buffer<glm::mat4>* buffer);
            static Buffer<glm::mat4>* get_transform_buffer();

            static void bind_buffer();

        protected:
            Bufferobject<glm::mat4>* transform;
            Shape* shape;
            Material* material;

            Bufferobject<bufferobject> object;

        public:
            Object(const glm::mat4& transform = glm::mat4(1), Shape* shape = nullptr, Material* material = nullptr);
            ~Object();
        };
    } // OddityEngine
} // Graphics

#endif //ODDITY_GRAPHICS_OBJECT_H
