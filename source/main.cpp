#include "OddityEngine.h"

#include <fmt/core.h>
#include <fmt/chrono.h>
#include <thread>

#include "util/Value.h"

#include "graphics/render/Tracer.h"

#include "graphics/buffer/Bufferobject.h"

void mirror_ball(OddityEngine::Graphics::Render::Tracer* tracer) {
    auto cube_vertices = OddityEngine::Graphics::Buffer::create_object_list(&tracer->vertexbuffer, OddityEngine::Graphics::Render::Tracer::obj_to_vert(OddityEngine::Graphics::Loader::obj("models/cube.obj")));
    auto plane_vertices = OddityEngine::Graphics::Buffer::create_object_list(&tracer->vertexbuffer, OddityEngine::Graphics::Render::Tracer::obj_to_vert(OddityEngine::Graphics::Loader::obj("models/plane.obj")));
    auto sphere_vertices = OddityEngine::Graphics::Buffer::create_object_list(&tracer->vertexbuffer, OddityEngine::Graphics::Render::Tracer::obj_to_vert(OddityEngine::Graphics::Loader::obj("models/sphere.obj")));

    auto basic_red_transparent = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->materialbuffer, OddityEngine::Graphics::Render::buffermaterial(vec4(0.7, 0.3, 0.3, 0.3), vec4(0), 0));
    auto transparent_white = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->materialbuffer, OddityEngine::Graphics::Render::buffermaterial(vec4(1, 1, 1, 0.1), vec4(0), 0));
    auto basic_blue = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->materialbuffer, OddityEngine::Graphics::Render::buffermaterial(vec4(0.3, 0.3, 1, 1), vec4(0.3, 0.3, 1, 1), 0.5));
    auto basic_black = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->materialbuffer, OddityEngine::Graphics::Render::buffermaterial(vec4(0.1, 0.1, 0.1, 1), vec4(0), 0));
    auto sky_light = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->materialbuffer, OddityEngine::Graphics::Render::buffermaterial(vec4(0), vec4(135, 206, 235, 255) / 255.0f, 0));
    auto glow_white = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->materialbuffer, OddityEngine::Graphics::Render::buffermaterial(vec4(1), vec4(10), 1));
    auto glow_grey = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->materialbuffer, OddityEngine::Graphics::Render::buffermaterial(vec4(0, 0, 0, 1), vec4(0.5, 0.5, 0.5, 1), 0));
    auto basic_white = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->materialbuffer, OddityEngine::Graphics::Render::buffermaterial(vec4(1), vec4(0), 1));
    auto glow_blue = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->materialbuffer, OddityEngine::Graphics::Render::buffermaterial(vec4(0), vec4(0, 0, 1, 1), 1));
    auto glow_red = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->materialbuffer, OddityEngine::Graphics::Render::buffermaterial(vec4(0), vec4(1, 0, 0, 1), 1));

    auto sphere = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->objectbuffer, OddityEngine::Graphics::Render::bufferobject(OddityEngine::Graphics::transform(vec3(0, 0, 0), vec3(0), vec3(5)), 1, OddityEngine::Graphics::Render::MESH, basic_white.get_index(), sphere_vertices[0].get_index(), sphere_vertices.size()));
//    auto cube = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->objectbuffer, OddityEngine::Graphics::bufferobject(OddityEngine::Graphics::transform(vec3(0, 0, 0.7), vec3(0), vec3(0.5)), 1, OddityEngine::Graphics::MESH, glow_white.get_index(), cube_vertices[0].get_index(), cube_vertices.size()));
//    auto plane = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->objectbuffer, OddityEngine::Graphics::bufferobject(OddityEngine::Graphics::transform(vec3(0, -1.5, 0), radians(vec3(90, 0, 0)), vec3(100)), 1, OddityEngine::Graphics::MESH, basic_black.get_index(), plane_vertices[0].get_index(), plane_vertices.size()));

