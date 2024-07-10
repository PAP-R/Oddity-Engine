#include "Updateable.h"

#include <set>

namespace OddityEngine {
	std::set<Updateable*> updateableSet;
	std::set<Updateable*> activeSet;

	Updateable::Updateable(bool global) {
		init(global);
	}

	Updateable::~Updateable() {
		terminate();
	}

	void Updateable::init(bool global) {
		updateableSet.emplace(this);
		if (global) {
			activeSet.emplace(this);
		}
	}

	void Updateable::terminate() {
		activeSet.erase(this);
		updateableSet.erase(this);
	}

	void Updateable::_update() {
		update();
		for (const auto u : _subdateables) {
			u->_update();
		}
	}

	void Updateable::set_active(const bool active) {
		if (active) {
			activeSet.emplace(this);
		}
		else {
			activeSet.erase(this);
		}
	}

	bool Updateable::update_all() {
		for (auto u = activeSet.begin(); u != activeSet.end();) {
			(*u++)->_update();
		}

		return !activeSet.empty();
	}
}
