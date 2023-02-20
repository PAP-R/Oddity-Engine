#ifndef ODDITYENGINE_PHYSICS_H
#define ODDITYENGINE_PHYSICS_H

#include "Object.h"

class Physics : public virtual Object {
public:
    vec3 *boundingBoxPosition;
    vec3 *boundingBoxHalfExtend;
    float radius;
    size_t substeps = 4;
    bool movable = false;

    Physics(vec3 *boundingBoxPosition, vec3 *boundingBoxHalfExtend, bool movable=false);

    void loop(float deltaSeconds);

    static void update_physics_sub_steps(float deltaSeconds, size_t substeps);

    void update_physics(float deltaSeconds);
};

#endif //ODDITYENGINE_PHYSICS_H
