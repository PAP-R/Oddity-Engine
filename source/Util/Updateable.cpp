#include "Updateable.h"

#include <set>

namespace OddityEngine {
	std::set<Updateable*> updateableSet;
	std::set<Updateable*> updateableSetActive;

	Updateable::Updateable(const bool global) {
		init(global);
	}

	Updateable::~Updateable() {
		terminate();
	}

	void Updateable::init(const bool global) {
		updateableSet.emplace(this);
		if (global) {
			updateableSetActive.emplace(this);
		}
	}

	void Updateable::terminate() {
		updateableSetActive.erase(this);
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
			updateableSetActive.emplace(this);
		}
		else {
			updateableSetActive.erase(this);
		}
	}

	bool Updateable::update_all() {
		for (auto u = updateableSetActive.begin(); u != updateableSetActive.end();) {
			(*u++)->_update();
		}

		return !updateableSetActive.empty();
	}
}
