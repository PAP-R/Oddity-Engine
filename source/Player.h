#ifndef ODDITYENGINE_PLAYER_H
#define ODDITYENGINE_PLAYER_H

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

class Player {
    double time = glfwGetTime();
    float deltaTime = glfwGetTime();

    vec3 movement = vec3(0);

public:
    vec3 position = vec3(2);
    vec2 angle = vec2(3.14f, 0.0f);

    float fov = 45.0f;

    float speed = 3.0f;
    float mouseSpeed = 0.005f;

    vec3 direction() const;
    vec3 right() const;
    vec3 up() const;
    void move();

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void cursor_callback(GLFWwindow* window, double xpos, double ypos);
};


#endif //ODDITYENGINE_PLAYER_H
