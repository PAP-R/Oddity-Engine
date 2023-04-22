#include "WindowOpenGL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"

using namespace glm;

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "GL/glew.h"

#include "GLFW/glfw3.h"


#include <vector>
#include <map>
#include <algorithm>
using namespace std;

#include "fmt/core.h"
using namespace fmt;

#include "source/base/tools/Debug.h"
#include "source/base/interfaces/CallBack.h"

map<GLFWwindow*, WindowOpenGL*> winToWin;

/// Callbacks ///
/// Key ///
void WindowOpenGL::addCallback(CallBack *callback) {
    callbackList.emplace_back(callback);
}

void WindowOpenGL::removeCallback(CallBack *callback) {
    callbackList.erase(remove(callbackList.begin(), callbackList.end(), callback));
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    for (auto f : winToWin[window]->getCallbackList()) {
        f->key_callback(window, key, scancode, action, mods);
    }
}

void WindowOpenGL::setKeyCallback() {
    glfwSetKeyCallback(window, key_callback);
}

void cursor_callback(GLFWwindow *window, double xpos, double ypos) {
    for (auto f : winToWin[window]->getCallbackList()) {
        f->cursor_callback(window, xpos, ypos);
    }
}

void WindowOpenGL::setCursorCallback() {
    glfwSetCursorPosCallback(window, cursor_callback);
}

void mouse_callback(GLFWwindow *window, int button, int action, int mods) {
    for (auto f : winToWin[window]->getCallbackList()) {
        f->mouse_callback(window, button, action, mods);
    }
}

void WindowOpenGL::setMouseButtonCallback() {
    glfwSetMouseButtonCallback(window, mouse_callback);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    for (auto f : winToWin[window]->getCallbackList()) {
        f->scroll_callback(window, xoffset, yoffset);
    }
}

void WindowOpenGL::setScrollCallback() {
    glfwSetScrollCallback(window, scroll_callback);
}


void WindowOpenGL::setCallbacks() {
    setKeyCallback();
    setCursorCallback();
    setMouseButtonCallback();
    setScrollCallback();
}


/// Toggles ///
void WindowOpenGL::setCursor(int mode) {
    glfwSetInputMode(window, GLFW_CURSOR, mode);
}

void WindowOpenGL::setResolution(size_t width, size_t height) {
    size = vec2(width, height);
    glfwSetWindowSize(window, width, height);
}

void WindowOpenGL::setWindowPos(size_t x, size_t y) {
    this->position = vec2(x, y);
    glfwSetWindowPos(window, x, y);
}

void WindowOpenGL::setWindowMode(int mode) {
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    windowMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    switch (mode) {
        case SCREEN_MODE_WINDOW:
            //glfwSetWindowMonitor(window, nullptr, position.x, position.y, size.x, size.y, nullptr);
            glfwSetWindowMonitor(window, nullptr, 0, 0, windowMode->width, windowMode->height, windowMode->refreshRate);
            break;
        case SCREEN_MODE_BORDERLESS:
            glfwSetWindowMonitor(window, monitor, 0, 0, windowMode->width, windowMode->height, windowMode->refreshRate);
            break;
        case SCREEN_MODE_FULLSCREEN:
            glfwSetWindowMonitor(window, monitor, 0, 0, size.x, size.y, windowMode->refreshRate);
            break;
    }
}


/// WindowOpenGL ///
WindowOpenGL::WindowOpenGL(const char *name, size_t width, size_t height, int x, int y) {
    position = vec2(x, y);
    size = vec2(width, height);

    glfwWindowHint(GLFW_SAMPLES, 16);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        printf("Failed to create WindowOpenGL\n");
        throw runtime_error("Failed to create WindowOpenGL");
    }

    glfwMakeContextCurrent(window);

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

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
    glEnable(GL_CULL_FACE);

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glClearColor(0.1, 0.1, 0.1, 1.0);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    winToWin.emplace(window, this);
}

WindowOpenGL::~WindowOpenGL() {
    glfwDestroyWindow(window);
}

void WindowOpenGL::addObject(Graphics* go) {
    objects.emplace_back(go);
}

void WindowOpenGL::removeObject(Graphics* go) {
    objects.erase(remove(objects.begin(), objects.end(), go));
}

