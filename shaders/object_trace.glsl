#version 450 core

#include <object.glsl>

trace_result multi_trace(vec3 point, vec3 dir, uint max_steps, uint start, uint end) {
    dir = normalize(dir);
    vec3 start_point = point;
    vec3 min_point = point;
    trace_result current = multi_closest(point, 1, start, end);
    trace_result min = current;

    for (uint i = 0; i < max_steps; i++) {
        float angle = dot(current.diff, dir);
        if (angle > TOLERANCE) {
            point = point + dir * angle;
        }
        else {
            point = point + dir * abs(current.distance);
        }

        current = multi_closest(point, 1, start, end);

        if (abs(current.distance) < abs(min.distance)) {
            min_point = point;
            min = current;
            if (abs(min.distance) < TOLERANCE) {
                break;
            }
        }
    }

    min.diff += min_point - start_point;
    min.position = min.diff + start_point;

    return min;
}

trace_result trace(vec3 point, vec3 dir, uint max_steps, uint obj) {
    return multi_trace(point, dir, max_steps, obj, obj);
}

trace_result multi_trace_hit(vec3 point, vec3 dir, uint max_steps, uint start, uint end) {
    dir = normalize(dir);
    vec3 start_point = point;
    vec3 min_point = point;
    trace_result current = multi_closest(point, 1, start, end);
    trace_result min = current;

    for (uint i = 0; i < max_steps; i++) {
        point = point + dir * abs(current.distance);

        current = multi_closest(point, 1, start, end);

        if (abs(current.distance) < abs(min.distance)) {
            min_point = point;
            min = current;
            if (abs(min.distance) < TOLERANCE) {
                break;
            }
        }
    }

    min.diff += min_point - start_point;
    min.position = min.diff + start_point;

    return min;
}

trace_result trace_hit(vec3 point, vec3 dir, uint max_steps, uint obj) {
    return multi_trace_hit(point, dir, max_steps, obj, obj);
}