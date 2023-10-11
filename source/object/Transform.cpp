#include "Transform.h"

namespace OddityEngine {
    Transform::Transform() {
        this->translation.parent = this;
        this->rotation.parent = this;
        this->scale.parent = this;
        this->transformation = mat4(1);
    }

    Transform::Transform(vec3 translation, quat rotation, vec3 scale) : Transform() {
        this->transformation = translate(mat4(1), translation) * glm::toMat4(rotation) * glm::scale(mat4(1), scale);
    }

    mat4 Transform::get_transform() {
        return this->transformation;
    }

    Transform::operator mat4() {
        return this->transformation;
    }

    mat4 Transform::transform(vec3 translation, quat rotation, vec3 scale) {
        return translate(mat4(1), translation) * glm::toMat4(rotation) * glm::scale(mat4(1), scale);
    }

/// Translation
    Transform::translation::operator vec3() const {
        return parent->transformation[3];
    }

    void Transform::translation::set(vec3 translation) {
        parent->transformation = parent->transform(translation, quat(parent->rotation), parent->scale);
    }

    void Transform::translation::add(vec3 translation) {
        parent->transformation = translate(parent->transformation, translation);
    }

    mat4 Transform::translation::to_transform(vec3 translation) {
        return translate(mat4(1), translation);
    }

/// Rotation
    Transform::rotation::operator quat() const {
        mat4 temp = parent->transformation;
        temp[3] = vec4(0, 0, 0, 1);
        vec3 scale = parent->scale;
        for (int i = 0; i < 3; i++) {
            temp[i] /= scale[i];
        }

        return glm::toQuat(temp);
    }

    Transform::rotation::operator vec3() const {
        return eulerAngles(quat(*this));
    }

    void Transform::rotation::set(quat rotation) {
        parent->transformation = parent->transform(parent->translation, rotation, parent->scale);
    }

    mat4 Transform::rotation::to_transform(quat rotation) {
        return glm::toMat4(rotation);
    }

/// Scale
    Transform::scale::operator vec3() const {
        return {glm::length(parent->transformation[0]), glm::length(parent->transformation[1]), glm::length(parent->transformation[2])};
    }

    void Transform::scale::set(vec3 scale) {
        parent->transformation = parent->transform(parent->translation, quat(parent->rotation), scale);
    }

    void Transform::scale::add(vec3 scale) {
        parent->transformation = glm::scale(parent->transformation, scale);
    }

    mat4 Transform::scale::to_transform(vec3 scale) {
        return glm::scale(mat4(1), scale);
    }
}




