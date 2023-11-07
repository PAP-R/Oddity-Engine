#ifndef ODDITYENGINE_INTERFACE_H
#define ODDITYENGINE_INTERFACE_H

#include <glm/glm.hpp>

namespace OddityEngine {
    namespace Graphics {
        namespace Render {

            class Interface {
            public:
                void render();

                void set_screen_size(const glm::vec2 &size);
            };

        } // OddityEngine
    } // Graphics
} // Render

#endif //ODDITYENGINE_INTERFACE_H
