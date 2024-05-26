#include "Shader.h"

#include "GL/glew.h"

#include <sstream>
#include "Util/File.h"

#include "fmt/core.h"

#include <algorithm>
#include <vector>

#include "boost/xpressive/xpressive.hpp"

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

        Shader::Shader(GLuint type, const std::string &path) : Shader(type) {
            Debug::message("Started loading {}", path);

            paths.clear();
            uniforms.clear();

            name = path;

            add(read_shader(path));

            Debug::message("Finished loading {}", path);

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

        GLuint Shader::get_ID() {
            if (outdated) {
                recompile();
            }

            return ID;
        }

        Shader::operator GLuint() {
            return get_ID();
        }

        std::string Shader::add_element(ShaderElement element) {
            outdated = true;
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

            boost::xpressive::smatch match;

            while(std::getline(stream, line, '\n')) {
                if (boost::xpressive::regex_search(line, match, boost::xpressive::sregex::compile(R"((?<=#include\s\<).+(?=\>))"))) {
                    std::string sub_path = match[0];
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

                    if (boost::xpressive::regex_search(cell, match, boost::xpressive::sregex::compile(R"(\s*#selector\s*\(\s*.+?(?=\s*\)))"))) {
                        selector = true;
                        select_by = boost::xpressive::regex_replace(match.str(), boost::xpressive::sregex::compile(R"(\s*#selector\s*\(\s*)"), "");
                        cell = boost::xpressive::regex_replace(cell, boost::xpressive::sregex::compile(R"(\s*#selector\s*\(\s*.+?\s*\)\s+)"), "");
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
//                        Debug::print("{}", current);

                        size_t debug_number = 0;
                        Debug::print(" {} ", debug_number++);

                        if (!boost::xpressive::regex_search(current, match, boost::xpressive::sregex::compile(R"(^(?:\s*(layout\(.*?\))?\s*((?:\w+\s*)+)?|#define)\s+(?:(\w+):)?(\w+)\s*(?:\(\s*([\s\S]+?)\s*\)|\{\s*([\s\S]+?)\s*\})?)"))) {
                            Debug::message("Some shader type things couldn't be found at:\n{}", current);
                        }

                        Debug::print(" {} ", debug_number++);

                        std::string layout = match[1];
                        Debug::print(" {} ", debug_number++);
                        std::string type = match[2];
                        Debug::print(" {} ", debug_number++);
                        std::string name = match[4];
                        Debug::print(" {} ", debug_number++);
                        std::string parameter_string = match[5] + match[6];
                        Debug::print(" {} ", debug_number++);

                        if (match[3]) {
                            Debug::message("Found selector [{}]", match[3].str());
                            this->selector_elements.add(match[3].str(), name);
                            auto all_selector = selector_elements.get_all();
                            for (auto a : all_selector) {
                                Debug::print("{}:\n", a.first);
                                for (auto b : a.second) {
                                    Debug::print("\t{}\n", b);
                                }
                            }
                            current = boost::xpressive::regex_replace(current, boost::xpressive::sregex::compile(fmt::format("{}:", match[3].str())), "");
                        }

                        Debug::print(" {} ", debug_number++);

                        for (boost::xpressive::sregex_iterator cur(parameter_string.begin(), parameter_string.end(), boost::xpressive::sregex::compile(R"(([\w\s]+))")), end; cur != end; ++cur) {
                            auto single_parameter = boost::xpressive::regex_replace((*cur).str(), boost::xpressive::sregex::compile(R"(^\s*|\s+$|\s+(?=\s))"), "");
                            boost::xpressive::smatch paramatch;
                            if (boost::xpressive::regex_search(single_parameter, paramatch, boost::xpressive::sregex::compile(R"(([\w\s]+)\s+(\w+))"))) {
                                parameter_types.emplace_back(paramatch[1]);
                                parameters.emplace_back(paramatch[2]);
                            }
                        }

                        Debug::print(" {} ", debug_number++);

                        name_list.emplace_back(add_element({name, current, type, layout, selector, parameters, parameter_types, select_by}));

                        Debug::message("\nCreated element {}", name);

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

        void Shader::needed(const std::string& name, Vector<std::string>* available, Vector<std::string>* ordered, const std::string& path) {
            if (std::find(ordered->begin(), ordered->end(), name) != ordered->end()) {
                return;
            }

            auto new_path = fmt::format("{}{}/", path, name);

            auto current = this->elements.get(name)->back();
            available->erase(std::remove(available->begin(), available->end(), name));

            bool not_struct = !current.type.contains("struct");

            auto temp_available = *available;
            for (auto& es : temp_available) {
                if (es.empty()) continue;
                auto el = elements.get(es);

                if (el == nullptr) continue;
                auto& e = el->back();

                if (not_struct && e.type.contains("buffer")) {
                    for (const auto& p : e.parameters) {
                        if (boost::xpressive::regex_search(current.content, boost::xpressive::sregex::compile(fmt::format(R"(\W+{}\W+)", p)))) {
                            needed(es, available, ordered, new_path);
                            break;
                        }
                    }
                }
                else if ((not_struct || e.type.contains("struct")) && boost::xpressive::regex_search(current.content, boost::xpressive::sregex::compile(fmt::format(R"(\W+{}\W+)", es)))) {
                    needed(es, available, ordered, new_path);
                }
            }

            size_t offset = 0;

            for (size_t i = ordered->size(); i > 0; i--) {
                auto es = (*ordered)[i - 1];
                auto el = elements.get(es);

                if (el == nullptr) continue;
                auto& e = el->back();

                if (not_struct && e.type.contains("buffer")) {
                    for (auto p : e.parameters) {
                        if (boost::xpressive::regex_search(current.content, boost::xpressive::sregex::compile(fmt::format(R"(\W+{}\W+)", p)))) {
                            offset = i;
                            break;
                        }
                    }
                }
                else if ((not_struct || e.type.contains("struct")) && boost::xpressive::regex_search(current.content, boost::xpressive::sregex::compile(fmt::format(R"(\W+{}\W+)", es)))) {
                    offset = i;
                    break;
                }

                if (offset != 0) break;
            }


//            Debug::message("Sorting {}{} to {}", path, name, offset);

            ordered->emplace(offset, name);

            if (path.empty()) {
                temp_available = *available;

                for (const auto &c: temp_available) {
                    auto &e = this->elements.get(c)->back();
                    if (!e.layout.empty()) {
//                        Debug::message("Adding possibly unneeded thing: {}", c);
                        needed(c, available, ordered, "other/");
                    }
                    else {
//                        Debug::message("Tossing really unneeded thing: {}", c);
                    }
                }
            }
        }

        void Shader::sort(Vector<std::string>* elements) {
            std::stable_sort(elements->begin(), elements->end(), [&](const std::string& first, const std::string& second){
                auto first_element_ptr = this->elements.get(first);
                auto second_element_ptr = this->elements.get(second);

                if (first_element_ptr == nullptr || second_element_ptr == nullptr) {
                    return false;
                }

                auto first_element = first_element_ptr->back();
                auto second_element = second_element_ptr->back();

                if (first_element.type.contains("buffer")) {
                    for (auto p : first_element.parameters) {
                        if (boost::xpressive::regex_search(second_element.content, boost::xpressive::sregex::compile(fmt::format(R"(\W+{}\W+)", p)))) {
                            Debug::message("[{}] contains [{}]", second, first);
                            return true;
                        }
                    }
                }
                else if (boost::xpressive::regex_search(second_element.content, boost::xpressive::sregex::compile(fmt::format(R"(\W+{}\W+)", first)))) {
                    Debug::message("[{}] contains [{}]", second, first);
                    return true;
                }

                Debug::message("[{}] does not contain [{}]", second, first);
                return false;
            });
        }

        std::string Shader::create_code() {
            std::string shader_code = fmt::format("#version {}\n", VERSION);

            auto elements = this->elements.get_all_paths();

            Vector<std::string> ordered_names;

            for (auto& ep : elements) {
                auto& e = this->elements.get(ep)->back();
                if (e.enum_selector) {
                    auto all_selector = selector_elements.get_all();
                    for (auto a : all_selector) {
                        Debug::print("{}:\n", a.first);
                        for (auto b : a.second) {
                            Debug::print("\t{}\n", b);
                        }
                    }

                    Debug::message("Selector {}", e.name);

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

                        Debug::print("Funcs size {}", funcs->size());

                        for (auto f : *funcs) {
                            Debug::print("{} ", f);
                        }

                        for (size_t i = 0; i < funcs->size(); i++) {
                            if (i == 0) {
                                e.content += fmt::format("\t\tdefault:\n");
                            }
                            else {
                                e.content += fmt::format("\t\tcase {}:\n", i);
                            }

                            e.content += fmt::format("\t\t\t{}{}({});\n\t\t\tbreak;\n", e.type.contains("void") ? "" : "return ", (*funcs)[i], parameters);
                        }
                    }

                    e.content += "\t}\n}\n";
                }

//                size_t offset;
//
//                if (e.type.contains("buffer")) {
//                    offset = 0;
//                    for (size_t i = ordered_names.size(); i > 0; i--) {
//                        if (boost::xpressive::regex_search(e.content, boost::xpressive::sregex::compile("[\[^\w_]]" + ordered_names[i - 1] + "[\[^\w_]]"))) {
//                            offset = i;
//                            break;
//                        }
//                    }
//                }
//                else {
//                    offset = ordered_names.size();
//                    for (size_t i = 0; i < ordered_names.size(); i++) {
//                        if (boost::xpressive::regex_search(this->elements.get(ordered_names[i])->back().content, boost::xpressive::sregex::compile("[\[^\w_]]" + e.name + "[\[^\w_]]"))) {
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

//            sort(&elements);

            for (const auto& c : ordered_names) {
//                Debug::message("Adding Thing [{}] to [{}] code", c, name);
                shader_code += fmt::format("{}\n", this->elements.get(c)->back().content);
            }

            return shader_code;
        }

        GLuint Shader::compile() {
            auto shader_code = create_code();

            Debug::message("Started compiling Shader: {}", name);

            GLuint ID = glCreateShader(type);

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

                std::stringstream shader_stream(shader_code);
                std::string line;
                for (int i = 1; std::getline(shader_stream, line); i++) {
                    fmt::print("{:3d} \t: {}\n", i, line);
                }

                Debug::error("Shader Error on Compilation of {}: {}\n", name, &shaderError[0]);
            }

            Debug::message("Finished compiling Shader: {}\n\n", name);

            Debug::message("1 {} is{} a shader", this->name, (glIsShader(this->ID) ? "" : "n't"));

            return ID;
        }

        GLuint Shader::recompile() {
            Debug::message("2 {} is{} a shader", this->name, (glIsShader(this->ID) ? "" : "n't"));

            auto old_ID = this->ID;

            this->ID = compile();

            outdated = false;

            Debug::message("3 {} is{} a shader", this->name, (glIsShader(this->ID) ? "" : "n't"));

            if (glIsShader(old_ID)) {
                glDeleteShader(old_ID);
            }

            return this->ID;
        }

        GLuint Shader::selector_index(const std::string &selector, const std::string &function) {
            auto selector_functions = selector_elements.get(selector);
            if (selector_functions != nullptr) {
                auto it = std::find(selector_functions->begin(), selector_functions->end(), function);
                if (it != selector_functions->end()) {
                    return it - selector_functions->begin();
                }
                return selector_functions->size();
            }
            return 0;
        }

        std::string Shader::selector_name(const std::string &selector, GLuint index) {
            auto selector_functions = selector_elements.get(selector);
            if (selector_functions != nullptr) {
                if (index < selector_functions->size()) {
                    return selector_functions[index];
                }
            }
            return "";
        }
    } // OddityEngine
} // Graphics