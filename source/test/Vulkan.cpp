#include <vulkan/vulkan.h>

#include <OddityEngine.h>

#include <Graphics/Vulkan/Window.h>

void initVulkan() {

}

void update() {

}

void cleanup() {

}

int main(int argc, char* args[]) {
    auto window = OddityEngine::Graphics::Vulkan::Window("Hallo", 400, 400, SDL_WINDOW_RESIZABLE);

    do {

    } while (OddityEngine::update());

    return 0;
}