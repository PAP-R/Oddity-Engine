#include "Shader.h"

#include <sstream>
#include "../../Util/file.h"

#include <fmt/core.h>

#include <algorithm>
#include <vector>

namespace OddityEngine {
    namespace Graphics {
        std::vector<std::string> paths;
        bool version_inserted;

        std::string read_shader(const std::string &path) {
            paths.emplace_back(path);

            std::stringstream shader_stream = Util::File::stream(fmt::format("{}/{}",SHADER_DIR, path));
            std::string shader_code;

            for (std::string line; std::getline(shader_stream, line); ) {
                if (line.contains("#version")) {
                    if (version_inserted) {
                        continue;
                    } else {
                        version_inserted = true;
                    }
                }
                else if (line.contains("#include")) {
                    auto first = line.find('<');
                    auto last = line.find('>');
                    std::string sub_path = line.substr(first, last-first);
                    if (std::find(paths.begin(), paths.end(), sub_path) != paths.end()) {
                        throw std::runtime_error(fmt::format("Recursive Inclusion in Shaders {} | {}", path, sub_path));
                    } else {
                        shader_code += read_shader(sub_path);
                    }
                } else {
                    shader_code += line;
                }
            }

            return shader_code;
        }

        Shader::Shader(GLuint type, const std::string &path) : type(type), ID(glCreateShader(type)) {
            paths.clear();
            version_inserted = false;

            std::string shader_code = read_shader(path);

            GLint result = GL_FALSE;

            char const * shader_chars = shader_code.c_str();
            glShaderSource(ID, 1, &shader_chars, nullptr);

            glCompileShader(ID);

            glGetShaderiv(ID, GL_COMPILE_STATUS, &result);

            if (result == GL_FALSE) {
                int info_length;
                glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &info_length);

                std::vector<char> shaderError(info_length + 1);
                glGetShaderInfoLog(ID, info_length, nullptr, &shaderError[0]);
                fmt::print("{} Shader Error : {}\n", info_length, &shaderError[0]);

                std::stringstream shader_stream(shader_code);
                std::string line;
                for (int i = 1; std::getline(shader_stream, line); i++) {
                    fmt::print("{:3d} \t: {}", i, line);
                }

                throw std::runtime_error(&shaderError[0]);
            }

            paths.clear();
        }

        Shader::~Shader() {
            glDeleteShader(ID);
        }

        Shader::operator GLuint() const {
            return ID;
        }
    } // OddityEngine
} // Graphics