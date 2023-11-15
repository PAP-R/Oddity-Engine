#include "OddityEngine.h"
#include "fmt/core.h"

#include <imgui.h>

#include <vector>

#include "Graphics/Graphics.h"
#include <Object/Object.h>
#include <Util/Time.h>
#include <Util/Debug.h>

namespace OddityEngine {
    namespace Stat {
        float fps;
        double spf;
        auto time = glfwGetTime();
        auto last_time = time;

        size_t frame_time_index = 0;
        size_t frame_time_max_index = 100;
        std::vector<float> frame_times(frame_time_max_index);

        void init() {
            Debug::add_value([&](){ImGui::Text("%s", fmt::format("FPS: {:6.0f} = 1 / {:6.6f}", Stat::fps, Stat::spf).c_str() );});
            Debug::add_value([&](){ImGui::PlotLines("Frame Times", Stat::frame_times.data(), Stat::frame_time_max_index, 0, nullptr, 0, 1000);});
        }

        void update() {
            time = glfwGetTime();
            spf = time - last_time;
            fps = 1 / spf;

            frame_times[frame_time_index] = fps;
            frame_time_index = (frame_time_index + 1) % frame_time_max_index;

            last_time = time;
        }
    }

    void init() {
        Stat::init();
        Graphics::init();
    }

    void terminate() {
        Graphics::terminate();
    }

    bool update() {
        Time::update();
        Stat::update();
        Object::update_all();
        return Graphics::update();
    }
} // OddityEngine