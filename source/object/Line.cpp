#include "Line.h"

namespace OddityEngine {
    Line::Line() : Object() {
        this->type = LINE;
    }

    Line::Line(vec4 start, vec4 stop) : Line() {
        this->start = start;
        this->stop = stop;
    }

    Line::Line(vec3 start, float start_size, vec3 stop, float stop_size) : Line(vec4(start, start_size), vec4(stop, stop_size)) {}
}
