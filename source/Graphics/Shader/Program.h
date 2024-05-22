#ifndef ODDITYENGINE_PROGRAM_H
#define ODDITYENGINE_PROGRAM_H

#include "GL/glew.h"
#include "Shader.h"

#include <string>
#include <vector>

namespace OddityEngine {
    namespace Graphics {

        class Program {
        protected:
            GLuint ID;
            bool outdated = true;

        public:
            std::vector<Shader> shaders;


            Program() = default;
            /**
             * Creates an Opengl program from a vertex and a fragment shader
             * @param vertex vertex shader
             * @param fragment fragment shader
             */
            Program(std::initializer_list<Shader> shaders);
            // Program(std::string vertex, std::string fragment);
            ~Program();

            void compile_shaders();
            GLuint compile();
            GLuint recompile();

            /***
             * Finds uniform location in shader
             * @param name variable name
             * @return location
             */
            GLint uniform_location(const std::string& name) const;

            void apply();

            GLuint get_ID();

            /**
             * Inserts ID for program
             * @return program ID
             */
            operator GLuint();
        };

    } // OddityEngine
} // Graphics

#endif //ODDITYENGINE_PROGRAM_H
