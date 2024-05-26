#ifndef ODDITYENGINE_WINDOW_OPENGL_H
#define ODDITYENGINE_WINDOW_OPENGL_H

#include <SDL.h>
#include "imgui.h"

#include <Graphics/Window.h>

#include "Graphics/OpenGL/Buffer/Buffer.h"

#include "Graphics/OpenGL/Render/Scene.h"
#include "Graphics/OpenGL/Shader/Program.h"
#include "Graphics/OpenGL/Shader/Shader.h"

namespace OddityEngine::Graphics::OpenGL {
    class Window : public Graphics::Window {
    protected:
        SDL_Window* window = nullptr;
        SDL_Surface* surface = nullptr;

        SDL_GLContext gl_context;

        ImGuiContext* imgui_context = nullptr;

        GLuint vertex_array;

        Buffer<GLfloat> screenbuffer;

        Program view_program;

        Scene* scene = nullptr;

    public:
        Window(const char* name, int width, int height, unsigned int flags);
        ~Window();

        void make_current() override;
        void update() override;

        void set_scene(Scene* scene);
    };
}

#endif //ODDITYENGINE_WINDOW_OPENGL_H
