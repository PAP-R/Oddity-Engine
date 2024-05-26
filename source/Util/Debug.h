#ifndef DEBUG_H
#define DEBUG_H

#include <string>
#include <functional>
#include <stdexcept>
#include <fmt/core.h>

#include <locale>

namespace OddityEngine {
    class Debug {
    public:
        Debug() = delete;

        static size_t message_count;


        template<typename ... T>
        static void print(const std::string& fmt, T&& ... args) {
            fmt::vprint(fmt, fmt::make_format_args(args...));
        }

        template<typename ... T>
        static void message(const std::string& fmt, T&& ... args) {
            print("{:>4}\t", message_count++);
            print(fmt, args...);
            print("\n");
        }

        template<typename ... T>
        static void error(const std::string& fmt, T&& ... args) {
            Debug::message(fmt, args...);
            throw std::runtime_error(fmt::vformat(fmt, fmt::make_format_args(args...)));
        }

        template<typename ... T>
        static void assert_error(bool condition, const std::string& fmt, T&& ... args) {
            if (condition) {
                Debug::error(fmt, args...);
            }
        }

        static void update();
        static void add_value(std::function<void()> value);
    };
}



#endif //DEBUG_H
