#ifndef ODDITYENGINE_REGEX_H
#define ODDITYENGINE_REGEX_H

#include <string>
#include "Vector.h"

namespace OddityEngine::Regex {
    struct Character {
        std::string possible_chars;
        bool inverse;
    };

    class Pattern {
        Vector<Character> characters;
    };
}

#endif //ODDITYENGINE_REGEX_H
