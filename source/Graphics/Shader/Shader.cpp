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

        Shader::Shader(GLuint type) : type(type), ID(glCreateShader(type)) {}

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

        std::string Shader::add_element(ShaderElement element) {
            elements.add(element.name, element);
            return element.name;
        }


        Vector<std::string> Shader::add(const std::string& string) {
            Vector<std::string> name_list;
            size_t roundbracket = 0, swirlybracket = 0, squarebracket = 0, index = 0, next = 0;

            std::stringstream stream(string);
            std::string line;
            std::string current;
            Vector<std::string> parameters;
            Vector<std::string> parameter_types;
            bool selector = false;
            std::string select_by;

            while(std::getline(stream, line, '\n')) {
                if (line.contains("#include")) {
                    auto first = line.find('<') + 1;
                    auto last = line.find('>');
                    std::string sub_path = line.substr(first, last - first);
                    if (std::find(paths.begin(), paths.end(), sub_path) == paths.end()) {
                        name_list += add(read_shader(sub_path));
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
                        selector = true;
                        size_t first = cell.find_first_of('(') + 1;
                        size_t last = cell.find_first_of(')', first);
                        select_by = cell.substr(first, last - first);
                        cell.erase(0, last + 2);
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

                        if (current[after_name] == '(') {
                            size_t after_parameters = current.find_first_of(')') - 1;
                            std::stringstream parameter_string(current.substr(after_name + 1, after_parameters - after_name));
                            std::string single_parameter;
                            while (std::getline(parameter_string, single_parameter, ',')) {
                                size_t offset_front = 0;
                                while (single_parameter[offset_front] == ' ') {
                                    offset_front++;
                                }

                                size_t offset_back = single_parameter.size() - 1;
                                while (single_parameter[offset_back] == ' ') {
                                    offset_back--;
                                }
                                size_t space = single_parameter.find(' ', offset_front);

                                parameter_types.emplace_back(single_parameter.substr(offset_front, space - offset_front));
                                parameters.emplace_back(single_parameter.substr(space, offset_back - space + 1));
                            }
                        }

                        size_t before_name = current.find_last_of(' ', after_name - 2);
                        if(before_name == current.npos) {
                            before_name = 0;
                        }
                        before_name += 1;

                        std::string name = current.substr(before_name, after_name - before_name);
                        name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
                        std::string type = current.substr(0, before_name);

                        if (name.contains(':')) {
                            std::string selector_name = name.substr(0, name.find_first_of(':'));
                            current.erase(current.find(selector_name), selector_name.size() + 1);
                            name.erase(0, selector_name.size() + 1);
                            selector_elements.add(selector_name, name);
                        }

                        name_list.emplace_back(add_element({name, current, type, selector, parameters, parameter_types, select_by}));

                        current.clear();
                        selector = false;
                        parameters.clear();
                        parameter_types.clear();
                    }
                }
            }

            return name_list;
        }

        std::string Shader::compile() {
            std::string shader_code = fmt::format("#version {}\n", VERSION);

            Vector<std::string> ordered_names;

            auto elements = this->elements.get_all_paths();

            for (auto& ep : elements) {
                auto& e = this->elements.get(ep)->back();
                if (e.enum_selector) {
                    e.content = fmt::format("{} {}(", e.type, e.name);
                    std::string parameters;
                    for (size_t p = 0; p < e.parameters.size(); p++) {
                        if (p != 0) {
                            parameters += ", ";
                            e.content += ", ";
                        }

                        parameters += e.parameters[p];
                        e.content += fmt::format("{} {}", e.parameter_types[p], e.parameters[p]);
                    }

                    e.content += fmt::format(") {{\n\tswitch({}) {{\n", e.select_by);

                    auto funcs = selector_elements.get(e.name);
                    if (funcs != nullptr) {
                        for (size_t i = 0; i < funcs->size(); i++) {
                            if (i == 0) {
                                e.content += fmt::format("\t\tdefault:\n");
                            }
                            else {
                                e.content += fmt::format("\t\tcase {}:\n", i);
                            }

                            e.content += fmt::format("\t\t\treturn {}({});\n\t\t\tbreak;\n", (*funcs)[i], parameters);
                        }
                    }

                    e.content += "\t}\n}\n";
                }

                size_t offset;

                if (e.type.contains("buffer")) {
                    offset = 0;
                    for (size_t i = ordered_names.size(); i > 0; i--) {
                        if (e.content.contains(ordered_names[i - 1])) {
                            offset = i;
                            break;
                        }
                    }
                }
                else {
                    offset = ordered_names.size();
                    for (size_t i = 0; i < ordered_names.size(); i++) {
                        if (this->elements.get(ordered_names[i])->back().content.contains(e.name)) {
                            offset = i;
                            break;
                        }
                    }
                }


                ordered_names.emplace(offset, e.name);
            }

            for (const auto& c : ordered_names) {
                shader_code += fmt::format("{}\n", this->elements.get(c)->back().content);
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