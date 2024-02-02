#ifndef ODDITYENGINE_INTERFACE_H
#define ODDITYENGINE_INTERFACE_H

#include "GL/glew.h"

#include "glm/glm.hpp"

#include "Graphics/Buffer/Buffer.h"
#include "Graphics/Buffer/Bufferobject.h"
#include "Graphics/Loader.h"

namespace OddityEngine {
    namespace Graphics {
        namespace Render {
            struct alignas(16) bufferobject {
                glm::mat4 transform;
                float radius;
                glm::uint32 type;
                glm::uint32 material;
                glm::uint32 vertexstart;
                glm::uint32 vertexcount;
            };

            struct alignas(16) buffermaterial {
                glm::vec4 color;
                glm::vec4 emission;
                float roughness;
            };

            struct alignas(16) buffervertex {
                glm::vec4 pos;
                glm::vec4 color;
                glm::vec4 normal;
                glm::vec2 uv;
            };

            class Interface {
            protected:
                GLuint framebuffer = 0;

                GLsizei layer_count = 1;
                std::vector<GLuint> layers;
                GLuint texture = 0;
                glm::vec2 size = {0, 0};
                glm::vec2 screen_pos = {0, 0};
                glm::vec2 screen_size = {0, 0};

                float layer_ratio = 1;
                float ratio = 1;
                float last_ratio = ratio;

                std::vector<Bufferobject<glm::vec4>> texture_transform;

                void set_texture_transform();

            public:
                explicit Interface(Buffer<glm::vec4>* texture_transform_buffer, size_t layers = 1);
                explicit Interface(size_t layers = 1);
                virtual ~Interface();

                virtual void render() = 0;
                void bind_framebuffer(GLuint layer);

                virtual void set_texture(GLuint texture, const std::vector<GLuint>&layers);
                void set_texture_transform_buffer(Buffer<glm::vec4>* texture_transform_buffer);

                virtual float get_ratio();
                virtual void set_ratio(float ratio);
                virtual void shift_ratio(float ratio);
                virtual void set_size(const glm::vec2 &size);
                virtual void set_screen_size(const glm::vec2 &size);
                virtual void set_screen_pos(const glm::vec2 &pos);

                virtual GLsizei get_layers();
            };

            std::vector<buffervertex> obj_to_vert(Loader::Object object);
        } // OddityEngine
    } // Graphics
} // Render

#endif //ODDITYENGINE_INTERFACE_H
