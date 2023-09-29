#ifndef ODDITYENGINE_TRANSFORM_H
#define ODDITYENGINE_TRANSFORM_H

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
using namespace glm;

namespace OddityEngine {
    class Transform {
        mat4 transformation = mat4(1);

    public:
        Transform(vec3 translation, quat rotation, vec3 scale);
        Transform();

        static mat4 transform(vec3 translation, quat rotation, vec3 scale);

        mat4 get_transform();
        operator mat4();

        struct translation {
        public:
            Transform* parent;
            operator vec3() const;
            void set(vec3 translation);
            void add(vec3 translation);
            static mat4 to_transform(vec3 translation);
        } translation;

        struct rotation {
        public:
            Transform* parent;
            operator quat() const;
            operator vec3() const;
            void set(quat rotation);
            void add(quat rotation);
            static mat4 to_transform(quat rotation);
        } rotation;

        struct scale {
        public:
            Transform* parent;
            operator vec3() const;
            void set(vec3 scale);
            void add(vec3 scale);
            static mat4 to_transform(vec3 scale);
        } scale;
    };
}


#endif //ODDITYENGINE_TRANSFORM_H
