#include "Object.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <Util/Time.h>

namespace OddityEngine::Physics {
    std::vector<Object*> Object::instances;

    Object::Object(glm::vec3 position, glm::vec3 angle) : position(position), angle(angle) {
        instances.emplace_back(this);
        normalize();
    }

    void Object::update() {
        on_ground = position.y < 0;

        float delta_seconds = Time::get_delta();

        glm::vec3 g = {0, on_ground ? 0 : -Gravity, 0};
        float r = Air_Density;
        glm::vec3 V = velocity;

        float Cd = 5;

        float A = 1;

        glm::vec3 D = Cd * (r * V * abs(V)) / 2.0f * A;
        glm::vec3 W = mass * g;

        acceleration += (W - D) / glm::vec3(1, on_ground ? 1 : mass, 1);

        velocity += acceleration * delta_seconds * 0.5f;
        position += velocity * delta_seconds;
        if (on_ground) {
            position.y = 0;
            velocity.y = 0;
            acceleration.y = 0;
        }
        velocity += acceleration * delta_seconds * 0.5f;

        angle_velocity += angle_acceleration * delta_seconds * 0.5f;
        angle += angle_velocity * delta_seconds;
        angle_velocity += angle_acceleration * delta_seconds * 0.5f;

        normalize();
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

    void Object::update_all() {
        for (auto i : instances) {
            i->update();
        }
    }
} // OddityEngine