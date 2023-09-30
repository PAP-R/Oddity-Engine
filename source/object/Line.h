#ifndef ODDITYENGINE_LINE_H
#define ODDITYENGINE_LINE_H

#include "Object.h"

#define LINE 1

namespace OddityEngine {
    struct alignas(16) buffer_object_line : buffer_object {
        vec4 start;
        vec4 stop;
    };

    class Line : Object {
    protected:
        vec4 start;
        vec4 stop;

    public:
        Line();
        Line(vec4 start, vec4 stop);
        Line(vec3 start, float start_size, vec3 stop, float stop_size);

        struct data {
            Object* parent;
            void* vertex();
            void* index();
            void* transform();
            void* object();
        } data;
    };
}


#endif //ODDITYENGINE_LINE_H
