#include <chrono>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
using namespace glm;

#include <list>
#include <stdexcept>
using namespace std;

#include "base/Shader.h"
#include "source/base/Tracer.h"
#include "base/Control.h"

GLFWwindow* window;

chrono::time_point<chrono::system_clock> tStart;
chrono::time_point<chrono::system_clock> tLast;
chrono::time_point<chrono::system_clock> tNow;

Tracer* tracer;

void update() {
    glfwPollEvents();

    tNow = chrono::system_clock::now();

    tracer->set_object({{0, 0, 0, 1}, {1, 0.7882, 0.1333, 1}, Tracer::transform(vec3(0), vec3(0), vec3(1)), SPHERE});

    chrono::duration<float> deltaTime = tNow - tLast;
    tLast = tNow;

    glfwMakeContextCurrent(window);

    Control::update(deltaTime.count());
    tracer->loop(deltaTime.count());

    glfwSwapBuffers(window);
}

int main() {
    if (!glfwInit()) {
        throw runtime_error("Failed to initialize GLFW");
    }

    int width = 2000, height = 1000;

    vec2 size = vec2(width, height);

    glfwWindowHint(GLFW_SAMPLES, 16);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Oddity", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        printf("Failed to create WindowOpenGL\n");
        throw runtime_error("Failed to create WindowOpenGL");
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW");
        throw runtime_error("Failed to initialize GLEW");
    }

    glewExperimental = true;

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
//    glEnable(GL_CULL_FACE);

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glClearColor(0.0, 0.0, 0.0, 1.0);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    Camera camera = Camera(vec3(0, 0, 1), vec3(0));

    Control::set_camera(&camera);
    Control::set_callbacks(window);

    tracer = new Tracer(size, &camera);

    tStart = chrono::system_clock::now();
    tNow = tStart;
    tLast = tStart;

    while(!glfwWindowShouldClose(window)) {
        update();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}