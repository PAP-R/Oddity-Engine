#ifndef LAYERS_H
#define LAYERS_H

#include <map>

#include "Interface.h"

#include <Graphics/Texture/Atlas.h>
#include <Graphics/Shader/Program.h>

#include "Graphics/Camera.h"
#include "Graphics/Shader/Shader.h"


namespace OddityEngine::Graphics::Render {

class LayerRayer : public Interface {
protected:
    Camera* camera;

    std::vector<Graphics::Object*> object_list;

    GLuint depthbuffer = 0;

    Buffer<float> screenbuffer = Buffer<float>(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

    GLuint screen_framebuffer = 0;

    Shader mesh_vertex_shader = Shader(GL_VERTEX_SHADER, "object_ray.vert");
    Shader mesh_fragment_shader = Shader(GL_FRAGMENT_SHADER, "layered_object_ray.frag");
    Program mesh_program = Program({mesh_vertex_shader, mesh_fragment_shader});

    Shader point_vertex_shader = Shader(GL_VERTEX_SHADER, "point_ray.vert");
    Shader point_fragment_shader = Shader(GL_FRAGMENT_SHADER, "layered_point_ray.frag");
    Shader point_geometry_shader = Shader(GL_GEOMETRY_SHADER, "layered_point_ray.geom");
    Program point_program = Program({point_vertex_shader, point_fragment_shader, point_geometry_shader});

    Shader screen_vertex_shader = Shader(GL_VERTEX_SHADER, "layered_screen_ray.vert");
    Shader screen_fragment_shader = Shader(GL_FRAGMENT_SHADER, "layered_screen_ray.frag");
    Program screen_program = Program({screen_vertex_shader, screen_fragment_shader});

    Buffer<glm::vec4> layer_buffer = Buffer<glm::vec4>();
    GLuint layer_elements = 8;

    void create_buffers();
    void resize_buffers();

public:
    explicit LayerRayer(size_t layers, Camera* camera = new Camera());
    explicit LayerRayer(size_t layers, float layer_ratio, Camera* camera = new Camera());
    explicit LayerRayer(size_t layers, float ratio, float layer_ratio, Camera* camera = new Camera());
    ~LayerRayer() override;

    void render() override;

    void add_object(Program* program, Graphics::Object* object);
    void set_texture(GLuint texture, const std::vector<GLuint>&layers) override;

    void set_size(const glm::vec2 &size) override;
    void set_screen_size(const glm::vec2 &size) override;
};

}

#endif //LAYERS_H
