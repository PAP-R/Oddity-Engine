//
// Created by jan on 4/29/24.
//

#include "Regex.h"

namespace OddityEngine::Regex {
//    std::function<bool(char)>

    Set::Set(char c) {
        matches.emplace_back([c](char cIn){return cIn == c;});
    }

    Set::Set(const std::string& set) {
    }

    Set::Set(std::string::const_iterator start, const std::string::const_iterator& end) {
        if (*start == '^') {
            inverse = true;
            ++start;
        }

        for (auto it = start; it != end; ++it) {
            auto c = *it;
            matches.emplace_back([c](char cIn){return cIn == c;});
        }
    }

    bool Set::match(char c) {
        bool result = false;

        for (auto& m : matches) {
            result |= m(c);
        }

        return inverse != result;
    }


    std::string Pattern::character_class(char c) {
        switch (c) {
            default:
                return {c};
            case 'w':
                return R"([A-Za-z0-9_])";
            case 'W':
                return R"([^\w])";
            case 'd':
                return R"([0-9])";
            case 'D':
                return R"([^\d])";
            case 's':
                return "[ \t\r\n\f]";
            case 'S':
                return R"([^\s])";
        }
    }

    void Pattern::insert(const char &c) {
        sets.emplace_back(c);
    }

    void Pattern::insert(const std::string &pattern) {
        for (size_t ci = 0; ci < pattern.size(); ci++) {
            auto c = pattern[ci];
            switch(c) {
                default: {
                    insert(c);
                    break;
                }
                case '\\': {
                    ci += 1;
                    insert(character_class(pattern[ci]));
                    break;
                }
                case '[': {
                    size_t close = pattern.find(']', ci);
                    if (close == pattern.npos) Debug::error("Error at {} in regex pattern:\n{}", ci, pattern);
                    sets.emplace_back(pattern.begin(), pattern.begin() + close);
                    ci = close;
                }
            }
        }
    }

    std::string Pattern::set_string(const std::string::const_iterator& start, const std::string::const_iterator& end) {
        std::string match_string;
        if (*start == '^') {

        }
        for (auto it = start; it != end; ++it) {
            auto c = *it;

        }
    }

    Pattern::Pattern(const std::string& pattern) {
        insert(pattern);
    }

    bool Pattern::match(const std::string& input) {
        size_t count = 0;
        size_t set_index = 0;

        for (auto& c : input) {
            if (set_index >= sets.size()) return false;

            auto& set = sets[set_index];

            if (set.match(c)) {
                ++count;
            }
            else if (set.min <= count && count <= set.max) {
                ++set_index;
            }
            else {
                return false;
            }
        }

        return true;
    }
} // OddityEngine