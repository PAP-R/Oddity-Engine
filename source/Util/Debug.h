#ifndef DEBUG_H
#define DEBUG_H

#include <string>
#include <fmt/core.h>
#include <iostream>
#include <stdexcept>

class Debug {
public:
    Debug() = delete;

    static std::string messageLog;
    static size_t messageCount;

    /**
     * Formats a string with fmt
     * @tparam T type of the elemnts to insert
     * @param fmt format string
     * @param args elements to insert
     * @return formated string
     */
    template<typename ... T>
    static auto format(const std::string& fmt, T&& ... args) {
        return vformat(fmt, fmt::make_format_args(args...));
    }

    /**
     * Prints formatted text
     * @tparam T type of the elemnts to insert
     * @param fmt format string
     * @param args elements to insert
     */
    template<typename ... T>
    static void print(const std::string& fmt, T&& ... args) {
        auto text = format(fmt, args...);
        std::cout << text;
        messageLog += text;
    }

    /**
     * Prints formatted text with some debug formatting around it
     * @tparam T type of the elemnts to insert
     * @param fmt format string
     * @param args elements to insert
     */
    template<typename ... T>
    static void message(const std::string& fmt, T&& ... args) {
        print("{:>4}\t" + fmt + "\n", messageCount++, args...);
    }

    /**
     * Prints formatted text with some debug formatting around it and throws a runtime error
     * @tparam T type of the elemnts to insert
     * @param fmt format string
     * @param args elements to insert
     */
    template<typename ... T>
    static void error(const std::string& fmt, T&& ... args) {
        message(fmt, args...);
        throw std::runtime_error(format(fmt, args...));
    }

    /**
     * Prints formatted text with some debug formatting around it and throws a runtime error
     * @tparam T type of the elemnts to insert
     * @param condition
     * @param fmt format string
     * @param args elements to insert
     */
    template<typename ... T>
    static void assert_error(const bool condition, const std::string& fmt, T&& ... args) {
        if (condition) {
            Debug::error(fmt, args...);
        }
    }
};



#endif //DEBUG_H
