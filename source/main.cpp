#include "OddityEngine.h"

#include <fmt/core.h>
#include <fmt/chrono.h>
#include <thread>

#include "util/Value.h"

#include "graphics/Tracer.h"

#include "graphics/buffer/Bufferobject.h"

int main() {
    OddityEngine::init();

    auto window = OddityEngine::Graphics::create_window("Oddity", 500, 500);
    auto tracer = OddityEngine::Graphics::create_tracer(window, 1000, 1000);

    auto cube_vertices = OddityEngine::Graphics::Buffer::create_object_list(&tracer->vertexbuffer, OddityEngine::Graphics::Tracer::obj_to_vert(OddityEngine::Graphics::Loader::obj("models/cube.obj")));
    auto plane_vertices = OddityEngine::Graphics::Buffer::create_object_list(&tracer->vertexbuffer, OddityEngine::Graphics::Tracer::obj_to_vert(OddityEngine::Graphics::Loader::obj("models/plane.obj")));
    auto sphere_vertices = OddityEngine::Graphics::Buffer::create_object_list(&tracer->vertexbuffer, OddityEngine::Graphics::Tracer::obj_to_vert(OddityEngine::Graphics::Loader::obj("models/sphere.obj")));

    auto basic_red_transparent = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->materialbuffer, OddityEngine::Graphics::buffermaterial(vec4(0.7, 0.3, 0.3, 0.3), vec4(0), 0.5));
    auto basic_blue = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->materialbuffer, OddityEngine::Graphics::buffermaterial(vec4(0.3, 0.3, 1, 1), vec4(0.3, 0.3, 1, 1), 0.5));
    auto basic_black = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->materialbuffer, OddityEngine::Graphics::buffermaterial(vec4(0.1, 0.1, 0.1, 1), vec4(0), 0.5));
    auto sky_light = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->materialbuffer, OddityEngine::Graphics::buffermaterial(vec4(0), vec4(135, 206, 235, 255) / 255.0f, 1));
    auto glow_white = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->materialbuffer, OddityEngine::Graphics::buffermaterial(vec4(0.9), vec4(0.9), 1));
    auto glow_grey = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->materialbuffer, OddityEngine::Graphics::buffermaterial(vec4(0.5, 0.5, 0.5, 1), vec4(0.5, 0.5, 0.5, 1), 1));

    auto sphere = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->objectbuffer, OddityEngine::Graphics::bufferobject(OddityEngine::Graphics::transform(vec3(0, 0, 0.7), vec3(0), vec3(3)), 1, OddityEngine::Graphics::MESH, glow_grey.get_index(), sphere_vertices[0].get_index(), sphere_vertices.size()));
//    auto cube = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->objectbuffer, OddityEngine::Graphics::bufferobject(OddityEngine::Graphics::transform(vec3(0, 0, 0.7), vec3(0), vec3(0.5)), 1, OddityEngine::Graphics::MESH, glow_white.get_index(), cube_vertices[0].get_index(), cube_vertices.size()));
    auto plane = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->objectbuffer, OddityEngine::Graphics::bufferobject(OddityEngine::Graphics::transform(vec3(0, -1.5, 0), radians(vec3(90, 0, 0)), vec3(100)), 1, OddityEngine::Graphics::MESH, basic_black.get_index(), plane_vertices[0].get_index(), plane_vertices.size()));

    auto orb = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->objectbuffer, OddityEngine::Graphics::bufferobject(OddityEngine::Graphics::transform(vec3(0, 0, 0.7), vec3(0), vec3(1)), 0.2, OddityEngine::Graphics::SPHERE, basic_red_transparent.get_index()));
//    auto inner_orb = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->objectbuffer, OddityEngine::Graphics::bufferobject(OddityEngine::Graphics::transform(vec3(0, 0, 0.7), vec3(0), vec3(0.5)), 0.2, OddityEngine::Graphics::SPHERE, basic_blue.get_index()));
    auto sky = OddityEngine::Graphics::Buffer::Bufferobject(&tracer->objectbuffer, OddityEngine::Graphics::bufferobject(OddityEngine::Graphics::transform(vec3(0), vec3(0), vec3(1000)), 100, OddityEngine::Graphics::SPHERE, sky_light.get_index()));

    float time = OddityEngine::Time::get_runtime<std::chrono::milliseconds, float>();

    OddityEngine::Debug::add_value([&](){ImGui::Text("%s", fmt::format("Time: [ {: 05.05f} ]", time).c_str());});

    do {
        time = OddityEngine::Time::get_runtime<std::chrono::milliseconds, float>();
//        inner_orb.set(OddityEngine::Graphics::bufferobject(OddityEngine::Graphics::transform(vec3(sin(time) * 0.2, cos(time) * 0.2, 0.7 + sin(time) * 0.2), vec3(0), vec3(0.5)), 0.2, OddityEngine::Graphics::SPHERE, basic_blue.get_index()));
        sphere.set(OddityEngine::Graphics::bufferobject(OddityEngine::Graphics::transform(vec3(0, 0, 0.7), vec3(sin(time), cos(time), sin(time)), vec3(1)), 1, OddityEngine::Graphics::MESH, glow_grey.get_index(), sphere_vertices[0].get_index(), sphere_vertices.size()));
    } while (OddityEngine::update());

    OddityEngine::terminate();


    /*
     * Vsync I guess
     * std::this_thread::sleep_for(std::chrono::duration<double>(slower_threshold - slower));
     */
}