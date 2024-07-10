#include <fmt/core.h>

#include <steam_api.h>

#include <OddityEngine.h>
using namespace OddityEngine;

#include <Util/Debug.h>
#include <Util/Time.h>

#include <Window/Window.h>

#include <Util/Trie.h>

long double runtime = 0;

void print_time() {
    Debug::message("Time[{}]: {} | {}", Time::frame(), Time::runtime(), runtime);
}

int main(int argc, char* argv[]) {
    print_time();

    OddityEngine::init();

    auto window = Window("Hallo", 100, 100, SDL_WINDOW_RESIZABLE);
    // auto window2 = Window("Hallo2", 100, 100, SDL_WINDOW_RESIZABLE);
    // window.set_windowed_fullscreen();

    while (OddityEngine::update()) {
        runtime += Time::delta();
    }

    OddityEngine::terminate();

    print_time();

    Debug::print("\nDebug Log:\n{}", Debug::messageLog);

    return 0;
}