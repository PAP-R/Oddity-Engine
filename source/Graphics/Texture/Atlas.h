#ifndef TESTSPIEL_ATLAS_H
#define TESTSPIEL_ATLAS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>

#include <Graphics/Buffer/Buffer.h>

namespace OddityEngine {
    namespace Graphics {
        namespace Texture {
            struct Texture {
                std::vector<glm::vec4> data;
                glm::vec<3, GLsizei> size;
                glm::vec<3, GLsizei> offset;
            };

            class Atlas {
            protected:
                GLuint texture = 0;

                std::vector<std::vector<glm::vec4>> texture_data_list;
                std::vector<glm::vec<3, GLsizei>> texture_size_list;
                std::vector<glm::vec<3, GLsizei>> texture_offset_list;
                glm::vec<3, GLsizei> max_size = {0, 0, 0};
                glm::vec<3, GLsizei> last_max_size = {0, 0, 0};

                Buffer<glm::vec4> texture_transform_buffer = Buffer<glm::vec4>(GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW);

                void retexture();

            public:
                Atlas();
                ~Atlas();

                GLuint add(const Texture& texture);
                GLuint get_texture();

                void activate(int to_unit);
            };

        } // OddityEngine
    } // Graphics
} // Texture

#endif //TESTSPIEL_ATLAS_H
