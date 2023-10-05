#include "Shader.h"

#include <sstream>
#include <fstream>
#include <vector>

#include "fmt/core.h"

namespace OddityEngine::Graphics::Shader {
    Shader::Shader(GLuint type, const std::string &path) : type(type), ID(glCreateShader(type)), path(path) {
        std::stringstream shader_code;

        std::ifstream shader_stream(path, std::ios::in);
        if (shader_stream.is_open()) {
            shader_code << shader_stream.rdbuf();
            shader_stream.close();
        }
        else {
            throw std::runtime_error(fmt::format("Failed to read shader at {}", path));
        }

        this->code = shader_code.str();
    }

    Shader::~Shader() {
        glDeleteShader(this->ID);
    }

    void Shader::compile() {
        GLint result = GL_FALSE;
        int infolength = 0;

        char const * codestring = this->code.c_str();

        glShaderSource(this->ID, 1, &codestring, nullptr);
        glCompileShader(ID);

        glGetShaderiv(ID, GL_COMPILE_STATUS, &result);
        glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &infolength);

        if (result == GL_FALSE) {
            std::vector<char> shaderError(infolength + 1);
            glGetShaderInfoLog(ID, infolength, nullptr, &shaderError[0]);
            fmt::print("{} Shader Error : {}\n", infolength, &shaderError[0]);

            std::stringstream code(this->code);
            std::string line;
            int linenumber = 1;

            fmt::print("{} :\n", this->path);

            while (std::getline(code, line)) {
                fmt::print("{:3d} : {}\n", linenumber++, line);
            }

            throw std::runtime_error(&shaderError[0]);
        }
    }

    Shader::operator unsigned int() const {
        return this->ID;
    }


    GLuint create_program(GLuint vertex, GLuint fragment) {
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
};
