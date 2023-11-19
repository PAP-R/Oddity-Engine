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

    GLuint texture = 0;
    GLuint layered_framebuffer = 0;

    std::map<Program*, std::vector<Graphics::Object*>> program_object_map;

    size_t layers;

    float step_ratio = 0.5;
    float screen_ratio = 1;
    float last_ratio = screen_ratio;

    Buffer<float> screenbuffer;

    Shader vertex_shader = Shader(GL_VERTEX_SHADER, "screen_ray.vert");
    // Shader geometry_shader = Shader(GL_GEOMETRY_SHADER, "layered.geometry");
    Shader fragment_shader = Shader(GL_FRAGMENT_SHADER, "screen_ray.frag");
    Program screen_program = Program({vertex_shader, fragment_shader});

    void retexture();

public:
    LayerRayer(size_t layers, Camera* camera = new Camera());
    ~LayerRayer();

    void render() override;

    void add_object(Program* program, Graphics::Object* object);

    void set_screen_size(const glm::vec2 &size) override;
};

}

#endif //LAYERS_H
