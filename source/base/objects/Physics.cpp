#include "Physics.h"

#include "glm/glm.hpp"
#include <glm/gtx/vector_angle.hpp>

using namespace glm;

#include <vector>
#include <algorithm>
#include <ranges>
#include <map>
#include "Grid.h"
#include "source/base/tools/Debug.h"
#include "fmt/format.h"

#define FALLOFF 0.01f

vector<Physics*> physicsObjects;
Grid<Physics*> physicsObjectSpace(4);

void Physics::add_velocity(vec3 deltaVelocity) {
    acceleration += deltaVelocity;
}

void Physics::move(float deltaSeconds) {
    velocity += acceleration * deltaSeconds;
    acceleration = vec3(0);
    pos += velocity * deltaSeconds;
    velocity -= velocity * resistance * deltaSeconds;
}

void Physics::update_physics_sub_steps(float deltaSeconds, size_t substeps) {
    float subDeltaSeconds = deltaSeconds / substeps;
    for (int i = 0; i < substeps; i++) {
        for (auto po : physicsObjects) {
            if (po->gravity != 0) {
                po->update_gravity(subDeltaSeconds);
            }
            if (po->movable) {
                po->move(subDeltaSeconds);
                po->update_physics(subDeltaSeconds);
            }
        }
    }
}

void Physics::update_gravity(float deltaSeconds) {
    for (auto po : physicsObjects | ranges::views::filter([this](Physics* po){return po->movable && po != this;})) {
        po->add_velocity(normalize(this->pos - po->pos) * (this->gravity * po->mass / (distance(this->pos, po->pos) * FALLOFF)));
    }
}

bool Physics::collision_axis_aligned_bounding_box(Physics *other) {
    if (this == other) {
        return false;
    }
    if (abs(this->boundingBoxPosition->x - other->boundingBoxPosition->x) < this->boundingBoxHalfExtend->x + other->boundingBoxHalfExtend->x) {
        if (abs(this->boundingBoxPosition->y - other->boundingBoxPosition->y) < this->boundingBoxHalfExtend->y + other->boundingBoxHalfExtend->y) {
            if (abs(this->boundingBoxPosition->z - other->boundingBoxPosition->z) < this->boundingBoxHalfExtend->z + other->boundingBoxHalfExtend->z) {
                return true;
            }
        }
    }

    return false;
}

bool Physics::collision_sphere(Physics *other) {
    return (distance(this->pos, other->pos) < this->radius + other->radius);
}

void Physics::react_sphere(Physics *other) {
    float velo = length(this->velocity) + length(other->velocity);

    float sharedBounciness = (this->bounciness + other->bounciness) / 2;

    vec3 impact = this->pos - other->pos;
    float push = (this->radius + other->radius) - length(impact);
    vec3 impactDirection = normalize(impact);

    float impactMass = this->mass + other->mass;

    if (other->movable) {
        this->pos += impactDirection * push;
        other->pos -= impactDirection * push;
        this->velocity = normalize(reflect(this->velocity, impactDirection)) * (velo * this->mass / impactMass) * sharedBounciness;
        other->velocity = normalize(reflect(other->velocity, -impactDirection)) * (velo * other->mass / impactMass) * sharedBounciness;
    }
    else {
        this->pos += impactDirection * push ;
        this->velocity = reflect(this->velocity, impactDirection) * bounciness;
    }
}

bool Physics::collide(Physics *other) {
    if (this == other) {
        return false;
    }
    bool collision = false;
    switch (this->shape + other->shape) {
        case PHYSICS_SHAPE_CUBE + PHYSICS_SHAPE_CUBE:
            if (collision_axis_aligned_bounding_box(other)) {
                collision = true;
            }
        case PHYSICS_SHAPE_SPHERE + PHYSICS_SHAPE_SPHERE:
            if (collision_sphere(other)) {
                react_sphere(other);
                collision = true;
            }
    }

    return collision;
}

void Physics::update_physics(float deltaSeconds) {
    auto key = static_cast<vec<3, int>>(this->pos / float(physicsObjectSpace.get_chunksize()));

    auto sizeInGrid = physicsObjectSpace.in_grid(*this->boundingBoxHalfExtend) + vec<3, int>(1);

    vector<Physics *> collided;

    for (int x = -sizeInGrid.x; x <= sizeInGrid.x; x++) {
        for (int y = -sizeInGrid.y; y <= sizeInGrid.y; y++) {
            for (int z = -sizeInGrid.z; z <= sizeInGrid.z; z++) {
                for (auto po : physicsObjectSpace.get(key + vec<3, int>(x, y, z))) {// | std::ranges::views::filter([this](Physics* po){ return this != po && distance(*this->boundingBoxPosition, *po->boundingBoxPosition) < this->radius + po->radius; })) {
                    if (find(collided.begin(), collided.end(), po) == collided.end()) {
                        if (collide(po)) {
                            collided.emplace_back(po);
                        }
                    }
                }
            }
        }
    }

    collided.clear();

    physicsObjectSpace.move(key, this);
}

void Physics::loop(float deltaSeconds) {
    Object::loop(deltaSeconds);

    update_physics_sub_steps(deltaSeconds, this->substeps);
}

Physics::Physics(vec3 *boundingBoxPosition, vec3 *boundingBoxHalfExtend, bool movable, Shape shape) : boundingBoxPosition(boundingBoxPosition), boundingBoxHalfExtend(boundingBoxHalfExtend), movable(movable), shape(shape) {
    physicsObjects.emplace_back(this);

    radius = length(*boundingBoxHalfExtend);
    auto key = static_cast<vec<3, int>>(*boundingBoxPosition / float(physicsObjectSpace.get_chunksize()));
    physicsObjectSpace.insert(key, this, *boundingBoxHalfExtend);
}

void Physics::setMovable(bool movable) {
    Physics::movable = movable;
}

void Physics::setGravity(float gravity) {
    this->gravity = gravity;
    this->mass = gravity;
}

void Physics::setShape(Shape shape) {
    this->shape = shape;
    switch (shape) {
        case PHYSICS_SHAPE_CUBE:
            radius = length(*boundingBoxHalfExtend);
            break;
        case PHYSICS_SHAPE_SPHERE:
            radius = glm::max(boundingBoxHalfExtend->x, glm::max(boundingBoxHalfExtend->y, boundingBoxHalfExtend->z));
            break;
    }
}
