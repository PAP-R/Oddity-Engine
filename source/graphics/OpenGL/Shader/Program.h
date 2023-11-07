#ifndef ODDITYENGINE_PROGRAM_H
#define ODDITYENGINE_PROGRAM_H

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <string>

namespace OddityEngine {
    namespace Graphics {

        class Program {
        protected:
            GLuint ID;

        public:
            Program() = default;
            /**
             * Creates an Opengl program from a vertex and a fragment shader
             * @param vertex vertex shader
             * @param fragment fragment shader
             */
            Program(GLuint vertex, GLuint fragment);
            ~Program();

            /***
             * Finds uniform location in shader
             * @param name variable name
             * @return location
             */
            GLint uniform_location(const std::string& name);

            /**
             * Inserts ID for program
             * @return program ID
             */
            operator GLuint() const;
        };

    } // OddityEngine
} // Graphics

#endif //ODDITYENGINE_PROGRAM_H
