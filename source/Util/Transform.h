#ifndef ODDITYENGINE_TRANSFORM_H
#define ODDITYENGINE_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace OddityEngine {
    namespace Util {
        glm::mat4 transform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) {
            return glm::scale(glm::mat4(1), scale) * glm::toMat4(glm::quat(rotation)) * glm::translate(glm::mat4(1), translation);
        }
    }
} // Transform

#endif //ODDITYENGINE_TRANSFORM_H
