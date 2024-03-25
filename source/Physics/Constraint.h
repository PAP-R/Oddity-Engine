#ifndef CONSTRAINT_H
#define CONSTRAINT_H
#include <glm/vec4.hpp>

#include "Util/CombinedIndex.h"

namespace OddityEngine::Physics {
    struct alignas(16) Constraint_struct {
        float offset = 0;
        GLuint type = 0;
        GLuint first = 0;
        GLuint second = 0;
    };

    class Constraint : Constraint_struct {

    };
}

#endif //CONSTRAINT_H
