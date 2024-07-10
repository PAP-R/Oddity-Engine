#include "Window.h"

#include <Util/Debug.h>
#include <set>

namespace OddityEngine {
	std::set<Window *> windowSet;

	/**
     * Creates a simple SDL based window
     * @param name The windows title
     * @param width the width of the window
     * @param height the height of the window
     * @param flags SDL_WINDOW flags
     * @return the window
     */
	auto create_window(const char *name, const int width, const int height, const unsigned int flags) {
		const auto window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		Debug::assert_error(window == nullptr, "Failed to create window");

		return window;
	}

	Window::Window(const char *name, const int width, const int height, const unsigned int flags) {
		init(name, width, height, flags);
	}

	Window::~Window() {
		terminate();
	}

	void Window::init(const char *name, int width, int height, unsigned int flags) {
		if (_window == nullptr) {
			_window = create_window(name, width, height, flags);
			_windowID = SDL_GetWindowID(_window);
			_size = {width, height};
		}

		Event::init();
		Updateable::init(true);

		windowSet.emplace(this);
	}

	void Window::terminate() {
		if (_window != nullptr) {
			SDL_DestroyWindow(_window);
		}

		Event::terminate();
		Updateable::terminate();

		windowSet.erase(this);
	}

	void Window::update() {
	}

	void Window::event(SDL_Event &event) {
		if (event.type == SDL_WINDOWEVENT && event.window.windowID == _windowID) {
			switch (event.window.event) {
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					set_size({event.window.data1, event.window.data2});
					Debug::message(fmt::format("Window {} resized to [{} / {}]", event.window.windowID, event.window.data1, event.window.data2));
					break;
				case SDL_WINDOWEVENT_RESTORED:
					set_size(get_size());
					Debug::message(fmt::format("Window {} restored to [{} / {}]", event.window.windowID, _size.x, _size.y));
					break;
				case SDL_WINDOWEVENT_MINIMIZED:
					set_size({0, 0});
					Debug::message(fmt::format("Window {} minimized", event.window.windowID));
				break;
				case SDL_WINDOWEVENT_CLOSE:
					terminate();
					Debug::message(fmt::format("Window {} closed", event.window.windowID));
				break;
			}
		}
	}

	void Window::set_size(glm::ivec2 size) {
		_size = size;
	}

	glm::ivec2 Window::get_size() {
		SDL_GetWindowSize(this->_window, &_size.x, &_size.y);
		return _size;
	}

	SDL_Window* Window::get_window() {
		return _window;
	}

	void Window::set_fullscreen() const {
		Debug::assert_error(SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN) != 0, "Fullscreen failed");
	}

	void Window::set_windowed_fullscreen() const {
		Debug::assert_error(SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0, "Windowed Fullscreen failed");
	}

	void Window::set_windowed() const {
		Debug::assert_error(SDL_SetWindowFullscreen(_window, 0) != 0, "Windowed failed");
	}

	size_t Window::count() {
		return windowSet.size();
	}

	void Window::terminat_all() {
		for (const auto copy = windowSet; const auto w: copy) {
			delete(w);
		}
	}

	Window *Window::get(SDL_Window *window) {
		const auto windowIterator = std::find_if(windowSet.begin(), windowSet.end(), [&window](Window *w) { return window == w->_window; });

		if (windowIterator == windowSet.end()) {
			return nullptr;
		}

		return *windowIterator;
	}

	Window *Window::get(const Uint32 windowID) {
		return get(SDL_GetWindowFromID(windowID));
	}
}
