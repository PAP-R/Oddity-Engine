#ifndef ODDITYENGINE_PROGRAM_H
#define ODDITYENGINE_PROGRAM_H

#include "GL/glew.h"

#include "GLFW/glfw3.h"

namespace OddityEngine {
    namespace Graphics {

        class Program {
        protected:
            GLuint ID;

        public:
            /**
             * Creates an Opengl program from a vertex and a fragment shader
             * @param vertex vertex shader
             * @param fragment fragment shader
             */
            Program(GLuint vertex, GLuint fragment);
            ~Program();

            /**
             * Inserts ID for program
             * @return program ID
             */
            explicit operator GLuint() const;
        };

    } // OddityEngine
} // Graphics

#endif //ODDITYENGINE_PROGRAM_H
