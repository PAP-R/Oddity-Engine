#ifndef ODDITY_OBJECT_H
#define ODDITY_OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <vector>

#define Gravity 9.81f
#define Air_Density 1.0f

namespace OddityEngine {

    class Object {
    protected:
        static std::vector<Object*> instances;

    public:
        glm::vec3 position = {0, 0, 0};
        glm::vec3 velocity = {0, 0, 0};
        glm::vec3 acceleration = {0, 0, 0};

        glm::vec3 angle = {0, 0, 0};
        glm::vec3 angle_velocity = {0, 0, 0};
        glm::vec3 angle_acceleration = {0, 0, 0};
        glm::quat orientation = {1, 0, 0, 0};

        float mass = 10;

        bool on_ground = false;

        Object();

        virtual void update();

        void normalize();

        [[nodiscard]] glm::vec3 front();
        [[nodiscard]] glm::vec3 right();
        [[nodiscard]] glm::vec3 up();

        static void update_all();
    };

} // OddityEngine

#endif //ODDITY_OBJECT_H
