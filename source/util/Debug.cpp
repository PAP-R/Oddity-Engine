#include <variant>
#include "Debug.h"

#include "fmt/core.h"

namespace OddityEngine::Debug {
    std::vector<std::pair<fmt::string_view, fmt::format_args>> texts;
    std::vector<point> points;

    void add_point(glm::vec3 *pos, std::string *text) {

    }

    std::vector<point> get_points() {
        return points;
    }

    point get_point(size_t index) {
        return points[index];
    }

    void _add_text(fmt::string_view format, fmt::format_args args) {
        fmt::vprint(format, args);
        texts.emplace_back(format, args);
    }

    std::vector<std::string> get_texts() {
        std::vector<std::string> actual;
        for (auto text_pair : texts) {
            actual.emplace_back(fmt::vformat(text_pair.first, text_pair.second));
        }
        return actual;
    }

    std::string get_text(size_t index) {
        return fmt::vformat(texts[index].first, texts[index].second);
    }
}