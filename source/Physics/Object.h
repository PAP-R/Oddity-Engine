#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <map>

#include <NeuralNetwork/Network.h>

#include "Graphics/Buffer/Buffer.h"

namespace OddityEngine::Physics {
    enum STATES {
        TO_DELETE = 1,
        SHOW = 1 << 1,
        HIT = 1 << 2,
        MOVE = 1 << 3,
        CLIP = 1 << 4,
        CONNECTED = 1 << 5,
    };

    enum SHAPES {
        SPHERE,
        NETWORK,
        PLANE,
        CUBE,
    };

    struct alignas(16) Object_struct {
        glm::vec4 position = {0, 0, 0, 1};
        glm::vec4 velocity = {0, 0, 0, 1};
        glm::vec4 acceleration = {0, 0, 0, 1};

        glm::vec4 angle = {0, 0, 0, 1};
        glm::vec4 angle_velocity = {0, 0, 0, 1};
        glm::vec4 angle_acceleration = {0, 0, 0, 1};

        glm::quat orientation = {1, 0, 0, 0};

        glm::mat4 transform;
        glm::mat4 inverse_transform;

        float test_value[10];

        float radius = 0;

        float mass = 1;
        float restitution = 1;
        GLuint state = SHOW | MOVE | CLIP;

        GLuint shape = SPHERE;
        GLuint network_index = 0;

        GLuint prev_index = 0;
        GLuint next_index = 0;
    };

    class Object : public Object_struct {
    protected:
        glm::vec3 _front = {0, 0, -1};
        glm::vec3 _right = {1, 0, 0};
        glm::vec3 _up = {0, 1, 0};
    public:
        Object* parent = nullptr;
        Object* prev = nullptr;
        Object* next = nullptr;

        std::map<void*, GLuint> buffer_indices;

        Vector<NeuralNetwork::Network> net = Vector<NeuralNetwork::Network>(10, NeuralNetwork::Network(9, 4));
        Vector<Object*> conections;

        virtual ~Object() = default;

        explicit Object(glm::vec3 position = {0, 0, 0}, glm::vec3 angle = {0, 0, 0}, SHAPES shape = SPHERE);

        Object& operator= (Object_struct obj);

        virtual bool update();
        virtual bool update(void* context);

        void normalize();

        void set_next(Object* next);
        void set_prev(Object* prev);

        glm::vec3 front();
        glm::vec3 right();
        glm::vec3 up();

        virtual glm::vec4 closest(glm::vec3 point);
        virtual glm::vec4 closest(glm::vec3 point, glm::vec3 position, glm::vec3 angle);
        virtual float distance(glm::vec3 point);
    };
}


#endif //OBJECT_H