void WindowOpenGL::setCamera(Camera *cam) {
    camera = cam;
}


bool WindowOpenGL::loop(float deltaSeconds) {
    runtimeSeconds += deltaSeconds;
    float fps = 1 / deltaSeconds;

    Debug::add_text(format("FPS: {}\n", fps));
    Debug::add_text(format("Camera:\nPos:\t[ {:.1f} | {:.1f} | {:.1f} ]\nDir:\t[ {:.1f} | {:.1f} | {:.1f} ]\nAng:\t[ {:.1f} | {:.1f} ]\n", camera->position.x, camera->position.y, camera->position.z, camera->direction().x, camera->direction().y, camera->direction().z, camera->angle.x, camera->angle.y));

    glfwPollEvents();
    glfwMakeContextCurrent(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Hallo", nullptr, 0 | (ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration));
    ImGui::Text("%s", Debug::get_text().c_str());
    ImGui::End();

    mat4 projection = perspective(radians(camera->fov), float(size.x) / float(size.y), 0.1f, 100.0f);

    mat4 view = lookAt(camera->position, camera->position + camera->direction(), camera->up());

    vec3 globalIllumination = vec3(cos(runtimeSeconds), -1, sin(runtimeSeconds));

    for (auto o : objects) {
        glUseProgram(o->get_program());

        GLint glilUBO = glGetUniformLocation(o->get_program(), "GLIL");

        glUniform3f(glilUBO, globalIllumination.x, globalIllumination.y, globalIllumination.z);

        GLint cameraUBO = glGetUniformLocation(o->get_program(), "CAMERAPOS");

        glUniform3f(cameraUBO, camera->position.x, camera->position.y, camera->position.z);

        GLint shaderTime = glGetUniformLocation(o->get_program(), "TIME");

        glUniform1f(shaderTime, runtimeSeconds);

        vec3 up{0, 1, 0};

        vec3 right = normalize(cross(o->dir, up));

        right = all(isnan(right)) ? up : right;

        float angle = orientedAngle(normalize(o->dir), normalize(up), right);

        mat4 model = rotate(scale(translate(mat4(1.0f), o->pos), o->scale), angle, right);

        mat4 mvp = projection * view * model;

        GLuint matrix = glGetUniformLocation(o->get_program(), "MVP");

        glUniformMatrix4fv(matrix, 1, GL_FALSE, &mvp[0][0]);

        for (size_t i = 0; i < o->get_buffer_size(); i++) {
            switch (o->get_type()[i]) {
                case GL_ELEMENT_ARRAY_BUFFER:
                    glBindBuffer(o->get_type()[i], o->get_buffers()[i]);
                    break;
                default:
                    glEnableVertexAttribArray(i);
                    glBindBuffer(o->get_type()[i], o->get_buffers()[i]);
                    glVertexAttribPointer(i, o->get_blocksize()[i], GL_FLOAT, GL_FALSE, 0, nullptr);
                    break;
            }
        }

        glDrawElements(GL_TRIANGLES, o->get_size(), GL_UNSIGNED_INT, 0);

        for (size_t i = 0; i < o->get_buffers().size(); i++) {
            glDisableVertexAttribArray(i);
        }
    }

    for (size_t i = 0; i < Debug::get_points().size(); i++) {
        auto ray = Debug::get_points()[i] - camera->position;
        ray = ray * camera->direction();
        auto raylength = length(ray);
        auto shortray = normalize(ray);
        auto point = project(Debug::get_points()[i], view, projection, vec4(0, 0, size.x, size.y));
        auto point2d = vec2(point.x, size.y - point.y);

        if ((ray.x + ray.y + ray.z) < 0) {
            continue;
        }

        ImGui::SetNextWindowPos(ImVec2(point2d.x, point2d.y));
        ImGui::SetNextWindowSize(ImVec2(400, 64));
        ImGui::Begin(to_string(i).data(), nullptr, (ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration));

        ImGui::Text("%s", Debug::get_point_texts()[i].data());
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);

    return !glfwWindowShouldClose(window);
}

const vector<CallBack*> &WindowOpenGL::getCallbackList() const {
    return callbackList;
}

Camera * WindowOpenGL::getCamera() {
    return camera;
}
