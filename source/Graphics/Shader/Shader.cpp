#include "Shader.h"

#include "GL/glew.h"

#include <sstream>
#include "Util/File.h"

#include "fmt/core.h"

#include <algorithm>
#include <vector>

#include "Util/Debug.h"

namespace OddityEngine {
    namespace Graphics {
        std::vector<std::string> paths;
        std::vector<std::string> uniforms;

        std::string Shader::read_shader(const std::string &path) {
            paths.emplace_back(path);

            std::stringstream shader_stream = Util::File::stream(fmt::format("{}/{}",SHADER_DIR, path));
            std::string shader_code;

            for (std::string line; std::getline(shader_stream, line); ) {
                size_t first_char =  line.find_first_not_of(" \t\n");
                if (first_char >= line.size()) {
                    continue;
                }
                if (line.contains("  ")) {
                    line.erase(std::unique(line.begin() + first_char, line.end(), [](unsigned char a, unsigned char b){return std::isspace(a) && std::isspace(b);}), line.end());
                }
                if (line.contains("#version")) {
                    continue;
                }
                if (line.rfind("#include", first_char) != std::string::npos) {
                    auto first = line.find('<') + 1;
                    auto last = line.find('>');
                    std::string sub_path = line.substr(first, last-first);
                    if (std::find(paths.begin(), paths.end(), sub_path) == paths.end()) {
                        shader_code += read_shader(sub_path) + '\n';
                    }
                }
                else if (line.rfind("uniform", first_char) != std::string::npos) {
                    std::string uniform_line = line.substr(first_char, line.find_last_not_of(" \t\n") + 1);
                    if (std::find(uniforms.begin(), uniforms.end(), uniform_line) == uniforms.end()) {
                        shader_code += uniform_line + '\n';
                        uniforms.emplace_back(uniform_line);
                    }
                }
                else {
                    shader_code += line + '\n';
                }
            }

            return shader_code;
        }

        Shader::Shader(GLuint type) : type(type) {}

        Shader::Shader(GLuint type, const std::string &path) : type(type), ID(glCreateShader(type)) {
            paths.clear();
            uniforms.clear();

            add(read_shader(path));

            compile();

            // fmt::print("\t{} :\n", path);
            // std::stringstream shader_stream(shader_code);
            // std::string line;
            // for (int i = 1; std::getline(shader_stream, line); i++) {
            //     fmt::print("{:3d} \t: {}\n", i, line);
            // }
            // fmt::print("\n");


            paths.clear();
        }

        Shader::~Shader() {
            glDeleteShader(ID);
        }

        Shader::operator GLuint() const {
            return ID;
        }

        GLuint Shader::get_ID() const {
            return ID;
        }

        size_t Shader::add_element(const std::string& name, const std::string& content, const std::string& type, const Vector<std::string>& parameters, const Vector<std::string>& parameter_types) {
            size_t offset = elements.size();

            for (size_t i = 0; i < elements.size(); i++) {
                if (elements[i].content.contains(fmt::format(" {}", name))) {
                    offset = i;
                    break;
                }
            }

            elements.emplace(offset, name, content, type, parameters, parameter_types);

            return offset;
        }


        Vector<ShaderElement> Shader::add(const std::string& string) {
            size_t roundbracket = 0, swirlybracket = 0, squarebracket = 0, index = 0, next = 0;

            std::stringstream stream(string);
            std::string line;
            std::string current;

            while(std::getline(stream, line, '\n')) {
                if (line.contains("#include")) {
                    auto first = line.find('<') + 1;
                    auto last = line.find('>');
                    std::string sub_path = line.substr(first, last - first);
                    if (std::find(paths.begin(), paths.end(), sub_path) == paths.end()) {
                        add(read_shader(sub_path));
                    }
                    continue;
                }

                std::stringstream linestream(line);
                std::string cell;
                size_t semicount = std::ranges::count(line, ';');
                for(size_t cellindex = 0; std::getline(linestream, cell, ';'); cellindex++) {
                    if (cell.contains("//")) {
                        break;
                    }

                    if (cell.contains(SELECTOR)) {
                        auto selector = cell.find(SELECTOR);
                        auto first = cell.find_first_of('(', selector) + 1;
                        auto last = cell.find_first_of(')', first);
                        auto selector_name = cell.substr(first, last - first);
                    }

                    current += cell;
                    if (cellindex < semicount) {
                        current += ";";
                    }
                    current += "\n";


                    roundbracket += std::ranges::count(cell, '(');
                    roundbracket -= std::ranges::count(cell, ')');
                    swirlybracket += std::ranges::count(cell, '{');
                    swirlybracket -= std::ranges::count(cell, '}');
                    squarebracket += std::ranges::count(cell, '[');
                    squarebracket -= std::ranges::count(cell, ']');

                    if (roundbracket == 0 && swirlybracket == 0 && squarebracket == 0) {
                        size_t start = 0;
                        if (current.contains("layout")) {
                            start = current.find_first_of(')');
                        }

                        size_t after_name = current.find_first_of("({[=;", start);
                        if (after_name == current.npos) {
                            continue;
                        }
                        size_t before_name = current.find_last_of(' ', after_name - 2);
                        if(before_name == current.npos) {
                            before_name = 0;
                        }

                        std::string name = current.substr(before_name + 1, after_name - before_name);
                        std::string type = current.substr(0, before_name - 1);

                        if (name.contains(':')) {
                            std::string selector_name = name.substr(0, name.find_first_of(':') - 1);
                            current.erase(current.find(selector_name), selector_name.size() + 1);
                            selector_elements.add(selector_name, name);
                        }

                        add_element(name, current, type);

                        current.clear();
                    }
                }
            }

            return elements;
        }

        std::string Shader::compile() {
            std::string shader_code = fmt::format("#version {}\n", VERSION);

            for (const auto& e : elements) {
                shader_code += e.content;
                shader_code += '\n';
            }

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
                fmt::print("Shader Error: {}\n", info_length, &shaderError[0]);

                std::stringstream shader_stream(shader_code);
                std::string line;
                for (int i = 1; std::getline(shader_stream, line); i++) {
                    fmt::print("{:3d} \t: {}\n", i, line);
                }

                Debug::error(&shaderError[0]);
            }

            return shader_code;
        }
    } // OddityEngine
} // Graphics