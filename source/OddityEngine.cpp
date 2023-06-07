#include "OddityEngine.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
using namespace glm;

#include <stdexcept>
using namespace std;

#include <chrono>


void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    printf("%s\n", message);
}


OddityEngine::OddityEngine() {
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

    glDebugMessageCallback(MessageCallback, nullptr);

    glewExperimental = true;

//    glEnable(GL_DEBUG_OUTPUT);
//    glEnable(GL_MULTISAMPLE);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);
//    glEnable(GL_CULL_FACE);

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glClearColor(0.0, 0.0, 0.0, 1.0);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    camera = new Camera(vec3(0, 0, 1), vec3(0));

    Control::set_camera(camera);
    Control::set_callbacks(window);

    tracer = new Tracer(size, camera);

    tStart = chrono::system_clock::now();
    tNow = tStart;
    tLast = tStart;
}

OddityEngine::~OddityEngine() {
    delete(tracer);
    delete(camera);
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool OddityEngine::update() {
    glfwMakeContextCurrent(window);

    glfwPollEvents();

    tNow = chrono::system_clock::now();

    chrono::duration<float> deltaTime = tNow - tLast;
    tLast = tNow;

    Control::update(deltaTime.count());
    tracer->loop(deltaTime.count());

    glfwSwapBuffers(window);

//    return !glfwWindowShouldClose(window);
    return true;
}

float OddityEngine::get_time() {
    chrono::duration<float> time = tNow - tStart;
    return time.count();
}

GLFWwindow *OddityEngine::get_window() {
    return window;
}
