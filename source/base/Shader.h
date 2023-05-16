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
    GLuint type;

    string version = "#version 460 core";

    vector<std::string> general;

    vector<std::string> constants;
    vector<std::string> inputs;
    vector<std::string> outputs;
    vector<std::string> uniforms;

    vector<std::string> function_declarations;
    vector<std::string> function_definitions;
    vector<std::string> main_part;

public:
    Shader(GLuint type) {
        this->type = type;
        ID = glCreateShader(type);
    }

    ~Shader() {
        glDeleteShader(ID);
    }

    void add_function(const string& code);

    void add_main(const string& code);

    void add_constant(const string& constant);

    void add_in(const string& input);

    void add_in(const string &layout, const string& input);

    void add_buffer(const string &layout, const string& input);

    void add_out(const string& output);

    void add_uniform(const string& uniform);

    void add(const string& whatever);

    void compile();

    operator unsigned int() const {
        return ID;
    }
};

GLuint createProgram(GLuint vertex, GLuint fragment);

#endif //ODDITYENGINE_SHADER_H
