#ifndef ODDITYENGINE_PROGRAM_H
#define ODDITYENGINE_PROGRAM_H

#include "GL/glew.h"

#include <string>
#include <vector>

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
            Program(std::initializer_list<GLuint> shaders);
            // Program(std::string vertex, std::string fragment);
            ~Program();

            /***
             * Finds uniform location in shader
             * @param name variable name
             * @return location
             */
            GLint uniform_location(const std::string& name) const;

            void apply();

            GLuint get_ID() const;

            /**
             * Inserts ID for program
             * @return program ID
             */
            operator GLuint() const;
        };

    } // OddityEngine
} // Graphics

#endif //ODDITYENGINE_PROGRAM_H
