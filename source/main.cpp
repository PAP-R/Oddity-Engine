#include <fmt/core.h>

#include <steam_api.h>

#include <OddityEngine.h>
using namespace OddityEngine;

#include <util/Debug.h>
#include <Util/Time.h>

#include <Window/Window.h>

#include <Util/Trie.h>

void print_time() {
    Debug::message("Time[{}]: {}", Time::frame(), Time::runtime());
}

int main(int argc, char* argv[]) {
    print_time();

    OddityEngine::init();

    Trie<int> trie;

    trie.add("Hallo", 2);

    trie.add("CA", trie["Hallo"].front());

    trie.add("Ciao", trie('C', 'A').front());

    Debug::message("Trie test : ", trie('C', 'i', 'a', 'o').front());

    for (auto t : trie) {
        Debug::message("Trie: {} : {}", t.first, t.second.front());
    }

    auto window = Window("Hallo", 100, 100, SDL_WINDOW_RESIZABLE);

    while (OddityEngine::update()) {

    }

    OddityEngine::terminate();

    print_time();

    Debug::print("\nDebug Log:\n{}", Debug::messageLog);

    return 0;
}