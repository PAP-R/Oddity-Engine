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

    string version = "#version 460 core";

    vector<string> code;

    vector<size_t> indices;

public:
    Shader(GLuint type);

    Shader(GLuint type, const string& path);

    ~Shader() {
        glDeleteShader(ID);
    }
//
//    void add_function(const string& code);
//
//    void add_main(const string& code);
//
//    void add_constant(const string& constant);
//
//    void add_in(const string& input);
//
//    void add_in(const string &layout, const string& input);
//
//    void add_buffer(const string &layout, const string& input);
//
//    void add_out(const string& output);
//
//    void add_uniform(const string& uniform);
//
//    void add(const string& whatever);

    void compile();

    operator unsigned int() const {
        return ID;
    }
};

GLuint createProgram(GLuint vertex, GLuint fragment);

#endif //ODDITYENGINE_SHADER_H
