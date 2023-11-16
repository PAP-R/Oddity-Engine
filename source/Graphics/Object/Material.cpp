#include "Material.h"

namespace OddityEngine {
    namespace Graphics {
        Texture::Atlas Material::texture_atlas = Texture::Atlas();
        Buffer<buffermaterial>* Material::active_material_buffer = nullptr;

        Buffer<buffermaterial> *Material::create_material_buffer() {
            set_material_buffer(new Buffer<buffermaterial>());
            return active_material_buffer;
        }

        void Material::set_material_buffer(Buffer<buffermaterial> *buffer) {
            active_material_buffer = buffer;
        }

        Buffer<buffermaterial> *Material::get_material_buffer() {
            return active_material_buffer;
        }

        Material::Material(glm::vec4 albedo, glm::vec4 emission, glm::vec3 normal, float shine) {
            if (active_material_buffer == nullptr) create_material_buffer();

            material.set_buffer(active_material_buffer);

            material.set({
                texture_atlas.add({{albedo}, {1, 1, 1}, {0, 0, 0}}),
                texture_atlas.add({{emission}, {1, 1, 1}, {0, 0, 0}}),
                texture_atlas.add({{glm::vec4(normal, shine)}, {1, 1, 1}, {0, 0, 0}})
            });
        }

        Material::Material(const Texture::Texture& albedo, const Texture::Texture& emission, const Texture::Texture& normal_shine) : material(active_material_buffer) {
            if (active_material_buffer == nullptr) create_material_buffer();

            material.set({
                texture_atlas.add(albedo),
                texture_atlas.add(emission),
                texture_atlas.add(normal_shine)}
                );
        }

        void Material::activate(int to_unit) {
            texture_atlas.activate(to_unit);
        }

        GLuint Material::index() {
            return material.get_index();
        }

        void Material::bind_buffer() {
            glBindBufferBase(active_material_buffer->get_type(), MATERIAL, *active_material_buffer);
        }
    } // OddityEngine
} // Graphics