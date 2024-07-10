#include <fmt/core.h>

#include <steam_api.h>

#include <OddityEngine.h>
using namespace OddityEngine;

#include <Util/Debug.h>
#include <Util/Time.h>

#include <Window/Window.h>

#include <Util/Trie.h>

#include <Util/Syncable.h>

long double runtime = 0;

void print_time() {
    Debug::message("Time[{}]: {} | {}", Time::frame(), Time::runtime(), runtime);
}

class ErrorCallback {
    STEAM_CALLBACK(ErrorCallback, OnGameOverlayActivated, GameOverlayActivated_t);
};

void ErrorCallback::OnGameOverlayActivated(GameOverlayActivated_t* pCallback) {
    if (pCallback->m_bActive) {
        Debug::message("Steam overlay opened");
    }
    else {
        Debug::message("Steam overlay closed");
    }
}

int main(int argc, char* argv[]) {
    print_time();

    OddityEngine::init();

    auto window = Window("Hallo", 100, 100, SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN);
    // auto window2 = Window("Hallo2", 100, 100, SDL_WINDOW_RESIZABLE);
    // window.set_windowed_fullscreen();

    // SteamNetworkingMessages()->SendMessageToUser()

    auto networking = Networking();
    networking.create_lobby();
    networking.get_lobby_list();

    while (OddityEngine::update()) {
        runtime += Time::delta();
    }

    OddityEngine::terminate();

    print_time();

    Debug::print("\nDebug Log:\n{}", Debug::messageLog);

    return 0;
}