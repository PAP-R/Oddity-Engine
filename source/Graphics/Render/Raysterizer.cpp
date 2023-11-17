#include "Raysterizer.h"

#include <glm/gtx/quaternion.hpp>

namespace OddityEngine {
    namespace Graphics {
        namespace Render {
            Raysterizer::Raysterizer(Camera* camera) : camera(camera) {
            }

            Raysterizer::Raysterizer(Buffer<glm::vec4>* texture_transform_buffer, Camera* camera) : Interface(texture_transform_buffer), camera(camera) {
            }

            void Raysterizer::render() {
                if (ratio != last_ratio) {
                    set_size(screen_size * ratio);
                    last_ratio = ratio;
                }

                glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

                glClear(GL_COLOR_BUFFER_BIT);

                glViewport(0, 0, size.x, size.y);

                float aspect = static_cast<float>(screen_size.y) / static_cast<float>(screen_size.x);
                float fov = glm::radians(camera->fov);

                auto projection = glm::mat4(1);
                projection[0][0] = fov / aspect;
                projection[1][1] = fov;

                auto view = glm::toMat4(camera->orientation);
                for (auto p : program_object_list) {
                    p.first->apply();

                    glm::mat4 mvp = projection * view * p.second->get_transform();
                    glUniformMatrix4fv(p.first->uniform_location("mvp"), 1, GL_FALSE, &mvp[0][0]);

                    Object::bind_buffer();
                    Shape::bind_buffer();
                    Material::bind_buffer();
                    Material::activate(0);

                    glEnableVertexAttribArray(0);

                    glBindBuffer(GL_ARRAY_BUFFER, *Shape::get_vertex_buffer());
                    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
                    glDrawArrays(GL_TRIANGLES, p.second->get_shape()->vertex_start(), p.second->get_shape()->vertex_count());

                    glDisableVertexAttribArray(0);
                }
            }

            size_t Raysterizer::add_object(Program* program, Graphics::Object* object) {
                program_object_list.emplace_back(program, object);
                return program_object_list.size();
            }
        } // OddityEngine
    } // Graphics
} // Render