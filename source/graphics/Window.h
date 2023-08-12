#ifndef ODDITYENGINE_WINDOW_H
#define ODDITYENGINE_WINDOW_H

#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "imgui_internal.h"

using namespace glm;

#include <vector>

namespace OddityEngine::Graphics {
    class Window {
    private:
        GLFWwindow* window;
        ImGuiContext* context;
        std::vector<Window*>* list;

    public:
        Window(std::vector<Window*>* list, const char * name, int width = 400, int height = 400);

        ~Window();

        void update();

        vec<2, size_t> get_pos();
        vec<2, size_t> get_size();

        GLFWwindow* get_window();
        ImGuiContext* get_context();
    };

}


#endif //ODDITYENGINE_WINDOW_H
