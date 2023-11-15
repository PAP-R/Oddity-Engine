#include <OddityEngine.h>
#include "Graphics/Window.h"
#include <Graphics/Render/Tracer.h>
#include <Graphics/Buffer/Buffer.h>
#include <Graphics/Buffer/Bufferobject.h>
#include <Util/Transform.h>
#include <Util/Debug.h>
#include <Util/Debug.h>

int main() {
    OddityEngine::init();

    auto window = new OddityEngine::Graphics::Window("Oddity", 400, 400);
    auto scene = new OddityEngine::Graphics::Render::Scene();
    window->set_scene(scene);
    auto tracer = new OddityEngine::Graphics::Render::Tracer();
    scene->add_renderer(tracer);

    auto cube_vertices = OddityEngine::Graphics::create_buffer_object_list(tracer->vertexbuffer, OddityEngine::Graphics::Render::obj_to_vert(OddityEngine::Graphics::Loader::obj("models/cube.obj")));

    auto basic_white = OddityEngine::Graphics::Bufferobject(tracer->materialbuffer, OddityEngine::Graphics::Render::buffermaterial(glm::vec4(1), glm::vec4(0), 0));
    auto glow_white = OddityEngine::Graphics::Bufferobject(tracer->materialbuffer, OddityEngine::Graphics::Render::buffermaterial(glm::vec4(1), glm::vec4(1), 0));

    auto box = OddityEngine::Graphics::Bufferobject(tracer->objectbuffer, OddityEngine::Graphics::Render::bufferobject(OddityEngine::Util::transform({0, 0, 0}, {0, 0, 0}, {5, 5, 5}), 1, OddityEngine::Graphics::Render::MESH, basic_white.get_index(), cube_vertices[0].get_index(), cube_vertices.size()));
    auto glow_box = OddityEngine::Graphics::Bufferobject(tracer->objectbuffer, OddityEngine::Graphics::Render::bufferobject(OddityEngine::Util::transform({0, 0, 1}, {0, 0, 0}, {1, 1, 1}), 1, OddityEngine::Graphics::Render::MESH, glow_white.get_index(), cube_vertices[0].get_index(), cube_vertices.size()));

    float roughness = 0;
    OddityEngine::Debug::add_value([&](){ImGui::SliderFloat("Roughness", &roughness, 0, 1);});

    do {
        basic_white.set(OddityEngine::Graphics::Render::buffermaterial(glm::vec4(1), glm::vec4(0), roughness));
    } while (OddityEngine::update());

    OddityEngine::terminate();
}