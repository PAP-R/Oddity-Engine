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
        for (auto po : physicsObjects | ranges::views::filter([](Physics* po){return po->movable;})) {
            po->update_physics(subDeltaSeconds);
        }
    }
}

void Physics::update_physics(float deltaSeconds) {
    auto lastPos = this->pos;
    this->pos.y -= deltaSeconds * 8;

    auto key = static_cast<vec<3, int>>(this->pos / chunkSize);

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            for (int z = -1; z <= 1; z++) {
                for (auto po : physicsObjectSpace.get(key + vec<3, int>(x, y, z))) {// | std::ranges::views::filter([this](Physics* po){ return this != po && distance(*this->boundingBoxPosition, *po->boundingBoxPosition) < this->radius + po->radius; })) {
                    if (this == po && distance(*this->boundingBoxPosition, *po->boundingBoxPosition) > this->radius + po->radius) {
                        continue;
                    }
                    if (abs(this->boundingBoxPosition->x - po->boundingBoxPosition->x) < this->boundingBoxHalfExtend->x + po->boundingBoxHalfExtend->x) {
                        if (abs(this->boundingBoxPosition->y - po->boundingBoxPosition->y) < this->boundingBoxHalfExtend->y + po->boundingBoxHalfExtend->y) {
                            if (abs(this->boundingBoxPosition->z - po->boundingBoxPosition->z) < this->boundingBoxHalfExtend->z + po->boundingBoxHalfExtend->z) {
                                float distance = (this->boundingBoxHalfExtend->y + po->boundingBoxHalfExtend->y) - (this->boundingBoxPosition->y - po->boundingBoxPosition->y);
                                if (po->movable) {
                                    this->pos.y += distance / 2;
                                    po->pos.y -= distance / 2;
                                }
                                else {
                                    this->pos.y += distance;
                                }

                            }
                        }
                    }
                }
            }
        }
    }

    physicsObjectSpace.move(key, this);
}

void Physics::loop(float deltaSeconds) {
    Object::loop(deltaSeconds);

    if (movable) {
        update_physics_sub_steps(deltaSeconds, this->substeps);
    }
}

Physics::Physics(vec3 *boundingBoxPosition, vec3 *boundingBoxHalfExtend, bool movable) : boundingBoxPosition(boundingBoxPosition), boundingBoxHalfExtend(boundingBoxHalfExtend), movable(movable) {
    physicsObjects.emplace_back(this);

    radius = length(*boundingBoxHalfExtend);
    auto key = static_cast<vec<3, int>>(*boundingBoxPosition / chunkSize);
    physicsObjectSpace.insert(key, this);
}
