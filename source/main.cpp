#include <chrono>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
using namespace glm;

#include <list>
#include <stdexcept>
using namespace std;

#include "base/Shader.h"
#include "base/Tracer.h"

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    printf("%s\n", message);
}


int main() {
    if (!glfwInit()) {
        throw runtime_error("Failed to initialize GLFW");
    }

    int width = 1080, height = 720;

    vec2 size = vec2(width, height);

    glfwWindowHint(GLFW_SAMPLES, 16);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(width, height, "Oddity", NULL, NULL);
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

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
//    glEnable(GL_CULL_FACE);

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glClearColor(0.0, 0.0, 0.0, 1.0);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    Camera camera(vec3(0), vec2(0));

    Tracer tracer(size, &camera);

    auto tStart = chrono::system_clock::now();
    auto tLast = chrono::system_clock::now();
    auto tNow = chrono::system_clock::now();
    chrono::duration<float> deltaTime;

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        tNow = chrono::system_clock::now();
        deltaTime = tNow - tLast;
        tLast = tNow;

        glfwMakeContextCurrent(window);

        tracer.loop(deltaTime.count());

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}