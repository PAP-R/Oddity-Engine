#ifndef ODDITYENGINE_VALUE_H
#define ODDITYENGINE_VALUE_H

#include <string>
#include <stdexcept>
#include <iostream>
#include <utility>

#include "fmt/core.h"
#include <fmt/chrono.h>

namespace OddityEngine {

#define SYMBOL '%'

    class ValueInterface {
    public:
        std::string format;
        bool show = true;

        std::string result;

        ValueInterface(std::string name) : format(std::move(name)) {}
        virtual ~ValueInterface(){}

        virtual std::string get_text() = 0;
    };

    template<typename... T>
    class Value : public ValueInterface {
    private:
        std::tuple<T...> values;

    public:
        Value(std::string format, T... values) : ValueInterface(format), values(values...) {}

        virtual std::string get_text() {
            return std::apply([&](auto&&... args){return fmt::vformat(this->format, fmt::make_format_args(*args...));}, this->values);
        }
    };
}

#endif //ODDITYENGINE_VALUE_H
