#include "OddityEngine.h"

#include <fmt/core.h>
#include <fmt/chrono.h>
#include <thread>

#include "util/Value.h"

#include "graphics/Tracer.h"

int main() {
    OddityEngine::init();

    auto window = OddityEngine::Graphics::create_window("Oddity", 500, 500);
    auto tracer = OddityEngine::Graphics::create_tracer(window, 1000, 1000);

    do {
    } while (OddityEngine::update());

    OddityEngine::terminate();


    /*
     * Vsync I guess
     * std::this_thread::sleep_for(std::chrono::duration<double>(slower_threshold - slower));
     */
}