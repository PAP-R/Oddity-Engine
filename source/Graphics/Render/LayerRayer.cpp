#include "LayerRayer.h"

namespace OddityEngine::Graphics::Render {
    LayerRayer::LayerRayer(size_t layers, Camera* camera) : layers(layers), camera(camera) {
        std::vector<float> screen = {
            0.0f, 0.0f, -1.0f,
            0.0f, 1.0f, -1.0f,
            1.0f, 0.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            0.0f, 1.0f, -1.0f,
            1.0f, 0.0f, -1.0f,
        };

        create_buffer_object_list(&screenbuffer, screen);
    }

    LayerRayer::~LayerRayer() {
        glDeleteFramebuffers(1, &layered_framebuffer);
        glDeleteTextures(1, &texture);
    }

    void LayerRayer::render() {
        if (screen_ratio != last_ratio) {
            set_size(screen_size * screen_ratio);
            last_ratio = screen_ratio;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, layered_framebuffer);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int l = 0; l < layers; l++) {
            glViewportIndexedf(l, 0, 0, size.x / pow(2, l), size.y / pow(2, l));
        }

        Object::bind_buffer();
        Shape::bind_buffer();
        Material::bind_buffer();
        Material::activate(0);

        float aspect = static_cast<float>(screen_size.x) / static_cast<float>(screen_size.y);
        float fov = glm::radians(camera->fov);

        auto projection = glm::perspective(fov, aspect, 0.1f, 100.0f);
        auto view = glm::lookAt(camera->position, camera->position + camera->front(), camera->up());

        for (auto p : program_object_map) {
            p.first->apply();

            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, *Shape::get_vertex_buffer());
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, *Shape::get_uv_buffer());
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
            for (auto o : p.second) {
                glm::mat4 mvp = projection * view * o->get_transform();
                glUniformMatrix4fv(p.first->uniform_location("mvp"), 1, GL_FALSE, &mvp[0][0]);
                glUniform1ui(p.first->uniform_location("object"), o->get_index());

                glDrawArrays(GL_TRIANGLES, o->get_shape()->vertex_start(), o->get_shape()->vertex_count());
            }
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
        }

        for (int l = 1; l < layers; l++) {
            glUniform1ui(screen_program.uniform_location("layer"), l);


        }
    }

    void LayerRayer::add_object(Program* program, Graphics::Object* object) {
        program_object_map[program].emplace_back(object);
    }

    void LayerRayer::set_screen_size(const glm::vec2& size) {
        Interface::set_size(size * screen_ratio);
        Interface::set_screen_size(size);
    }
}