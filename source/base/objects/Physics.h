#ifndef ODDITYENGINE_PHYSICS_H
#define ODDITYENGINE_PHYSICS_H

#include "Object.h"

class Physics : public virtual Object {
public:
    vec3 *boundingBoxPosition;
    vec3 *boundingBoxHalfExtend;

    Physics(vec3 *boundingBoxPosition, vec3 *boundingBoxHalfExtend);

    void loop(float deltaSeconds);
};

#endif //ODDITYENGINE_PHYSICS_H
