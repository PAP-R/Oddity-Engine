#include "OddityEngine.h"

#include <fmt/core.h>

int main() {
    OddityEngine::init();
    OddityEngine::Graphics::create_window("Oddity", 500, 500);

    std::string nummer = "Nummer";

    OddityEngine::Debug::add_text("Dies ist eine {} {} {} {}", 3, nummer, "Nummer", 7);

    for (auto text : OddityEngine::Debug::get_texts()) {
        printf(text.c_str());
    }

    OddityEngine::terminate();
}