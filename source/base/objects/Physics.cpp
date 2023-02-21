#include "Physics.h"

#include "glm/glm.hpp"

using namespace glm;

#include <vector>
#include <algorithm>
#include <ranges>
#include <map>
#include "Grid.h"

#define chunkSize 4.0f

vector<Physics*> physicsObjects;
Grid<Physics*> physicsObjectSpace(4);

void Physics::update_physics_sub_steps(float deltaSeconds, size_t substeps) {
    float subDeltaSeconds = deltaSeconds / substeps;
    for (int i = 0; i < substeps; i++) {
        for (auto po : physicsObjects | ranges::views::filter([](Physics* po){return po->movable || po->gravity != 0;})) {

            po->update_physics(subDeltaSeconds);
        }
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
    if (this == other || distance(*this->boundingBoxPosition, *other->boundingBoxPosition) > this->radius + other->radius) {
        return false;
    }
    return true;
}

void Physics::update_physics(float deltaSeconds) {
    auto key = static_cast<vec<3, int>>(this->pos / chunkSize);

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            for (int z = -1; z <= 1; z++) {
                for (auto po : physicsObjectSpace.get(key + vec<3, int>(x, y, z))) {// | std::ranges::views::filter([this](Physics* po){ return this != po && distance(*this->boundingBoxPosition, *po->boundingBoxPosition) < this->radius + po->radius; })) {

                }
            }
        }
    }

    physicsObjectSpace.move(key, this);
}

void Physics::loop(float deltaSeconds) {
    Object::loop(deltaSeconds);

    update_physics_sub_steps(deltaSeconds, this->substeps);
}

Physics::Physics(vec3 *boundingBoxPosition, vec3 *boundingBoxHalfExtend, bool movable, Shape shape) : boundingBoxPosition(boundingBoxPosition), boundingBoxHalfExtend(boundingBoxHalfExtend), movable(movable), shape(shape) {
    physicsObjects.emplace_back(this);

    radius = length(*boundingBoxHalfExtend);
    auto key = static_cast<vec<3, int>>(*boundingBoxPosition / chunkSize);
    physicsObjectSpace.insert(key, this);
}

void Physics::setMovable(bool movable) {
    Physics::movable = movable;
}

void Physics::setGravity(float gravity) {
    Physics::gravity = gravity;
}
