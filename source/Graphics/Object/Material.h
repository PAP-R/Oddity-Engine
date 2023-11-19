#ifndef TESTSPIEL_MATERIAL_H
#define TESTSPIEL_MATERIAL_H

#include "Graphics/Texture/Atlas.h"
#include "Graphics/Buffer/Bufferobject.h"

namespace OddityEngine {
    namespace Graphics {

        struct buffermaterial {
            GLuint albedo;
            GLuint emission;
            GLuint normal_shine;
        };

        class Material {
        protected:
            static Buffer<buffermaterial>* active_material_buffer;
            static Texture::Atlas* texture_atlas;

        public:
            static Buffer<buffermaterial>* create_material_buffer();
            static void set_material_buffer(Buffer<buffermaterial>* buffer);
            static Buffer<buffermaterial>* get_material_buffer();
            static void activate(int to_unit);

            static void bind_buffer();

        protected:
            Bufferobject<buffermaterial> material;

        public:
            Material(glm::vec4 albedo, glm::vec4 emission, glm::vec3 normal, float shine);
            Material(const Texture::Texture& albedo, const Texture::Texture& emission, const Texture::Texture& normal_shine);

            GLuint index();
        };

    } // OddityEngine
} // Graphics

#endif //TESTSPIEL_MATERIAL_H
