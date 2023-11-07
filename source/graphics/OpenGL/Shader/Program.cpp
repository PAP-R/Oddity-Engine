#include "Program.h"

#include <vector>
#include "fmt/core.h"

namespace OddityEngine {
    namespace Graphics {
        Program::Program(GLuint vertex, GLuint fragment) {
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);

            GLint result = GL_FALSE;
            int info_length = 0;

            glGetProgramiv(ID, GL_LINK_STATUS, &result);
            glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &info_length);

            if (result == GL_FALSE) {
                int info_length;
                glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &info_length);

                std::vector<char> program_error(info_length + 1);
                glGetProgramInfoLog(ID, info_length, nullptr, &program_error[0]);
                fmt::print("{} Shader Program Error : {}\n", info_length, &program_error[0]);
                throw std::runtime_error(&program_error[0]);
            }
        }

        Program::~Program() {
            glDeleteProgram(ID);
        }

        Program::operator GLuint() const {
            return ID;
        }

        GLint Program::uniform_location(const std::string& name) {
            return glGetUniformLocation(ID, name.c_str());
        }
    } // OddityEngine
} // Graphics