#include "OddityEngine.h"

#ifdef ODDITY_ENGINE_STEAM
#include <cstdlib>
#include <iostream>
#include <steam_api.h>
#endif

#include <Util/Debug.h>
#include <Util/Time.h>

#include <Window/Event.h>
#include <Util/Updateable.h>

namespace OddityEngine {
    bool initilized = false;
    bool steam = false;

    void init() {
        if (initilized) return;
#ifdef ODDITY_ENGINE_STEAM

        if(SteamAPI_RestartAppIfNecessary(360)) {
            std::exit(0);
        }

        if(!SteamAPI_Init()) {
            Debug::error("Failed to initialize Steam API");
        }
        else {
            steam = true;
            Debug::message("Steam initilized as Player: {}", SteamFriends()->GetPersonaName());
        }
#endif

        Debug::assert_error(SDL_Init(SDL_INIT_EVERYTHING) < 0, "Failed to initialize SDL");
        initilized = true;

        Debug::message("Engine initilized");
    }

    void terminate() {
        if (!initilized) return;
        SDL_Quit();

#ifdef ODDITY_ENGINE_STEAM
        if (steam) {
            SteamAPI_Shutdown();
            Debug::message("Steam terminated");
        }
#endif
        initilized = false;
        Debug::message("Engine Terminated");
    }

    bool update() {
        if (!initilized) return false;
        Time::update();
        Event::update();
        return Updateable::update_all();
    }
}