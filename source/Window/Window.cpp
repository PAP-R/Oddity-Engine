#include "Window.h"

#include <Util/Debug.h>
#include <vector>

namespace OddityEngine {
    std::vector<Window*> windowList;

    auto create_window(const char* name, const int width, const int height, const unsigned int flags) {
        const auto window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
        Debug::assert_error(window == nullptr, "Failed to create window");

        return window;
    }

    Window::Window(const char *name, const int width, const int height, const unsigned int flags) : _window(create_window(name, width, height, flags)), _size(width, height) {
        windowList.emplace_back(this);
    }

    Window::~Window() {
        SDL_DestroyWindow(_window);
        std::erase(windowList, this);
    }

    void Window::update() {

    }

    void Window::set_size(glm::ivec2 size) {
        _size = size;
    }

    bool Window::update_all() {
        for (const auto w : windowList) {
            w->update();
        }

        return !windowList.empty();
    }

    void Window::terminat_all() {
        for (const auto copy = windowList; const auto w : copy) {
            delete(w);
        }
    }

    Window* Window::get(SDL_Window *window) {
        const auto windowIterator = std::ranges::find_if(windowList, [&window](Window* w){return window == w->_window;});

        if (windowIterator == windowList.end()) {
            return nullptr;
        }

        return *windowIterator;
    }

    Window* Window::get(const Uint32 windowID) {
        return get(SDL_GetWindowFromID(windowID));
    }
}
