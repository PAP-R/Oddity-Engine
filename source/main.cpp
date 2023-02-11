#include <chrono>
#include "base/Window.h"
#include "object/Cube.h"
#include "source/object/Player.h"

int main() {
    if (!glfwInit()) {
        throw runtime_error("Failed to initialize GLFW");
    }
    int width = 1080, height = 720;

    auto window = Window("Oddity", width, height);

    Cube cube;

    cube.pointsToFloat();

    GraphicsObject cubeGO(cube.pos);
    GraphicsObject cubeGO2(cube.pos + vec3(1, 0, 0), vec3(1, 1, 1));

    cubeGO.addData(3, cube.vertices, GL_STATIC_DRAW);
    cubeGO2.addData(3, cube.vertices, GL_STATIC_DRAW);

    window.addObject(cubeGO);
    window.addObject(cubeGO2);

    window.setCamera(Camera(vec3(2, 0, 0), vec2(-quarter_pi<float>(), -quarter_pi<float>())));

    Player player;

    window.setCursor(GLFW_CURSOR_DISABLED);

    window.addCallback(&player);

    window.setKeyCallback();
    window.setCursorCallback();

    chrono::duration<float> t;
    auto tstart = chrono::system_clock::now();

    while(window.loop()) {
        player.move();
        window.getCamera()->position = player.position;
        window.getCamera()->angle = player.angle;

    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    window.selfdestruct();
    glfwTerminate();
}