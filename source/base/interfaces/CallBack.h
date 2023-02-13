//
// Created by jan on 10/02/2023.
//

#ifndef ODDITYENGINE_CALLBACK_H
#define ODDITYENGINE_CALLBACK_H


class CallBack {
public:
    virtual void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) = 0;
    virtual void cursor_callback(GLFWwindow *window, double xpos, double ypos) = 0;
    virtual void mouse_callback(GLFWwindow *window, int button, int action, int mods) = 0;
    virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) = 0;
};


#endif //ODDITYENGINE_CALLBACK_H
