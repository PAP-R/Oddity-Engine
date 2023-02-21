#include <chrono>
#include "base/Window.h"
#include "object/Cube.h"
#include "source/object/Player.h"
#include "source/base/tools/Debug.h"
#include "base/objects/Physics.h"

#include <list>

int main() {
    if (!glfwInit()) {
        throw runtime_error("Failed to initialize GLFW");
    }

    int width = 1080, height = 720;

    auto window = new Window("Oddity", width, height);

    Cube cube1(vec3(0, 0, 0), vec3(0, 1, 0), vec3(1), loadShader("shaders/vert.shader"), loadShader("shaders/frag.shader"));
//    cube1.setMovable(true);
    //Cube cube2(true, vec3(0, 0, 0), vec3(0, 1, 0), vec3(2), loadShader("shaders/vert.shader"), loadShader("shaders/linefrag.shader"));
//    Cube cube3(vec3(0, -5, 0), vec3(0, 1, 0), vec3(20, 4, 20));
//    cube3.setGravity(1.0f);

    window->addObject(&cube1);
    //window->addObject(&cube2);
//    window->addObject(&cube3);

    vector<Cube*> cubes;
    for (int x = -4; false && x <= 4; x += 2) {
        for (int z = -4; z <= 4; z += 2) {
            for (int i = 0; i < 1; i++) {
                cubes.emplace_back(new Cube(vec3(x * 1.1, 10 + i * 3, z * 1.1), vec3(0, 1, 0), vec3(1), loadShader("shaders/vert.shader"), loadShader("shaders/frag.shader")));
                window->addObject(cubes.back());
            }
        }
    }

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
        Physics::update_physics_sub_steps(deltaTime.count(), 1);
    } while(window->loop(deltaTime.count()));

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    delete window;
    glfwTerminate();
}