#ifndef SHADER_DIR
#define SHADER_DIR "shaders"
#endif

#ifndef ODDITYENGINE_SHADER_H
#define ODDITYENGINE_SHADER_H

#include "GL/glew.h"
#include <string>

#include "Util/Vector.h"
#include <Util/Trie.h>

#define VERSION "450 core"
#define SELECTOR "#selector"

namespace OddityEngine {
    namespace Graphics {
        struct ShaderElement {
            std::string name;
            std::string content;
            std::string type;
            std::string layout;
            bool enum_selector;
            Vector<std::string> parameters;
            Vector<std::string> parameter_types;
            std::string select_by;
        };

        class Shader {
        protected:
            std::string name;
            GLuint ID = 0;
            GLuint type = 0;
            Util::Trie<ShaderElement> elements;
            Util::Trie<std::string> selector_elements;

            bool outdated = true;

        public:
            Shader() = default;

            /**
             * Creates and compiles an Opengl shader from a source file with #include combinations
             * @param type shader type
             */
            Shader(GLuint type);

            /**
             * Creates and compiles an Opengl shader from a source file with #include combinations
             * @param type shader type
             * @param path Path to the file from SHADER_DIR
             */
            Shader(GLuint type, const std::string& path);

            ~Shader();

            GLuint get_ID();

            /**
             * Inserts ID for shader
             * @return shader ID
             */
            operator GLuint();

            static std::string read_shader(const std::string& path);

            std::string add_element(ShaderElement element);
            Vector<std::string> add(const std::string& string);

            GLuint compile();
            GLuint recompile();

            void needed(const std::string& name, Vector <std::string>* available, Vector <std::string>* ordered, const std::string& path = "");

            void sort(Vector <std::string> *elements);

            GLuint selector_index(const std::string& selector, const std::string& function);
            std::string selector_name(const std::string& selector, GLuint index);

            std::string create_code();
        };

    } // OddityEngine
} // Graphics

#endif //ODDITYENGINE_SHADER_H
