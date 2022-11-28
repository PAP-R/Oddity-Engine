#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "Player.h"
#include "common/loadShader.h"
#include "common/loadTexture.h"

static GLfloat g_color_buffer_data[12 * 3 * 3];

double lastTime = 0.0;

void createColor(GLuint colorbuffer) {
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);
    lastTime = currentTime;

    for (float & v : g_color_buffer_data) {
        v += (((float)rand() / RAND_MAX) * 2 - 1) * deltaTime;
        v = v < 0.0f ? 0.0f : v > 1.0f ? 1.0f : v;
    }

    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
}

int main() {
    int width = 1080, height = 720;

    glewExperimental = true;
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

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

    glEnable(GL_CULL_FACE);

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

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

     /// Shaders
     GLuint program = loadFileShaders("shaders/testvert.shader", "shaders/testfrag.shader");

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    static const GLfloat g_uv_buffer_data[] = {
            0.000059f, 1.0f-0.000004f,
            0.000103f, 1.0f-0.336048f,
            0.335973f, 1.0f-0.335903f,
            1.000023f, 1.0f-0.000013f,
            0.667979f, 1.0f-0.335851f,
            0.999958f, 1.0f-0.336064f,
            0.667979f, 1.0f-0.335851f,
            0.336024f, 1.0f-0.671877f,
            0.667969f, 1.0f-0.671889f,
            1.000023f, 1.0f-0.000013f,
            0.668104f, 1.0f-0.000013f,
            0.667979f, 1.0f-0.335851f,
            0.000059f, 1.0f-0.000004f,
            0.335973f, 1.0f-0.335903f,
            0.336098f, 1.0f-0.000071f,
            0.667979f, 1.0f-0.335851f,
            0.335973f, 1.0f-0.335903f,
            0.336024f, 1.0f-0.671877f,
            1.000004f, 1.0f-0.671847f,
            0.999958f, 1.0f-0.336064f,
            0.667979f, 1.0f-0.335851f,
            0.668104f, 1.0f-0.000013f,
            0.335973f, 1.0f-0.335903f,
            0.667979f, 1.0f-0.335851f,
            0.335973f, 1.0f-0.335903f,
            0.668104f, 1.0f-0.000013f,
            0.336098f, 1.0f-0.000071f,
            0.000103f, 1.0f-0.336048f,
            0.000004f, 1.0f-0.671870f,
            0.336024f, 1.0f-0.671877f,
            0.000103f, 1.0f-0.336048f,
            0.336024f, 1.0f-0.671877f,
            0.335973f, 1.0f-0.335903f,
            0.667969f, 1.0f-0.671889f,
            1.000004f, 1.0f-0.671847f,
            0.667979f, 1.0f-0.335851f
    };

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    createColor(colorbuffer);

    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

    GLuint texture = loadDDS("textures/uvtemplate.dds");//loadBMP_custom("textures/uvtemplate.bmp");//

    GLuint textureSampler = glGetUniformLocation(program, "myTextureSampler");

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

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glUniform1i(textureSampler, 0);

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

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
                1,
                2,
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