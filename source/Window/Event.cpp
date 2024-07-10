#include "Event.h"

#include <set>

namespace OddityEngine {
	std::set<Event*> eventSet;
	std::set<Event*> eventSetActive;

	Event::Event() {
		init();
	}

	Event::~Event() {
		terminate();
	}

	void Event::set_active(bool active) {
		if (active) {
			eventSetActive.emplace(this);
		}
		else {
			eventSetActive.erase(this);
		}
	}

	void Event::init() {
		eventSet.emplace(this);
		eventSetActive.emplace(this);
	}

	void Event::terminate() {
		eventSetActive.erase(this);
		eventSet.erase(this);
	}

	void Event::update() {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			for (auto e = eventSetActive.begin(); e != eventSetActive.end();) {
				(*e++)->event(event);
			}
		}
	}
}
