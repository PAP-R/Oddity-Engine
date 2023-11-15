#include "Atlas.h"

namespace OddityEngine {
    namespace Graphics {
        namespace Texture {
            void Atlas::retexture() {
                if (!texture_data_list.empty()) {
                    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
                    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, max_size.x, max_size.y, max_size.z, 0, GL_RGBA, GL_FLOAT, nullptr);

                    for (int i = 0; i < texture_data_list.size(); i++) {
                        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, texture_offset_list[i].x, texture_offset_list[i].y, texture_offset_list[i].z, texture_size_list[i].x, texture_size_list[i].y, texture_size_list[i].z, GL_RGBA, GL_FLOAT, texture_data_list[i].data());
                    }
                }
            }

            Atlas::Atlas() {
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
            }

            Atlas::~Atlas() {
                glDeleteTextures(1, &texture);
            }

            GLuint Atlas::add(const Texture& texture) {
                if (texture.size.x > max_size.x) {
                    max_size.x = texture.size.x;
                }
                if (texture.size.y > max_size.y) {
                    max_size.y = texture.size.y;
                }

                GLuint index = texture_data_list.size();
                texture_data_list.emplace_back(texture.data);
                texture_size_list.emplace_back(texture.size);

                glm::vec<3, GLint> offset_actual = texture.offset;
                offset_actual.z += max_size.z;
                texture_offset_list.emplace_back(offset_actual);

                max_size.z += texture.size.z + texture.offset.z;

                retexture();

                return index;
            }

            void Atlas::activate(int to_unit) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
                glBindSampler(GL_TEXTURE0, 0);
            }
        } // OddityEngine
    } // Graphics
} // Texture