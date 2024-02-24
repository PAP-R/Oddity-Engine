#include "OddityEngine.h"

#include <steam_api.h>

#include <Graphics/Graphics.h>
#include <Util/Event_Poller.h>

#include <Util/Debug.h>
#include "Util/Time.h"

namespace OddityEngine {
    void init() {
        // if(SteamAPI_RestartAppIfNecessary(360)) {
        //     exit(0);
        // }
        // if(!SteamAPI_Init()) {
        //     Debug::error("Failed to initialize Steam API");
        // }
        Graphics::init();
    }

    void terminate() {
        Graphics::terminate();
        // SteamAPI_Shutdown();
    }

    bool update() {
        Util::Time::update();
        Event::update();
        return Graphics::update();
    }

}
