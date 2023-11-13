#ifndef ODDITYENGINE_DEBUG_H
#define ODDITYENGINE_DEBUG_H

#include <string>

#include "Value.h"

namespace OddityEngine {
    namespace Debug {
        void update();

        void message(const std::string& message);
        void error(const std::string& message);

        size_t add_value_interface(Util::ValueInterface* value_interface);

        template<typename T>
        size_t add_value(T value) {
            return add_value_interface(new Util::Value(value));
        }
    } // OddityEngine
} // Debug

#endif //ODDITYENGINE_DEBUG_H
