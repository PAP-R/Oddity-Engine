#include "Debug.h"

#include <stdexcept>
#include <fmt/core.h>

#include <locale>

namespace OddityEngine {
    size_t Debug::message_count = 0;
    std::vector<std::function<void()>> values;
    auto locale = std::locale::global(std::locale("en_IE.UTF-8"));

    void Debug::update() {
        for (const auto& v : values) {
            v();
        }
    }

    void Debug::add_value(std::function<void()> value) {
        values.emplace_back(value);
    }

}