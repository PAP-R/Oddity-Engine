#ifndef ODDITYENGINE_VALUE_H
#define ODDITYENGINE_VALUE_H

#include <string>
#include <stdexcept>
#include <iostream>
#include <utility>

#include "fmt/core.h"
#include <fmt/chrono.h>
#include <functional>
#include <future>

namespace OddityEngine {
    class ValueInterface {
    public:
        bool show = true;
        virtual void apply() = 0;

        virtual ~ValueInterface() = default;
    };

    template<typename T>
    class Value : public ValueInterface{
        T element;
    public:
        Value(T element) : element(element) {}

        void apply() override {
            element();
        }
    };

//    class ValueInterface {
//    public:
//        std::string format;
//        bool show = true;
//
//        std::string result;
//
//        ValueInterface(std::string name) : format(std::move(name)) {}
//        virtual ~ValueInterface(){}
//
//        virtual std::string apply() = 0;
//    };
//
//    template<typename E, typename V>
//    class Value : public ValueInterface {
//    private:
//        E element;
//        V values;
//
//    public:
//        Value(std::string format, V values) : ValueInterface(format), values(values) {}
//
//        virtual std::string apply() {
//            return std::apply([&](auto&&... args){return fmt::vformat(this->format, fmt::make_format_args(*args...));}, this->values);
//        }
//    };
}

#endif //ODDITYENGINE_VALUE_H