//    auto orb = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->objectbuffer, OddityEngine::Graphics::bufferobject(OddityEngine::Graphics::transform(vec3(0, 0, 0), vec3(0), vec3(2.9)), 2, OddityEngine::Graphics::SPHERE, transparent_white.get_index()));
    auto inner_orb = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->objectbuffer, OddityEngine::Graphics::Render::bufferobject(OddityEngine::Graphics::transform(vec3(0, 0, 0.7), vec3(0), vec3(0.5)), 0.2, OddityEngine::Graphics::Render::SPHERE, glow_white.get_index()));
    auto outer_orb = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->objectbuffer, OddityEngine::Graphics::Render::bufferobject(OddityEngine::Graphics::transform(vec3(0, 0, 0.7), vec3(0), vec3(0.5)), 0.2, OddityEngine::Graphics::Render::SPHERE, glow_white.get_index()));
    auto sky = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->objectbuffer, OddityEngine::Graphics::Render::bufferobject(OddityEngine::Graphics::transform(vec3(0), vec3(0), vec3(1000)), 100, OddityEngine::Graphics::Render::SPHERE, glow_grey.get_index()));

    float roughness = 0;
    OddityEngine::Debug::add_value([&](){ImGui::SliderFloat("Roughness", &roughness, 0, 1);});

    float time = OddityEngine::Time::get_runtime<std::chrono::milliseconds, float>();
    OddityEngine::Debug::add_value([&](){ImGui::Text("%s", fmt::format("Time: [ {: 05.05f} ]", time).c_str());});

    do {
        time = OddityEngine::Time::get_runtime<std::chrono::milliseconds, float>();

        basic_white.set(OddityEngine::Graphics::Render::buffermaterial(vec4(1, 1, 1, 1), vec4(0), roughness));

        inner_orb.set(OddityEngine::Graphics::Render::bufferobject(OddityEngine::Graphics::transform(vec3(sin(time), cos(time), 3), vec3(0), vec3(0.2)), 1, OddityEngine::Graphics::Render::SPHERE, glow_red.get_index(), sphere_vertices[0].get_index(), sphere_vertices.size()));
        outer_orb.set(OddityEngine::Graphics::Render::bufferobject(OddityEngine::Graphics::transform(vec3(-sin(time), -cos(time), 3), vec3(0), vec3(0.2)), 1, OddityEngine::Graphics::Render::MESH, glow_blue.get_index(), sphere_vertices[0].get_index(), sphere_vertices.size()));
//        sphere.set(OddityEngine::Graphics::bufferobject(OddityEngine::Graphics::transform(vec3(0, 0, 0.7), vec3(sin(time), cos(time), sin(time)), vec3(1)), 1, OddityEngine::Graphics::MESH, glow_grey.get_index(), sphere_vertices[0].get_index(), sphere_vertices.size()));
    } while (OddityEngine::update());
}

void polyspheres(OddityEngine::Graphics::Render::Tracer* tracer) {
    auto sphere_vertices = OddityEngine::Graphics::Buffer::create_object_list(&tracer->vertexbuffer, OddityEngine::Graphics::Render::Tracer::obj_to_vert(OddityEngine::Graphics::Loader::obj("models/sphere.obj")));

    auto glow_white = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->materialbuffer, OddityEngine::Graphics::Render::buffermaterial(vec4(1), vec4(10), 1));

    std::vector<OddityEngine::Graphics::Buffer::Bufferobject<OddityEngine::Graphics::Render::bufferobject>> spheres;
    for (int i = 0; i < 10; i++) {
        spheres.emplace_back(&tracer->objectbuffer, OddityEngine::Graphics::Render::bufferobject(OddityEngine::Graphics::transform(vec3(0, i * 3, 0), vec3(0), vec3(1)), 1, OddityEngine::Graphics::Render::MESH, glow_white.get_index(), sphere_vertices[0].get_index(), sphere_vertices.size()));
    }

    do {} while (OddityEngine::update());
}

int main() {
    OddityEngine::init();

    auto window = OddityEngine::Graphics::create_window("Oddity", 500, 500);
    auto tracer = OddityEngine::Graphics::Render::Tracer(window->get_texture_transform_buffer());
    tracer.set_size(vec2(400, 400));

    window->add_renderer(&tracer);

    polyspheres(&tracer);
//    mirror_ball(&tracer);

    OddityEngine::terminate();


    /*
     * Vsync I guess
     * std::this_thread::sleep_for(std::chrono::duration<double>(slower_threshold - slower));
     */
}