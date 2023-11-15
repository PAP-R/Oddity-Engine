#ifndef TESTSPIEL_RAYSTERIZER_H
#define TESTSPIEL_RAYSTERIZER_H

#include "Interface.h"
#include "Graphics/Camera.h"

namespace OddityEngine {
    namespace Graphics {
        namespace Render {

            class Raysterizer : public Interface {
            private:
                Camera* camera;

            };

        } // OddityEngine
    } // Graphics
} // Render

#endif //TESTSPIEL_RAYSTERIZER_H
