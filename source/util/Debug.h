#ifndef ODDITYENGINE_DEBUG_H
#define ODDITYENGINE_DEBUG_H

#include "glm/glm.hpp"

#include "fmt/core.h"

#include <string>
#include <vector>
#include <functional>

#include "Value.h"

namespace OddityEngine::Debug {
    typedef std::pair<glm::vec3*, std::string*> point;

    void add_point(glm::vec3* pos, std::string* text);
    std::vector<point> get_points();
    point get_point(size_t index);



    void _add_text(fmt::string_view format, fmt::format_args args);
    std::string get_text();

    size_t add_value_interface(ValueInterface* value_interface);

    template<typename... T>
    size_t add_value(std::string format, T... value) {
        return add_value_interface(new Value(format, value...));
    }

    void terminate();
}


#endif //ODDITYENGINE_DEBUG_H
