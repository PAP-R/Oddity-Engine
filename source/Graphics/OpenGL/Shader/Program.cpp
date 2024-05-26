#include "Program.h"

#include <vector>

#include "Shader.h"
#include "fmt/core.h"
#include "Util/Debug.h"

namespace OddityEngine {
    namespace Graphics {
        Program::Program(std::initializer_list<Shader> shaders) : shaders{shaders} {
            this->ID = compile();
        }

        // Program::Program(std::string vertex, std::string fragment) : Program(Shader(GL_VERTEX_SHADER, vertex), Shader(GL_FRAGMENT_SHADER, fragment)) {
        // }

        Program::~Program() {
            glDeleteProgram(ID);
        }

        GLuint Program::get_ID() {
            if (outdated) {
                recompile();
            }

            return ID;
        }

        Program::operator GLuint() {
            return get_ID();
        }

        GLint Program::uniform_location(const std::string& name) const {
            return glGetUniformLocation(ID, name.c_str());
        }

        GLuint Program::compile() {
            GLuint ID = glCreateProgram();

            for (auto& s : shaders) {
                glAttachShader(ID, s);
            }

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
                Debug::error("{} Shader Program Error : {}\n", info_length, &program_error[0]);
            }

            for (auto& s : shaders) {
                glDetachShader(ID, s);
            }

            return ID;
        }

        GLuint Program::recompile() {
            auto old_ID = this->ID;

            this->ID = compile();

            outdated = false;

            if (glIsProgram(old_ID)) {
                glDeleteProgram(old_ID);
            }

            Debug::message("Old ID {}\tNew ID {}", old_ID, this->ID);

            return this->ID;
        }

        void Program::compile_shaders() {
        }
    } // OddityEngine
} // Graphics