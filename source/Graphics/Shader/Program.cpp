#include "Program.h"

#include <vector>

#include "Shader.h"
#include "fmt/core.h"

namespace OddityEngine {
    namespace Graphics {
        size_t Program::add_value_interface(Util::ValueInterface* value_interface) {
            values.emplace_back(value_interface);
            return values.size() - 1;
        }

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

        // Program::Program(std::string vertex, std::string fragment) : Program(Shader(GL_VERTEX_SHADER, vertex), Shader(GL_FRAGMENT_SHADER, fragment)) {
        // }

        Program::~Program() {
            for (auto v : values) {
                delete(v);
            }
            glDeleteProgram(ID);
        }

        void Program::apply() {
            glUseProgram(*this);
            for (auto v : values) {
                if (v->show) {
                    v->apply();
                }
            }
        }

        Program::operator GLuint() const {
            return ID;
        }

        GLint Program::uniform_location(const std::string& name) {
            return glGetUniformLocation(ID, name.c_str());
        }
    } // OddityEngine
} // Graphics