#ifndef ODDITYENGINE_INTERFACE_H
#define ODDITYENGINE_INTERFACE_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"

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
                GLuint framebuffer;

                glm::vec2 size = {0, 0};
                glm::vec2 screen_pos = {0, 0};
                glm::vec2 screen_size = {0, 0};

                Bufferobject<glm::vec4> texture_transform;

                void set_texture_transform();

            public:
                explicit Interface(Buffer<glm::vec4>* texture_transform_buffer);
                explicit Interface();
                ~Interface();

                virtual void render() = 0;
                void set_texture(GLuint texture, GLuint layer);
                void set_texture_transform_buffer(Buffer<glm::vec4>* texture_transform_buffer);

                virtual void set_size(const glm::vec2 &size);
                virtual void set_screen_size(const glm::vec2 &size);
                virtual void set_screen_pos(const glm::vec2 &pos);
            };

            std::vector<buffervertex> obj_to_vert(Loader::Object object);
        } // OddityEngine
    } // Graphics
} // Render

#endif //ODDITYENGINE_INTERFACE_H
