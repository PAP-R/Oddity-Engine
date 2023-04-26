#include <chrono>
#include "source/base/window/WindowOpenGL.h"
#include "object/Cube.h"
#include "source/object/Player.h"
#include "source/base/tools/Debug.h"
#include "base/objects/Physics.h"
#include "source/object/Sphere.h"

#include <list>

int main() {
    if (!glfwInit()) {
        throw runtime_error("Failed to initialize GLFW");
    }

    int width = 1080, height = 720;

    auto window = new WindowOpenGL("Oddity", width, height);

    Sphere center(vec3(0, 0, 0), vec3(0, 1, 0), vec3(20), loadShader("shaders/vert.shader"), loadShader("shaders/shadedfrag.shader"));
    center.setGravity(8.0f);
    window->addObject(&center);
    Sphere ball1(vec3(30, 30, 0), vec3(0, 1, 0), vec3(3), loadShader("shaders/vert.shader"), loadShader("shaders/shadedfrag.shader"));
    ball1.setMovable(true);
    ball1.setGravity(5);
    window->addObject(&ball1);
    Sphere ball2(vec3(30, 0, 0), vec3(0, 1, 0), vec3(2), loadShader("shaders/vert.shader"), loadShader("shaders/shadedfrag.shader"));
    ball2.setMovable(true);
    ball2.setGravity(5);
    window->addObject(&ball2);

    vector<Sphere*> spheres;
    for (int x = -1; x <= 1; x += 2) {
        for (int z = -3; z <= 3; z += 2) {
            for (int i = 0; i < 2; i++) {
                spheres.emplace_back(new Sphere(vec3(x * 3, 22 + i * 3, z * 3), vec3(0, 1, 0), vec3(1), loadShader("shaders/vert.shader"), loadShader("shaders/frag.shader")));
                spheres.back()->setMovable(true);
                spheres.back()->setGravity(0.1f);
                window->addObject(spheres.back());
            }
        }
    }

    Camera camera(vec3(30), vec2(-2.4, -0.6));

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
        player.move(deltaTime.count());
        Physics::update_physics_sub_steps(deltaTime.count(), 4);
    } while(window->loop(deltaTime.count()));

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    delete window;
    glfwTerminate();
}