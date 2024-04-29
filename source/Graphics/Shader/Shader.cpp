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

            bool has_parameters = false;
            char parameter_delimiter;

            std::smatch m;

            while(std::getline(stream, line, '\n')) {
                if (std::regex_search(line, m, std::regex(R"((?<=#include\s\<).+(?=\>))"))) {
                    std::string sub_path = m.str();
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

                    if (std::regex_search(cell, m, std::regex(R"((?<=\s*#selector\s*\(\s*).+?(?=\s*\)))"))) {
                        selector = true;
                        select_by = m.str();
                        std::regex_search(cell, m, std::regex(R"((?<=\s*#selector\s*\(\s*.+?\s*\)\s+)[\s\S]+)"));
                        cell = m.str();
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
                        std::string name = std::regex_replace(current, std::regex(R"(\s*(layout\(.*\))*( *\w+ +)+(?=\w+ *[=\(\{\[;])|(?<=\w+) *(=|\(|\{|\[)[\s\S]*|\/\/.*)"), "");
                        std::string type = std::regex_replace(current, std::regex(R"( *\w+ *(=|\(|\{)[\s\S]*|\/\/.*)"), "");
                        std::string parameter_string = std::regex_replace(current, std::regex(R"(^.*?[\(\{]\s*|\s*?[\)\}][\s\S]*)"), "");

                        if (std::regex_search(name, m, std::regex(R"(\w+(?=:))"))) {
                            name = std::regex_replace(name, std::regex(R"(\w+:)"), "");
                            selector_elements.add(m.str(), name);
                        }

                        while (std::regex_search(parameter_string, m, std::regex(R"((?<=\s*)(\w+ *)+(?=\s*))"))) {
                            auto single_parameter = m.str();
                            std::smatch paramatch;
                            std::regex_search(single_parameter, paramatch, std::regex(R"(.+(?=\s+\w+$))"));
                            parameter_types.emplace_back(paramatch.str());
                            std::regex_search(single_parameter, paramatch, std::regex(R"(\w+$)"));
                            parameters.emplace_back(paramatch.str());
                        }

                        name_list.emplace_back(add_element({name, current, type, selector, parameters, parameter_types, select_by}));

                        current.clear();
                        selector = false;
                        has_parameters = false;
                        parameters.clear();
                        parameter_types.clear();
                    }
                }
            }

            return name_list;
        }

        void Shader::needed(const std::string& name, Vector<std::string>* available, Vector<std::string>* ordered) {
            if (std::find(ordered->begin(), ordered->end(), name) != ordered->end()) {
                return;
            }

            auto current = this->elements.get(name)->back();

            size_t offset = 0;

            for (size_t i = ordered->size(); i > 0; i--) {
                auto es = (*ordered)[i - 1];
                auto el = elements.get(es);

                if (el == nullptr) continue;
                auto& e = el->back();

                if (e.type.contains("buffer")) {
                    for (auto p : e.parameters) {
                        if (std::regex_search(current.content, std::regex(fmt::format(R"(\W+{}\W+)", p)))) {
                            offset = i;
                            break;
                        }
                    }
                }
                else if (std::regex_search(current.content, std::regex(fmt::format(R"(\W+{}\W+)", es)))) {
                    offset = i;
                    break;
                }

                if (offset != 0) break;
            }

            ordered->emplace(offset, name);
            available->erase(std::remove(available->begin(), available->end(), name));

            auto temp_available = *available;
            for (auto& es : temp_available) {
                if (es.empty()) continue;
                auto el = elements.get(es);

                if (el == nullptr) continue;
                auto& e = el->back();

                if (e.type.contains("buffer")) {
                    for (const auto& p : e.parameters) {
                        if (std::regex_search(current.content, std::regex(fmt::format(R"(\W+{}\W+)", p)))) {
                            needed(es, available, ordered);
                            break;
                        }
                    }
                }
                else if (std::regex_search(current.content, std::regex(fmt::format(R"(\W+{}\W+)", es)))) {
                    needed(es, available, ordered);
                }
            }
        }

        std::string Shader::compile() {
            std::string shader_code = fmt::format("#version {}\n", VERSION);

            auto elements = this->elements.get_all_paths();

            Vector<std::string> ordered_names;

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

//                size_t offset;
//
//                if (e.type.contains("buffer")) {
//                    offset = 0;
//                    for (size_t i = ordered_names.size(); i > 0; i--) {
//                        if (std::regex_search(e.content, std::regex("[\[^\w_]]" + ordered_names[i - 1] + "[\[^\w_]]"))) {
//                            offset = i;
//                            break;
//                        }
//                    }
//                }
//                else {
//                    offset = ordered_names.size();
//                    for (size_t i = 0; i < ordered_names.size(); i++) {
//                        if (std::regex_search(this->elements.get(ordered_names[i])->back().content, std::regex("[\[^\w_]]" + e.name + "[\[^\w_]]"))) {
//                            offset = i;
//                            break;
//                        }
//                    }
//                }
//
//
//                ordered_names.emplace(offset, e.name);
            }

            auto all_elements = this->elements.get_all();

            needed("main", &elements, &ordered_names);

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