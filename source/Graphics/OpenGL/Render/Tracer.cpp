#include "Tracer.h"

#include <imgui.h>

#include <fmt/core.h>

#include <Util/Debug.h>

namespace OddityEngine {
    namespace Graphics {
        namespace Render {

            void Tracer::init() {std::vector<float> screen = {
                        -1.0f, -1.0f, 1.0f,
                        1.0f, -1.0f, 1.0f,
                        -1.0f, 1.0f, 1.0f,
                        1.0f, 1.0f, 1.0f,
                        1.0f, -1.0f, 1.0f,
                        -1.0f, 1.0f, 1.0f,
                };

                create_buffer_object_list(&screenbuffer, screen);

                Debug::add_value([&](){ImGui::Text("%s", fmt::format("Front: [ {: 05.05f} / {: 05.05f} / {: 05.05f} ]", this->camera->front().x, this->camera->front().y, this->camera->front().z).c_str());});
                Debug::add_value([&](){ImGui::Text("%s", fmt::format("Up: [ {: 05.05f} / {: 05.05f} / {: 05.05f} ]", this->camera->up().x, this->camera->up().y, this->camera->up().z).c_str());});
                Debug::add_value([&](){ImGui::SliderFloat("Ratio", &ratio, 0.1, 1);});
                Debug::add_value([&](){ImGui::SliderFloat4("Quat", &this->camera->orientation[0], -1.1, 1.1);});
                Debug::add_value([&](){ImGui::SliderFloat3("Angle", &this->camera->angle[0], -361, 361);});
                Debug::add_value([&](){ImGui::SliderFloat3("Position", &this->camera->position[0], -20, 20);});
                Debug::add_value([&](){ImGui::SliderFloat("Cull", &cull, 0, 5);});
                Debug::add_value([&](){ImGui::SliderInt("Bounces", &bounces, 0, 10);});
                Debug::add_value([&](){ImGui::SliderInt("Spread", &spread, 0, 5);});
            }

            Tracer::Tracer(Camera *camera) : Interface(), camera(camera) {
                init();
            }

            Tracer::Tracer(Buffer *texture_transform_buffer, Camera* camera) : Interface(texture_transform_buffer), camera(camera) {
                init();
            }

            Tracer::~Tracer() {

            }

            void Tracer::render() {
                if (ratio != last_ratio) {
                    set_screen_size(screen_size);
                    last_ratio = ratio;
                }

                glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

                glClear(GL_COLOR_BUFFER_BIT);

                glUseProgram(program);

                glViewport(0, 0, size.x, size.y);

                glm::mat4 screen_perspective = glm::perspective(glm::radians(90.0f), 1.0f, 0.9f, 1.1f);
                glm::mat4 screen_projection = screen_perspective * glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
                glUniformMatrix4fv(program.uniform_location("screen_projection"), 1, GL_FALSE, &screen_projection[0][0]);

                glm::mat4 render_perspective = glm::perspective(camera->fov, static_cast<float>(screen_size.y) / static_cast<float>(screen_size.x), 0.1f, 100.0f);
                glm::mat4 render_projection = render_perspective * glm::lookAt(glm::vec3(0), camera->front(), camera->up());
                glUniformMatrix4fv(program.uniform_location("render_projection"), 1, GL_FALSE, &render_projection[0][0]);

                glUniform1ui(program.uniform_location("bounces"), bounces);
                glUniform1ui(program.uniform_location("spread"), spread);
                glUniform1f(program.uniform_location("cull"), cull);

                glUniform3f(program.uniform_location("camera_pos"), camera->position.x, camera->position.y, camera->position.z);

                if (objectbuffer != nullptr)
                    glBindBufferBase(objectbuffer->get_type(), 3, *objectbuffer);
                if (materialbuffer != nullptr)
                    glBindBufferBase(materialbuffer->get_type(), 4, *materialbuffer);
                if (vertexbuffer != nullptr)
                    glBindBufferBase(vertexbuffer->get_type(), 5, *vertexbuffer);

                glEnableVertexAttribArray(0);

                glBindBuffer(screenbuffer.get_type(), screenbuffer);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
                glDrawArrays(GL_TRIANGLES, 0, 6);

                glDisableVertexAttribArray(0);
            }

            void Tracer::set_screen_size(const glm::vec2 &size) {
                Interface::set_size(size * ratio);
                Interface::set_screen_size(size);
            }

            std::vector<buffervertex> Tracer::obj_to_vert(Loader::Object object) {
                std::vector<buffervertex> vertices;
                for (auto f : object.faces) {
                    vertices.emplace_back(glm::vec4(object.vertices[f[0] - 1], 1), glm::vec4(object.colors[f[0] - 1], 1), glm::vec4(object.normals[f[2] - 1], 1), object.uvs[f[1] - 1]);
                }

                return vertices;
            }
        } // OddityEngine
    } // Graphics
} // Render