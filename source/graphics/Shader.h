#ifndef ODDITYENGINE_SHADER_H
#define ODDITYENGINE_SHADER_H

#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
using namespace glm;

#include <string>

namespace OddityEngine::Graphics::Shader {
    enum indextype {
        GENERAL,
        CONSTANTS,
        INPUTS,
        OUTPUTS,
        UNIFORM,
        FUNCTIONS_DECLARATIONS,
        FUNCTION_DEFINITIONS,
        MAIN,
    };

    class Shader {
    private:
        GLuint ID;
        GLuint type;

        std::string path;
        std::string code;

    public:
        Shader(GLuint type, const std::string& path);
        ~Shader();
        void compile();
        operator unsigned int() const;
    };

    GLuint create_program(GLuint vertex, GLuint fragment);
}


#endif //ODDITYENGINE_SHADER_H
