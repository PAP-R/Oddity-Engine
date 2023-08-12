#include "OddityEngine.h"

#include <fmt/core.h>

int main() {
    OddityEngine::init();
    OddityEngine::Graphics::create_window("Oddity", 500, 500);

    while (OddityEngine::update()) {

    }

    OddityEngine::terminate();
}