#include <cstdio>
#include <cstdlib>
#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

#include "debug/Debug.h"

#include "base/Cube.h"
#include "base/HalfEdge.h"
#include "base/Player.h"
#include "common/loadShader.h"
#include "common/loadTexture.h"

static std::vector<GLfloat> g_color_buffer_data;

double lastTime = 0.0;

static void glfw_error_callback(int error, const char *description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

float weirdMod(float num, float mod) {
    while (num < -mod) {
        num += mod;
    }

    while (num > mod) {
        num -= mod;
    }

    return num;
}

void createColor(size_t c, GLuint colorbuffer) {
    if (g_color_buffer_data.size() != c) {
        g_color_buffer_data.resize(c);

        std::fill(g_color_buffer_data.begin(), g_color_buffer_data.end(), 0.5f);
    }

    double currentTime = glfwGetTime();
    auto deltaTime = float(currentTime - lastTime);
    lastTime = currentTime;

    for (float &v: g_color_buffer_data) {
        v += ((static_cast<float>(rand()) / RAND_MAX) * 2 - 1) * deltaTime * 10;
        v = weirdMod(v, 1.0f);
    }

    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, g_color_buffer_data.size() * sizeof(float), g_color_buffer_data.data(), GL_STATIC_DRAW);
}

int main() {
    for (size_t i = 3; i <= 12; i++) {
        printf("==\t==\t==\t==\t==\nPoints: %d\n", i);
        HalfEdge::test(i, (float)i/10);
    }
    return 0;

    Cube cube;
    int width = 1080, height = 720;

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        throw runtime_error("Failed to initialize GLFW");
    }


    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window;
    window = glfwCreateWindow(width, height, "Oddity", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        printf("Failed to create Window\n");
        throw runtime_error("Failed to create Window");
    }

    glfwMakeContextCurrent(window);

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    Player player;

    glfwSetWindowUserPointer(window, &player);

    auto keyfunc = [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        static_cast<Player *>((glfwGetWindowUserPointer(window)))->key_callback(window, key, scancode, action, mods);
    };

    glfwSetKeyCallback(window, keyfunc);

    auto cursorfunc = [](GLFWwindow *window, double posx, double posy) {
        static_cast<Player *>((glfwGetWindowUserPointer(window)))->cursor_callback(window, posx, posy);
    };

    glfwSetCursorPosCallback(window, cursorfunc);

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW");
        throw runtime_error("Failed to initialize GLEW");
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = true;
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    /// Shaders
    GLuint program = loadFileShaders("shaders/colorvert.shader", "shaders/colorfrag.shader");

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, cube.vertices.size() * sizeof(float), cube.vertices.data(), GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    createColor(cube.points.size() * 3 * 3 * 3, colorbuffer);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glUseProgram(program);

        /// Perspective
        player.move();

        mat4 projection = perspective(radians(player.fov), float(width) / float(height), 0.1f, 100.0f);

        mat4 view = lookAt(player.position, player.position + player.direction(), player.up());

        mat4 model = mat4(1.0f);

        mat4 mvp = projection * view * model;

        GLuint matrix = glGetUniformLocation(program, "MVP");

        glUniformMatrix4fv(matrix, 1, GL_FALSE, &mvp[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        createColor(cube.points.size() * 3 * 3 * 3, colorbuffer);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glDrawArrays(GL_TRIANGLES, 0, cube.points.size() * 3 * 3);
        glDisableVertexAttribArray(0);

        ImGui::Begin("Hallo", nullptr, 0 | (ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration));
        ImGui::Text("Player:\nPos:\t[ %1.2f | %1.2f | %1.2f ]\nDir:\t[ %1.2f | %1.2f | %1.2f ]\nAng:\t[ %1.2f | %1.2f ]\n", player.position.x, player.position.y, player.position.z, player.direction().x, player.direction().y, player.direction().z, player.angle.x, player.angle.y);

        for (size_t i = 0; i < Debug::get_points().size(); i++) {
            auto ray = Debug::get_points()[i] - player.position;
            ray = ray * player.direction();
            auto raylength = length(ray);
            auto shortray = normalize(ray);
            auto point = project(Debug::get_points()[i], view, projection, vec4(0, 0, width, height));
            auto point2d = vec2(point.x, height - point.y);

            if ((ray.x + ray.y + ray.z) < 0) {
                continue;
            }

            ImGui::SetNextWindowPos(ImVec2(point2d.x, point2d.y));
            ImGui::SetNextWindowSize(ImVec2(400, 64));
            ImGui::Begin(to_string(i).data(), nullptr, (ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration));

            ImGui::Text("%s", Debug::get_point_texts()[i].data());
            ImGui::End();
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}