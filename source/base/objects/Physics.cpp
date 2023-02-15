#include "Physics.h"

#include <vector>
#include <algorithm>
#include <ranges>

vector<Physics*> physicsObjects;

void Physics::loop(float deltaSeconds) {
    Object::loop(deltaSeconds);

    this->pos.y -= deltaSeconds * 8;
    for (auto po : physicsObjects | std::ranges::views::filter([this](Physics* po){ return this != po; })) {
        if (abs(this->boundingBoxPosition->x - po->boundingBoxPosition->x) < this->boundingBoxHalfExtend->x + po->boundingBoxHalfExtend->x) {
            if (abs(this->boundingBoxPosition->y - po->boundingBoxPosition->y) < this->boundingBoxHalfExtend->y + po->boundingBoxHalfExtend->y) {
                if (abs(this->boundingBoxPosition->z - po->boundingBoxPosition->z) < this->boundingBoxHalfExtend->z + po->boundingBoxHalfExtend->z) {
                    this->pos.y += (this->boundingBoxHalfExtend->y + po->boundingBoxHalfExtend->y) - (this->boundingBoxPosition->y - po->boundingBoxPosition->y);
                }
            }
        }
    }
}

Physics::Physics(vec3 *boundingBoxPosition, vec3 *boundingBoxHalfExtend) : boundingBoxPosition(boundingBoxPosition), boundingBoxHalfExtend(boundingBoxHalfExtend) {
    physicsObjects.emplace_back(this);
}
