#ifndef DEBUG_H
#define DEBUG_H

#include <string>
#include <functional>
#include <stdexcept>
#include <fmt/core.h>

namespace OddityEngine {
    class Debug {
    public:
        Debug() = delete;

        static size_t message_count;

        template<typename ... T>
        static void message(const std::string& fmt, T&& ... args) {
            fmt::print("{:4d}\t", message_count++);
            fmt::vprint(fmt, fmt::make_format_args(args...));
            fmt::print("\n");
        }

        template<typename ... T>
        static void error(const std::string& fmt, T&& ... args) {
            Debug::message(fmt, args...);
            throw std::runtime_error(fmt::vformat(fmt, fmt::make_format_args(args...)));
        }

        static void update();
        static void add_value(std::function<void()> value);
    };
}



#endif //DEBUG_H
