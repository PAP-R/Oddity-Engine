#include "Object.h"

namespace OddityEngine {
    Object::Object() {
        this->type = OBJECT;
        data.parent = this;
    }

    Object::Object(Transform transform) : Object() {
        this->transform = transform;
    }
}