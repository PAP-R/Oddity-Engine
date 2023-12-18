#include "LayerRayer.h"

#include <glm/gtx/quaternion.hpp>

#include "Util/Debug.h"
#include "Util/Time.h"

namespace OddityEngine::Graphics::Render {
    void LayerRayer::create_buffers() {
        glGenFramebuffers(1, &screen_framebuffer);

        std::vector<float> screen = {
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
        };

        create_buffer_object_list(&screenbuffer, screen);

        glGenRenderbuffers(1, &depthbuffer);
        resize_buffers();
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);
    }

    void LayerRayer::resize_buffers() {
        glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);

        layer_buffer.resize(sizeof(glm::vec4) * size.x * size.y * layer_elements);
    }

    LayerRayer::LayerRayer(size_t layers, Camera* camera) : Interface(layers), camera(camera) {
        create_buffers();
    }

    LayerRayer::LayerRayer(size_t layers, float layer_ratio, Camera* camera) : LayerRayer(layers, camera) {
        this->layer_ratio = layer_ratio;
    }

    LayerRayer::LayerRayer(size_t layers, float ratio, float layer_ratio, Camera* camera) : LayerRayer(layers, layer_ratio, camera) {
        this->ratio = ratio;
    }

    LayerRayer::~LayerRayer() {
        glDeleteRenderbuffers(1, &depthbuffer);
        Interface::~Interface();
    }

    void LayerRayer::render() {
        if (ratio != last_ratio) {
            set_size(screen_size * ratio);
            last_ratio = ratio;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, size.x, size.y);

        glBindBufferBase(layer_buffer.get_type(), LAYER, layer_buffer);

        Object::bind_buffer();
        Shape::bind_buffer();
        Material::bind_buffer();
        Material::activate(0);

        float aspect = size.x / size.y;
        float fov = glm::radians(camera->fov);

        auto projection = glm::perspective(fov, aspect, 0.1f, 100.0f);

        auto view = glm::lookAt(camera->position, camera->position + camera->front(), camera->up());

        mesh_program.apply();

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, *Shape::get_vertex_buffer());
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, *Shape::get_normal_buffer());
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, *Shape::get_uv_buffer());
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

        glUniform3f(mesh_program.uniform_location("camera_pos"), camera->position.x, camera->position.y, camera->position.z);
        glUniform2f(mesh_program.uniform_location("screen_size"), size.x, size.y);
        glUniform1ui(mesh_program.uniform_location("layer_elements"), layer_elements);
        glUniform1ui(mesh_program.uniform_location("frame_clock"), Time::get_frame());
        glUniform1ui(mesh_program.uniform_location("bounces"), layer_count);

        for (auto o : object_list) {
            if (o->get_shape()->get_shape() == Shape::MESH) {
                glm::mat4 mvp = projection * view * o->get_transform();
                glUniformMatrix4fv(mesh_program.uniform_location("mvp"), 1, GL_FALSE, &mvp[0][0]);
                glUniformMatrix4fv(mesh_program.uniform_location("model"), 1, GL_FALSE, &o->get_transform()[0][0]);
                glUniform1ui(mesh_program.uniform_location("object"), o->get_index());

                glDrawArrays(GL_TRIANGLES, o->get_shape()->vertex_start(), o->get_shape()->vertex_count());
            }
        }

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);


        point_program.apply();

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, *Shape::get_vertex_buffer());
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, *Shape::get_normal_buffer());
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, *Shape::get_uv_buffer());
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

        glUniform3f(point_program.uniform_location("camera_pos"), camera->position.x, camera->position.y, camera->position.z);
        glUniform2f(point_program.uniform_location("screen_size"), size.x, size.y);
        glUniform1ui(point_program.uniform_location("layer_elements"), layer_elements);
        glUniform1ui(point_program.uniform_location("frame_clock"), Time::get_frame());
        glUniform1ui(point_program.uniform_location("bounces"), layer_count);

        auto render_projection = glm::mat4(1);
        render_projection[0][0] = fov / aspect;
        render_projection[1][1] = fov;
        auto render_view = glm::toMat4(camera->orientation);

        for (auto o : object_list) {
            if (o->get_shape()->get_shape() == Shape::SPHERE) {
                glm::mat4 screen_mvp = projection * view * o->get_transform();
                glm::mat4 ray_mvp = projection * view * o->get_transform();
                glm::mat4 v = glm::mat4(1) * view * glm::mat4(1);
                glm::mat4 mp = projection * glm::mat4(1) * o->get_transform();
                glUniformMatrix4fv(point_program.uniform_location("projection"), 1, GL_FALSE, &projection[0][0]);
                glUniformMatrix4fv(point_program.uniform_location("mp"), 1, GL_FALSE, &mp[0][0]);
                glUniformMatrix4fv(point_program.uniform_location("mvp"), 1, GL_FALSE, &screen_mvp[0][0]);
                glUniformMatrix4fv(point_program.uniform_location("ray_mvp"), 1, GL_FALSE, &ray_mvp[0][0]);
                glUniformMatrix4fv(point_program.uniform_location("view"), 1, GL_FALSE, &v[0][0]);
                glUniformMatrix4fv(point_program.uniform_location("model"), 1, GL_FALSE, &o->get_transform()[0][0]);
                glUniform1ui(point_program.uniform_location("object"), o->get_index());

                glDrawArrays(GL_POINTS, o->get_shape()->vertex_start(), o->get_shape()->vertex_count());
            }
        }

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);


        // way_too_much = layer_buffer.get_data();


        if (layer_count > 1) {
            glBindFramebuffer(GL_FRAMEBUFFER, screen_framebuffer);

            glBindBufferBase(layer_buffer.get_type(), LAYER, layer_buffer);

            Object::bind_buffer();
            Shape::bind_buffer();
            Material::bind_buffer();
            Material::activate(0);

            screen_program.apply();

            glm::mat4 screen_perspective = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
            glm::mat4 screen_projection = screen_perspective * glm::lookAt(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
            glUniformMatrix4fv(screen_program.uniform_location("screen_projection"), 1, GL_FALSE, &screen_projection[0][0]);

            aspect = screen_size.y / screen_size.x;
            // float fov = glm::radians(camera->fov);
            fov = camera->fov / 90.0f;

            auto render_projection = glm::mat4(1);
            render_projection[0][0] = fov / aspect;
            render_projection[1][1] = fov;
            auto render_view = glm::toMat4(camera->orientation);

            glm::mat4 render_mvp = render_view * render_projection * glm::mat4(1);

            glUniformMatrix4fv(screen_program.uniform_location("mvp"), 1, GL_FALSE, &render_mvp[0][0]);

            glUniform3f(screen_program.uniform_location("camera_pos"), camera->position.x, camera->position.y, camera->position.z);

            glUniform1ui(screen_program.uniform_location("layer_elements"), layer_elements);

            glUniform1ui(screen_program.uniform_location("frame_clock"), Time::get_frame());

            glEnableVertexAttribArray(0);

            for (int l = 1; l < layer_count; l++) {
                auto step_size = size * static_cast<float>(pow(layer_ratio, l));
                glViewport(0, 0, step_size.x, step_size.y);
                glUniform2f(screen_program.uniform_location("screen_size"), size.x, size.y);
                glUniform1i(screen_program.uniform_location("layer_in"), layers[l]);
                glUniform1f(screen_program.uniform_location("ratio"), pow(layer_ratio, l));
                glUniform1ui(screen_program.uniform_location("spread"), 1 / pow(layer_ratio, l));

                glBindBuffer(screenbuffer.get_type(), screenbuffer);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

                glDrawArrays(GL_TRIANGLES, 0, 6);
            }

            glDisableVertexAttribArray(0);
        }
    }

    void LayerRayer::add_object(Program* program, Graphics::Object* object) {
        object_list.emplace_back(object);
    }

    void LayerRayer::set_texture(GLuint texture, const std::vector<GLuint>& layers) {
        Interface::set_texture(texture, layers);

        glBindFramebuffer(GL_FRAMEBUFFER, screen_framebuffer);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

        glDrawBuffer(GL_COLOR_ATTACHMENT0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            Debug::error("Framebuffer not complete");
        }
    }

    void LayerRayer::set_size(const glm::vec2& size) {
        Interface::set_size(size);
        resize_buffers();
    }

    void LayerRayer::set_screen_size(const glm::vec2 &size) {
        Interface::set_screen_size(size);
        set_size(size * ratio);
    }
}
