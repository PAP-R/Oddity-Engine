#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace OddityEngine::Physics {
    enum STATES {
        TO_DELETE = 1,
    };

    struct alignas(16) Object_struct {
        glm::vec4 position = {0, 0, 0, 1};
        glm::vec4 velocity = {0, 0, 0, 1};
        glm::vec4 acceleration = {0, 0, 0, 1};

        glm::vec4 angle = {0, 0, 0, 1};
        glm::vec4 angle_velocity = {0, 0, 0, 1};
        glm::vec4 angle_acceleration = {0, 0, 0, 1};

        glm::vec4 test_value = {0, 0, 0, 0};

        float mass = 1;
        GLuint state = 0;
    };

    struct Object : public Object_struct {
        glm::quat orientation = {1, 0, 0, 0};

        virtual ~Object() = default;

        explicit Object(glm::vec3 position = {0, 0, 0}, glm::vec3 angle = {0, 0, 0});

        void normalize();

        glm::vec3 front();
        glm::vec3 right();
        glm::vec3 up();

        virtual glm::vec3 closest(glm::vec3 point);
        virtual float distance(glm::vec3 point);
    };
}


#endif //OBJECT_H
