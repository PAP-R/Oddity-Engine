#include "Event.h"

#include <set>

namespace OddityEngine {
	std::set<Event*> eventSet;
	std::set<Event*> activeSet;

	Event::Event() {
		init();
	}

	Event::~Event() {
		terminate();
	}

	void Event::set_active(bool active) {
		if (active) {
			activeSet.emplace(this);
		}
		else {
			activeSet.erase(this);
		}
	}

	void Event::init() {
		eventSet.emplace(this);
		activeSet.emplace(this);
	}

	void Event::terminate() {
		activeSet.erase(this);
		eventSet.erase(this);
	}

	void Event::update() {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			for (auto e = activeSet.begin(); e != activeSet.end();) {
				(*e++)->event(event);
			}
		}
	}
}
