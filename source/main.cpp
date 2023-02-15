#include <chrono>
#include "base/Window.h"
#include "object/Cube.h"
#include "source/object/Player.h"
#include "source/base/tools/Debug.h"

int main() {
    if (!glfwInit()) {
        throw runtime_error("Failed to initialize GLFW");
    }

    int width = 1080, height = 720;

    auto window = new Window("Oddity", width, height);

    Cube cube1(vec3(0, 100, 0));
    Cube cube2(vec3(0, 0, 0), vec3(0, 1, 0), vec3(2), loadShader("shaders/vert.shader"), loadShader("shaders/linefrag.shader"));
    Cube cube3(vec3(0, -5, 0), vec3(0, 1, 0), vec3(10, 1, 10));

    window->addObject(&cube1);
    window->addObject(&cube2);
    window->addObject(&cube3);

    Camera camera(vec3(0, 4, 4), vec2(pi<float>(), -0.8));

    window->setCamera(&camera);

    Player player;

    player.camera = &camera;

    window->setCursor(GLFW_CURSOR_DISABLED);

    window->addCallback(&player);

    window->setCallbacks();

    auto tStart = chrono::system_clock::now();
    auto tLast = chrono::system_clock::now();
    chrono::duration<float> deltaTime;
    do {
        auto tNow = chrono::system_clock::now();
        deltaTime = tNow - tLast;
        tLast = tNow;

        Debug::clear_text();
        player.move();
        cube1.loop(deltaTime.count());
        cube2.loop(deltaTime.count());
    } while(window->loop(deltaTime.count()));

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    delete window;
    glfwTerminate();
}