#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <glm/glm.hpp>

#include "Event.h"
#include "Util/Updateable.h"

namespace OddityEngine {
	class Window : public Event, public Updateable {
	protected:
		SDL_Window *_window = nullptr;
		Uint32 _windowID = 0;
		SDL_Surface *_surface = nullptr;

		glm::ivec2 _size = {};

	public:
		/**
		 * A simple SDL based window
		 * Is automatically added to an internal window list
		 * @param name title of the window
		 * @param width in pixels
		 * @param height in pixels
		 * @param flags SDL_WINDOW flags
		 */
		Window(const char *name, int width, int height, unsigned int flags = 0);

		/**
		 * Also removes the window from the list
		 */
		~Window() override;

		/**
		 * Initilizes the window
		 * @param name title of the window
		 * @param width in pixels
		 * @param height in pixels
		 * @param flags SDL_WINDOW flags
		 */
		void init(const char *name, int width, int height, unsigned int flags = 0);

		/**
		 * Terminates the window
		 */
		void terminate();

		void update() override;

		void event(SDL_Event& event);

		/**
		 * Sets the windows size
		 * @param size new size
		 */
		void set_size(glm::ivec2 size);

		/**
		 * Gets the windows size
		 * @return current size
		 */
		glm::ivec2 get_size();

		void set_fullscreen() const;

		void set_windowed_fullscreen() const;

		void set_windowed() const;

		/**
		 * Counts all open windows
		 * @return window count
		 */
		static size_t count();

		/**
		 * Terminates all currently open windows
		 */
		static void terminat_all();

		/**
		 * Finds a Window instance by its corresponding SDL_Window
		 * @param window
		 * @return Window* if found otherwise nullptr
		 */
		static Window *get(SDL_Window *window);

		/**
		 * Finds a Window instance by its corresponding SDL windowID
		 * @param windowID
		 * @return
		 */
		static Window *get(Uint32 windowID);
	};
}


#endif //WINDOW_H
