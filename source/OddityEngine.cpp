#include "OddityEngine.h"

#include <steam_api.h>

#include <Graphics/Graphics.h>
#include <Util/Event_Poller.h>

#include <Util/Debug.h>
#include "Util/Time.h"

#define ODDITYENGINE_STEAM true

namespace OddityEngine {
    bool steam = true;

    void init() {
        if constexpr (ODDITYENGINE_STEAM) {
            if(SteamAPI_RestartAppIfNecessary(360)) {
                exit(0);
            }

            if(!SteamAPI_Init()) {
                Debug::message("Failed to initialize Steam API");
                steam = false;
            }
        }


        Graphics::init();
    }

    void terminate() {
        Graphics::terminate();
        if (steam) {
            SteamAPI_Shutdown();
        }
    }

    bool update() {
        Util::Time::update();
        Event::update();
        return Graphics::update();
    }

}
