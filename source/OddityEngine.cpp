#include "OddityEngine.h"

#include "graphics/Graphics.h"
#include "util/Time.h"
#include "util/Debug.h"

namespace OddityEngine {
    namespace Stat{
        double fps;
        double spf;
        auto slower = OddityEngine::Time::get_time_delta();
        auto slower_threshold = 0.1;

        void update() {
            spf = OddityEngine::Time::get_time_delta();
            Stat::slower += OddityEngine::Time::get_time_delta();
            if (Stat::slower > Stat::slower_threshold) {
                Stat::slower -= Stat::slower_threshold;
                Stat::fps = 1 / OddityEngine::Time::get_time_delta();
            }
        }
    }
    void init() {
        Graphics::init();
        Debug::add_value([&](){ImGui::Text("%s", fmt::format("FPS: {:6.0f} = 1/{:6.6f}", Stat::fps, Stat::spf).c_str());});
    }

    void terminate() {
        Graphics::terminate();
        Debug::terminate();
    }

    bool update() {
        Time::update();
        Stat::update();
        return Graphics::update();
    }
}
