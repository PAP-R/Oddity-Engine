#ifndef EVENT_H
#define EVENT_H

#include <Util/Trie.h>
#include <functional>
#include <SDL.h>

namespace OddityEngine {
	class Event {
	protected:
		Trie<std::function<void(SDL_Event&)>> _functions;

	public:
		Event();

		virtual ~Event();

		virtual void set_active(bool active);

		void init();
		void terminate();

		virtual void event(SDL_Event& event) = 0;
		static void update();
	};
}

#endif
