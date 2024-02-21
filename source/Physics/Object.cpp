#include "Object.h"

namespace OddityEngine::Physics {
    Object::Object(glm::vec3 position, glm::vec3 angle) {
        this->position = {position, 1};
        this->angle = {angle, 1};
        normalize();
    }

    bool Object::update() {
        return true;
    }

    void Object::normalize() {
        for (int i = 0; i < angle.length(); i++) {
            angle[i] = fmod(angle[i], 360);
        }

        orientation = glm::angleAxis(glm::radians(angle.x), glm::vec3(0, 1, 0));
        orientation *= glm::angleAxis(glm::radians(angle.y), glm::vec3(1, 0, 0));
    }

    glm::vec3 qrot(glm::quat q, glm::vec3 v) {
        glm::vec3 qv(q.x, q.y, q.z);
        return v + 2.0f * glm::cross(qv, cross(qv, v) + q.w * v);
    }

    glm::vec3 Object::front() {
        normalize();
        return qrot(orientation, glm::vec3(0, 0, -1));
    }

    glm::vec3 Object::right() {
        normalize();
        return qrot(orientation, glm::vec3(1, 0, 0));
    }

    glm::vec3 Object::up() {
        normalize();
        return qrot(orientation, glm::vec3(0, 1, 0));
    }

    glm::vec4 Object::closest(glm::vec3 point) {
        return closest(point, glm::vec3(position), glm::vec3(angle));
    }

    glm::vec4 Object::closest(glm::vec3 point, glm::vec3 position, glm::vec3 angle) {
        glm::vec3 diff = glm::vec3(position) - point;

        diff = glm::normalize(diff) * (glm::length(diff) - radius);

        glm::vec4 result(diff, glm::length(diff));

        if (glm::distance(diff + point, glm::vec3(position)) > glm::distance(point, glm::vec3(position))) {
            result.w *= -1;
        }

        return result;
    }

    float Object::distance(glm::vec3 point) {
        return glm::distance(glm::vec3(position), point);
    }
}