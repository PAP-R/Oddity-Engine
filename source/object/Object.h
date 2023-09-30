#ifndef ODDITYENGINE_OBJECT_H
#define ODDITYENGINE_OBJECT_H

#include "Transform.h"

#define OBJECT 0

namespace OddityEngine {
    struct alignas(16) buffer_object {
        uint32 type;
        mat4 transform;
    };

    class Object {
    protected:
        uint32 type;
        Transform transform;
    public:
        Object();
        Object(Transform transform);


        struct data {
            Object* parent;

            struct object {
                data* parent;
                operator void*();
                uint32 size();
            } object;
        } data;
    };
}

#endif //ODDITYENGINE_OBJECT_H
