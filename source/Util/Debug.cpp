#include "Debug.h"

#include <string>

#include <fmt/core.h>

namespace OddityEngine {
    namespace Debug {
        size_t message_count = 0;

        void update() {
            return;
        }

        void message(const std::string& message) {
            fmt::print("{:4d} \t: {}\n", message_count++, message);
        }

        void error(const std::string& message) {
            Debug::message(message);
            throw std::runtime_error(message);
        }
    } // OddityEngine
} // Debug