#include "Window.h"

#include <vector>
#include <algorithm>

#include <Util/Debug.h>

namespace OddityEngine::Graphics {
    std::vector<Window*> window_list;

    auto create_window(const char* name, const int width, const int height, const unsigned int flags) {
        auto window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
        if (window == nullptr) {
            Debug::error("Failed to create window");
        }

        return window;
    }

    Window::Window(const char* name, const int width, const int height, const unsigned int flags) : window(create_window(name, width, height, flags)), size(width, height) {
        window_list.emplace_back(this);
    }

    Window::~Window() {
        SDL_DestroyWindow(window);
    }

    void Window::set_size(glm::vec2 size) {
        this->size = size;
    }

    bool Window::update_all() {
        for (auto w : window_list) {
            w->update();
        }

        return !window_list.empty();
    }

    void Window::terminate_all() {
        for (auto w : window_list) {
            delete(w);
        }
        window_list.clear();
    }

    Window* Window::get(SDL_Window* window) {
        const auto window_iterator = std::ranges::find_if(window_list, [&window](Window* w){return window == w->window;});
        if (window_iterator == window_list.end()) {
            return nullptr;
        }
        return *window_iterator;
    }

    void Window::event(const SDL_Event& event) {
        const auto window = get(SDL_GetWindowFromID(event.window.windowID));
        if (window != nullptr) {
            if (event.type == SDL_WINDOWEVENT) {
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        window->set_size({event.window.data1, event.window.data2});
                        Debug::message(fmt::format("Window {} resized to [{} / {}]", event.window.windowID, event.window.data1, event.window.data2));
                        break;
                    case SDL_WINDOWEVENT_CLOSE:
                        window_list.erase(std::remove(window_list.begin(), window_list.end(), window), window_list.end());
                        window->~Window();
                        Debug::message(fmt::format("Window {} closed", event.window.windowID));
                        break;
                }
            }
        }
    }
}