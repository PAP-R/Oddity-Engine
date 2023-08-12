#ifndef ODDITYENGINE_DEBUG_H
#define ODDITYENGINE_DEBUG_H

#include "glm/glm.hpp"

#include "fmt/core.h"

#include <string>
#include <vector>

namespace OddityEngine::Debug {
    typedef std::pair<glm::vec3*, std::string*> point;

    void add_point(glm::vec3* pos, std::string* text);
    std::vector<point> get_points();
    point get_point(size_t index);

    void _add_text(fmt::string_view format, fmt::format_args args);
    std::vector<std::string> get_texts();
    std::string get_text(size_t index);



    template<typename... T>
    void add_text(fmt::format_string<T...> format, T&&... args) {
        _add_text(format, fmt::make_format_args(args...));
    }
}


#endif //ODDITYENGINE_DEBUG_H
