#ifndef DEBUG_H
#define DEBUG_H

#include <string>
#include <functional>

namespace OddityEngine::Debug {
    void message(const std::string& message);
    void error(const std::string& message);

    void update();
    void add_value(std::function<void()> value);
};



#endif //DEBUG_H
