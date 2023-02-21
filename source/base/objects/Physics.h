#ifndef ODDITYENGINE_PHYSICS_H
#define ODDITYENGINE_PHYSICS_H

#include "Object.h"

class Physics;

typedef bool (*Collision)(Physics *);

typedef enum {
    PHYSICS_SHAPE_CUBE,
    PHYSICS_SHAPE_SPPHERE
} Shape;

class Physics : public virtual Object {
private:
    vec3 *boundingBoxPosition;
    vec3 *boundingBoxHalfExtend;
    float radius;
    size_t substeps = 4;
    bool movable = false;
    float gravity = 0;
    Shape shape;

public:
    void setMovable(bool movable);

    void setGravity(float gravity);

    Physics(vec3 *boundingBoxPosition, vec3 *boundingBoxHalfExtend, bool movable=false, Shape shape=PHYSICS_SHAPE_CUBE);

    void loop(float deltaSeconds);

    static void update_physics_sub_steps(float deltaSeconds, size_t substeps);

    void update_physics(float deltaSeconds);

    bool collision_sphere(Physics *other);

    bool collision_axis_aligned_bounding_box(Physics *other);
};

#endif //ODDITYENGINE_PHYSICS_H
