#include "OddityEngine.h"

#include "graphics/Graphics.h"
#include "util/Time.h"
#include "util/Debug.h"

namespace OddityEngine {
    namespace Stat{
        float fps;
        double spf;
        auto slower = OddityEngine::Time::get_time_delta();
        auto slower_threshold = 0.1;

        std::vector<float> frame_times;

        void update() {
            spf = OddityEngine::Time::get_time_delta();
            Stat::slower += OddityEngine::Time::get_time_delta();
            float quick_fps = 1 / OddityEngine::Time::get_time_delta();
            frame_times.emplace_back(quick_fps);

            if (Stat::slower > Stat::slower_threshold) {
                Stat::slower -= Stat::slower_threshold;
                Stat::fps = quick_fps;
            }
        }
    }

    void init() {
        Graphics::init();
        Debug::add_value([&](){ImGui::Text("%s", fmt::format("FPS: {:6.0f} = 1/{:6.6f}", Stat::fps, Stat::spf).c_str());});
        Debug::add_value([&](){ImGui::PlotLines("Frame Times", Stat::frame_times.data(), Stat::frame_times.size());});
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
