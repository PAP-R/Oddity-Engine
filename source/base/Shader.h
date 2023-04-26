#ifndef ODDITYENGINE_SHADER_H
#define ODDITYENGINE_SHADER_H

#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
using namespace glm;

#include <string>
#include <vector>
#include <cstring>

using namespace std;



class Shader {
private:
    GLuint ID;

    string version = "#version 460 core";

    vector<std::string> constants;
    vector<std::string> input;
    vector<std::string> Output;

    vector<std::string> function_declarations;
    vector<std::string> function_definitions;

public:
    Shader(GLuint type) {
        ID = glCreateShader(type);
    }

    void add_function(string code);

    void compile() {
        GLuint result = GL_FALSE;
        int infoLength = 0;


    }
};


#endif //ODDITYENGINE_SHADER_H
