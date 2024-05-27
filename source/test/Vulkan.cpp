#include <vulkan/vulkan.h>

#include <OddityEngine.h>

#include <Graphics/Vulkan/Window.h>

#include <shaderc/shaderc.hpp>

void initVulkan() {

}

void update() {

}

void cleanup() {

}

int main(int argc, char* args[]) {
    OddityEngine::init();

    auto window = OddityEngine::Graphics::Vulkan::Window("Hallo", 400, 400, SDL_WINDOW_RESIZABLE);

    do {

    } while (OddityEngine::update());

    OddityEngine::terminate();

    return 0;
}