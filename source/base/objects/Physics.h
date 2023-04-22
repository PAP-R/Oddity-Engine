#ifndef ODDITYENGINE_PHYSICS_H
#define ODDITYENGINE_PHYSICS_H

#include "Object.h"

class Physics;

typedef bool (*Collision)(Physics *);

typedef enum {
    PHYSICS_SHAPE_CUBE,
    PHYSICS_SHAPE_SPHERE
} Shape;

class Physics : public virtual Object {
private:
    vec3 velocity = vec3(0);
    vec3 acceleration = vec3(0);
    float resistance = 0.01f;

    float bounciness = 0.6f;

    vec3 *boundingBoxPosition;
    vec3 *boundingBoxHalfExtend;
    float radius;
    size_t substeps = 4;
    bool movable = false;
    float gravity = 0;
    float mass = 1;
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

    void update_gravity(float deltaSeconds);

    void add_velocity(vec3 deltaVelocity);

    bool collide(Physics *other);

    void setShape(Shape shape);

    void move(float deltaSeconds);

    void react_sphere(Physics *other);
};

#endif //ODDITYENGINE_PHYSICS_H
