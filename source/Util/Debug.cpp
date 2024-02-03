#include "Debug.h"

#include <stdexcept>
#include <fmt/core.h>

namespace OddityEngine::Debug {
    size_t message_count = 0;
    std::vector<std::function<void()>> values;

    void message(const std::string& message) {
        fmt::print("{:4d} \t : {}\n", message_count++, message);
    }

    void error(const std::string& message) {
        Debug::message(message);
        throw std::runtime_error(message);
    }

    void update() {
        for (const auto& v : values) {
            v();
        }
    }

    void add_value(std::function<void()> value) {
        values.emplace_back(value);
    }

}