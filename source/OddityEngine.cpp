#include "OddityEngine.h"

#include <Graphics/Graphics.h>
#include <Util/Event_Poller.h>

#include "Util/Time.h"

namespace OddityEngine {
    void init() {
        Graphics::init();
    }

    void terminate() {
        Graphics::terminate();
    }

    bool update() {
        Util::Time::update();
        Event::update();
        return Graphics::update();
    }

}
