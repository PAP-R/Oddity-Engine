#include <cstdio>
#include <cstdlib>
#include <stdexcept>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <vector>
#include <algorithm>
#include <iostream>
#include <map>

#include "Player.h"
#include "common/loadShader.h"
#include "common/loadTexture.h"

static GLfloat g_color_buffer_data[12 * 3 * 3];

double lastTime = 0.0;

 std::vector<vec3> cubeCorners() {
    size_t cornerCount = 8;
    std::vector<vec3> cornerData;

    for (int i = 0; i < cornerCount; i++) {
        cornerData.emplace_back(((i / 4) % 2) * 2 - 1, ((i / 2) % 2) * 2 - 1, ((i) % 2) * 2 - 1);
        printf("[%3f / %3f / %3f]\n", cornerData.back().x, cornerData.back().y, cornerData.back().z);
    }

    return cornerData;
}

bool compair(std::pair<int, float> first, std::pair<int, float> second) {
     return first.second < second.second;
 }

std::vector<float> createTriangles(std::vector<vec3> points) {
     std::vector<std::vector<std::pair<int, float>>> lengths(points.size());

     for (int v = 0; v < points.size(); v++) {
         for (int o = 0; o < points.size(); o++) {
             int offset = (v + o) % points.size();
             lengths[v].emplace_back(offset, distance(points[v], points[offset]));
         }
         std::sort(lengths[v].begin(), lengths[v].end(), compair);
         for (auto d : lengths[v]) {
             printf("%d -> %d : %f\n", v, d.first, d.second);
         }
     }

     std::vector<float> verts;

     for (size_t v = 0; v < points.size(); v++) {
         for(int o = 0; o < 3; o++) {
            for (int i = 0; i < 3; i++) {
                verts.emplace_back(points[lengths[v][o].first][i]);
                printf("%f\t", verts.back());
            }
            printf("\n");
         }
     }


     return verts;
 }

std::vector<GLfloat> createCube() {
    return createTriangles(cubeCorners());
}

void createColor(GLuint colorbuffer) {
    double currentTime = glfwGetTime();
    auto deltaTime = float(currentTime - lastTime);
    lastTime = currentTime;

    for (float & v : g_color_buffer_data) {
        v += (((float)rand() / RAND_MAX) * 2 - 1) * deltaTime;
        v = v < 0.0f ? 0.0f : v > 1.0f ? 1.0f : v;
    }

    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
}

int main() {
    createTriangles(cubeCorners());
    int width = 1080, height = 720;

    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    window = glfwCreateWindow(width, height, "Oddity", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        printf("Failed to create Window\n");
        throw std::runtime_error("Failed to create Window");
    }

    Player player;

    printf("HorizontalAngle: %f\nVerticalAngle: %f\n", player.angle.x, player.angle.y);

    glfwSetWindowUserPointer(window, &player);

    auto keyfunc = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        static_cast<Player*>((glfwGetWindowUserPointer(window)))->key_callback(window, key, scancode, action, mods);
    };

    glfwSetKeyCallback(window, keyfunc);

    auto cursorfunc = [](GLFWwindow* window, double posx, double posy) {
        static_cast<Player*>((glfwGetWindowUserPointer(window)))->cursor_callback(window, posx, posy);
    };

    glfwSetCursorPosCallback(window, cursorfunc);

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW");
       throw std::runtime_error("Failed to initialize GLEW");
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

    /// Perspective


    /// Test Cube
    static const GLfloat g_vertex_buffer_data[] = {
            -1.0f,-1.0f,-1.0f, // triangle 1 : begin
            -1.0f,-1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f, // triangle 1 : end
            1.0f, 1.0f,-1.0f, // triangle 2 : begin
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f, // triangle 2 : end
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), createCube().data(), GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    createColor(colorbuffer);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);

        /// Perspective
        player.move();

        mat4 projection = perspective(radians(player.fov), float(width) / float(height), 0.1f, 100.0f);

        mat4 view = lookAt(
                player.position,
                player.position + player.direction(),
                player.up()
        );

        mat4 model = mat4(1.0f);

        mat4 mvp = projection * view * model;

        GLuint matrix = glGetUniformLocation(program, "MVP");

        glUniformMatrix4fv(matrix, 1, GL_FALSE, &mvp[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                0,
                3,
                GL_FLOAT,
                GL_FALSE,
                0,
                nullptr
        );

        createColor(colorbuffer);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
                1,
                3,
                GL_FLOAT,
                GL_FALSE,
                0,
                nullptr
        );

        glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}