#include "Tracer.h"

#include "imgui.h"

#include "fmt/core.h"

#include "Util/Time.h"
#include "Util/Debug.h"

namespace OddityEngine {
    namespace Graphics {
        namespace Render {
            void Tracer::init() {
                std::vector<float> screen = {
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
                Debug::add_value([&](){ImGui::SliderFloat("Cull", &cull, 0, 5);});
                Debug::add_value([&](){ImGui::SliderInt("Bounces", &bounces, 0, 10);});
                Debug::add_value([&](){ImGui::SliderInt("Spread", &spread, 0, 5);});
            }

            Tracer::Tracer(Camera *camera) : Interface(), camera(camera) {
                init();
            }

            Tracer::Tracer(Buffer<glm::vec4> *texture_transform_buffer, Camera* camera) : Interface(texture_transform_buffer), camera(camera) {
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

                glUniform1f(program.uniform_location("TIME"), Time::get());

                glm::mat4 screen_perspective = glm::perspective(glm::radians(90.0f), 1.0f, 0.0f, 1.0f);
                glm::mat4 screen_projection = screen_perspective * glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
                glUniformMatrix4fv(program.uniform_location("screen_projection"), 1, GL_FALSE, &screen_projection[0][0]);

                glm::mat4 render_perspective = glm::infinitePerspective(camera->fov, static_cast<float>(screen_size.x) / static_cast<float>(screen_size.y), 0.01f);
                glm::mat4 render_projection = render_perspective * glm::lookAt(glm::vec3(0), camera->front(), camera->up());
                glUniformMatrix4fv(program.uniform_location("render_projection"), 1, GL_FALSE, &render_projection[0][0]);

                glUniform1ui(program.uniform_location("bounces"), bounces);
                glUniform1ui(program.uniform_location("spread"), spread);
                glUniform1f(program.uniform_location("cull"), cull);

                glUniform3f(program.uniform_location("camera_pos"), camera->position.x, camera->position.y, camera->position.z);

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
        } // OddityEngine
    } // Graphics
} // Render