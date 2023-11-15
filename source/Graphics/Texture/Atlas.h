#ifndef TESTSPIEL_ATLAS_H
#define TESTSPIEL_ATLAS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>

namespace OddityEngine {
    namespace Graphics {
        namespace Texture {
            struct Texture {
                std::vector<glm::vec4> data;
                glm::vec<3, GLint> size;
                glm::vec<3, GLint> offset;
            };

            class Atlas {
            protected:
                GLuint texture = 0;
                std::vector<std::vector<glm::vec4>> texture_data_list;
                std::vector<glm::vec<3, GLint>> texture_size_list;
                std::vector<glm::vec<3, GLint>> texture_offset_list;
                glm::vec<3, size_t> max_size = {0, 0, 0};

                void retexture();

            public:
                Atlas();
                ~Atlas();

                GLuint add(const Texture& texture);

                void activate(int to_unit);
            };

        } // OddityEngine
    } // Graphics
} // Texture

#endif //TESTSPIEL_ATLAS_H
