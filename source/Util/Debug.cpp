#include "Debug.h"

#include <imgui.h>

#include <string>
#include <vector>

#include <stdexcept>

#include <fmt/core.h>

#include "Value.h"

namespace OddityEngine {
    namespace Debug {
        size_t message_count = 0;

        std::vector<Util::ValueInterface*> values;

        void apply() {
            for (auto v : values) {
                if (v->show) {
                    v->apply();
                }
            }
        }

        void update() {
            ImGui::Begin("Debug", nullptr, 0 | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
            apply();
            ImGui::End();
        }

        void message(const std::string& message) {
            fmt::print("{:4d} \t: {}\n", message_count++, message);
        }

        void error(const std::string& message) {
            throw std::runtime_error(message);
        }

        size_t add_value_interface(Util::ValueInterface *value_interface) {
            values.emplace_back(value_interface);
            return values.size() - 1;
        }

        void terminate() {
            for (auto v : values) {
                delete(v);
            }
        }
    } // OddityEngine
} // Debug