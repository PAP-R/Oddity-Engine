#include "Raysterizer.h"

#include <glm/gtx/quaternion.hpp>

namespace OddityEngine::Graphics::Render {
    void Raysterizer::create_depthbuffer() {
        glGenRenderbuffers(1, &depthbuffer);
        resize_depthbuffer();
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);
    }

    void Raysterizer::resize_depthbuffer() {
        glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);
    }

    Raysterizer::Raysterizer(Camera* camera) : camera(camera) {
        create_depthbuffer();
    }

    Raysterizer::Raysterizer(Buffer<glm::vec4>* texture_transform_buffer, Camera* camera) : Interface(texture_transform_buffer), camera(camera) {
        create_depthbuffer();
    }

    Raysterizer::~Raysterizer() {
        glDeleteRenderbuffers(1, &depthbuffer);
    }

    void Raysterizer::render() {
        if (ratio != last_ratio) {
            set_size(screen_size * ratio);
            last_ratio = ratio;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, size.x, size.y);

        Object::bind_buffer();
        Shape::bind_buffer();
        Material::bind_buffer();
        Material::activate(0);

        float aspect = static_cast<float>(screen_size.x) / static_cast<float>(screen_size.y);
        float fov = glm::radians(camera->fov);

        auto projection = glm::perspective(fov, aspect, 0.1f, 100.0f);
        // auto projection = glm::mat4(1);
        // projection[0][0] = fov / aspect;
        // projection[1][1] = fov;

        auto view = glm::lookAt(camera->position, camera->position + camera->front(), camera->up());
        for (auto p : program_object_map) {
            p.first->apply();

            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, *Shape::get_vertex_buffer());
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, *Shape::get_normal_buffer());
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

            glEnableVertexAttribArray(2);
            glBindBuffer(GL_ARRAY_BUFFER, *Shape::get_uv_buffer());
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

            for (auto o : p.second) {
                glm::mat4 mvp = projection * view * o->get_transform();
                glUniformMatrix4fv(p.first->uniform_location("mvp"), 1, GL_FALSE, &mvp[0][0]);
                glUniformMatrix4fv(p.first->uniform_location("model"), 1, GL_FALSE, &o->get_transform()[0][0]);
                glUniform1ui(p.first->uniform_location("object"), o->get_index());
                glUniform3f(p.first->uniform_location("camera_pos"), camera->position.x, camera->position.y, camera->position.z);

                glUniform1ui(p.first->uniform_location("bounces"), 1);
                glUniform1ui(p.first->uniform_location("spread"), 1);


                glDrawArrays(GL_TRIANGLES, o->get_shape()->vertex_start(), o->get_shape()->vertex_count());

            }
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
        }
    }

    void Raysterizer::add_object(Program* program, Graphics::Object* object) {
        program_object_map[program].emplace_back(object);
    }

    void Raysterizer::set_size(const glm::vec2& size) {
        Interface::set_size(size);
        resize_depthbuffer();
    }


    void Raysterizer::set_screen_size(const glm::vec2 &size) {
        set_size(size * ratio);
        Interface::set_screen_size(size);
    }
}