#include "Debug.h"

#include "fmt/core.h"

#include <sstream>

#include "Value.h"

namespace OddityEngine::Debug {
    std::vector<ValueInterface*> values;
    std::string console_text;


    std::vector<point> points;

    void add_point(glm::vec3 *pos, std::string *text) {

    }

    std::vector<point> get_points() {
        return points;
    }

    point get_point(size_t index) {
        return points[index];
    }

    size_t add_value_interface(ValueInterface* value_interface) {
        values.emplace_back(value_interface);
        return values.size() - 1;
    }

    std::string get_text() {
        std::stringstream result;
        for (auto v : values) {
            if (v->show) {
                result << v->get_text();
                if (result.str().back() != '\n') {
                    result << std::endl;
                }
            }
        }
        return result.str();
    }

    void terminate() {
        for (auto v : values) {
            delete(v);
        }
    }
}