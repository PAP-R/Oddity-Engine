#ifndef ODDITYENGINE_DEBUG_H
#define ODDITYENGINE_DEBUG_H

#include <string>

namespace OddityEngine {
    namespace Debug {
        void update();

        void message(const std::string& message);
        void error(const std::string& message);
    } // OddityEngine
} // Debug

#endif //ODDITYENGINE_DEBUG_H
