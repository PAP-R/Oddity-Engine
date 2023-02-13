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

    auto window = Window("Oddity", width, height);

    Cube cube;
    Cube biggerCube(4);


    GraphicsObject cubeGO(cube.pos);
    GraphicsObject cubeGO2(cube.pos + vec3(1, 0, 0), vec3(1, 1, 1));
    GraphicsObject cubeGO3(cube.pos + vec3(0, -5, -5), vec3(0, 1, 0));

    cubeGO.addData(3, cube.vertices, GL_STATIC_DRAW);
    cubeGO2.addData(3, cube.vertices, GL_STATIC_DRAW);
    cubeGO3.addData(3, biggerCube.vertices, GL_STATIC_DRAW);

    window.addObject(cubeGO);
    window.addObject(cubeGO2);
    window.addObject(cubeGO3);

    Camera camera;

    window.setCamera(&camera);

    Player player;

    player.camera = &camera;

    window.setCursor(GLFW_CURSOR_DISABLED);

    window.addCallback(&player);

    window.setCallbacks();

    chrono::duration<float> t;
    auto tstart = chrono::system_clock::now();

    do {
        Debug::clear_text();
        player.move();
    } while(window.loop());

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    window.selfdestruct();
    glfwTerminate();
}