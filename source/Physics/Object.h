#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Physics {
    class Object {
    protected:
    public:
        glm::vec3 position = {0, 0, 0};
        glm::vec3 velocity = {0, 0, 0};
        glm::vec3 acceleration = {0, 0, 0};

        glm::vec3 angle = {0, 0, 0};
        glm::vec3 angle_velocity = {0, 0, 0};
        glm::vec3 angle_acceleration = {0, 0, 0};
        glm::quat orientation = {1, 0, 0, 0};

        float mass = 1;

        explicit Object(glm::vec3 position = {0, 0, 0}, glm::vec3 angle = {0, 0, 0});

        void normalize();

        glm::vec3 front();
        glm::vec3 right();
        glm::vec3 up();
    };
}


#endif //OBJECT_H
