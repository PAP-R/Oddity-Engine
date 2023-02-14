#include <chrono>
#include "base/Window.h"
#include "object/Cube.h"
#include "source/object/Player.h"
#include "source/base/tools/Debug.h"

int main() {
    if (!glfwInit()) {
        throw runtime_error("Failed to initialize GLFW");
    }

    glEnable(GL_DEBUG_OUTPUT);

    int width = 1080, height = 720;

    auto window = new Window("Oddity", width, height);

    Cube cube1(1, vec3(0));
    Cube cube2(1, vec3(3, 0, 0), vec3(0, 1, 0), vec3(5), loadShader("shaders/vert.shader"), loadShader("shaders/linefrag.shader"));
    //Cube cube3(4, vec3(0, -5, -5), vec3(0, 1, 0));

    window->addObject(&cube1);
    window->addObject(&cube2);
    //window->addObject(&cube3);

    Camera camera(vec3(0, 4, 4), vec2(pi<float>(), -0.8));

    window->setCamera(&camera);

    Player player;

    player.camera = &camera;

    window->setCursor(GLFW_CURSOR_DISABLED);

    window->addCallback(&player);

    window->setCallbacks();

    chrono::duration<float> t;
    auto tstart = chrono::system_clock::now();

    do {
        Debug::clear_text();
        player.move();
    } while(window->loop());

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    delete window;
    glfwTerminate();
}