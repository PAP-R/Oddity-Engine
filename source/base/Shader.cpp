#include "Shader.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "fmt/core.h"
using namespace fmt;

Shader::Shader(GLuint type) : type(type), ID(glCreateShader(type)) {
}

Shader::Shader(GLuint type, const string &path) : type(type), ID(glCreateShader(type)){
    string line;
    stringstream sstr;

    ifstream shaderStream(path, ios::in);
    if (shaderStream.is_open()) {
        sstr << shaderStream.rdbuf();
        shaderStream.close();
    }
    else {
        printf("%s", format("Failed to read File {}\n", path).c_str());
    }

    while (getline(sstr, line, '\n')) {
        this->code.emplace_back(line);
    }
}

//void Shader::add_function(const string& code) {
//    string declaration = code.substr(0, code.find("{"));
//    string definition = code;
//    function_declarations.emplace_back(declaration.append(";\n"));
//    function_definitions.emplace_back(definition.append("\n"));
//}
//
//void Shader::add_main(const string &code) {
//    main_part.emplace_back(code);
//}
//
//void Shader::add_constant(const string &constant) {
//    constants.emplace_back(format("const {};\n", constant));
//}
//
//void Shader::add_in(const string &input) {
//    inputs.emplace_back(format("in {};\n", input));
//}
//
//void Shader::add_in(const string &layout, const string &input) {
//    inputs.emplace_back(format("layout(location = {}) in {};\n", layout, input));
//}
//
//void Shader::add_buffer(const string &layout, const string &input) {
//    inputs.emplace_back(format("layout({}) {};\n", layout, input));
//}
//
//void Shader::add_out(const string &output) {
//    outputs.emplace_back(format("out {};\n", output));
//}
//
//void Shader::add_uniform(const string &uniform) {
//    outputs.emplace_back(format("uniform {};\n", uniform));
//}
//
//void Shader::add(const string &whatever) {
//    general.emplace_back(format("{};\n", whatever));
//}

void Shader::compile() {
    stringstream code;
//
//    code << version << "\n";
//
//    for (const auto& c : constants)
//        code << c;
//
//    code << "\n";
//
//    for (const auto& g : general)
//        code << g;
//
//    code << "\n";
//
//    for (const auto& i : inputs)
//        code << i;
//
//    code << "\n";
//
//    for (const auto& o : outputs)
//        code << o;
//
//    code << "\n";
//
//    for (const auto& fd : function_declarations)
//        code << fd << "\n";
//
//    code << "\n";
//
//    for (const auto& fd : function_definitions)
//        code << fd << "\n";
//
//    code << "\n";
//
//    code << "void main() {\n";
//
//    for (const auto& m : main_part)
//        code << m << "\n";
//
//    code << "}\n";

    for (const auto& l : this->code) {
        code << l << "\n";
    }

    GLint result = GL_FALSE;
    int infoLength = 0;

    string codestr = code.str();
    char const * strsrc = codestr.c_str();

    glShaderSource(ID, 1, &strsrc, nullptr);
    glCompileShader(ID);

    glGetShaderiv(ID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &infoLength);

    string segment;
    int i = 1;
    while (getline(code, segment, '\n')) {
        printf("%d | %s\n", i++, segment.c_str());
    }
    printf("\n\n");

    if (result == GL_FALSE) {
        std::vector<char> shaderError(infoLength + 1);
        glGetShaderInfoLog(ID, infoLength, nullptr, &shaderError[0]);
        printf("%d : %s\n", infoLength, &shaderError[0]);
        throw std::runtime_error(&shaderError[0]);
    }
}

GLuint createProgram(GLuint vertex, GLuint fragment) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    GLint result = GL_FALSE;
    int infoLength = 0;

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
    if (infoLength > 1) {
        std::vector<char> programError(infoLength + 1);
        glGetProgramInfoLog(program, infoLength, nullptr, &programError[0]);
        printf("%d : %s\nProgram\n", infoLength, &programError[0]);
        throw std::runtime_error(&programError[0]);
    }

    return program;
}
