#ifndef UPDATEABLE_H
#define UPDATEABLE_H

#include <set>

namespace OddityEngine {
	/**
	 * Automatically runs the overloaded update fundction each frame
	 */
	class Updateable {
	protected:
		std::set<Updateable*> _subdateables;

	public:
		explicit Updateable(bool global = true);
		virtual ~Updateable();

		void init(bool global = true);
		void terminate();

		/**
		 * Update wrapper function that is run each frame and also updates the subdatables
		 */
		virtual void _update();

		/**
		 * Update function that is run each frame
		 */
		virtual void update() = 0;

		/**
		 * Set if this element should upate
		 * @param active activation status
		 * @return activation status
		 */
		virtual void set_active(bool active);

		/**
		 * Updates all updateables
		 * @return true while there are updateables
		 */
		static bool update_all();
	};
}

#endif //UPDATEABLE_H
