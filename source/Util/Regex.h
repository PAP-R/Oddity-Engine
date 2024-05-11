#ifndef ODDITYENGINE_REGEX_H
#define ODDITYENGINE_REGEX_H

#include <string>
#include "Vector.h"

namespace OddityEngine::Regex {
    struct Set {
        Vector<std::function<bool(char)>> matches;
        size_t min = 1;
        size_t max = 1;
        bool inverse = false;
        bool include = true;

        Set* nested = nullptr;

        Set() = default;
        explicit Set(char c);
        explicit Set(const std::string &set);
        explicit Set(std::string::const_iterator start, const std::string::const_iterator& end);

        bool match(char c);
    };

    class Pattern {
    protected:
        std::string character_class(char c);

        void insert(const char& c);
        void insert(const std::string& pattern);
        std::string set_string(const std::string::const_iterator& start, const std::string::const_iterator& end);

    public:
        Vector<Set> sets;

        Pattern(const std::string& pattern);

        bool match(const std::string& input);
    };

    Vector<std::string> search(std::string text, Pattern pattern);
}

#endif //ODDITYENGINE_REGEX_H
