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
#define SELECTOR "function"

namespace OddityEngine {
    namespace Graphics {
        struct ShaderElement {
            std::string name;
            std::string content;
            std::string type;
            Vector<std::string> parameters;
            Vector<std::string> parameter_types;
            bool enum_selector;
        };

        class Shader {
        protected:
            GLuint ID = 0;
            GLuint type = 0;
            Vector<ShaderElement> elements;
            Util::Trie<std::string> selector_elements;

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

            GLuint get_ID() const;
            /**
             * Inserts ID for shader
             * @return shader ID
             */
            operator GLuint() const;

            static std::string read_shader(const std::string& path);

            size_t add_element(const std::string& name, const std::string& content, const std::string& type = "", const Vector<std::string>& parameters = Vector<std::string>(), const Vector<std::string>& parameter_types = Vector<std::string>());
            Vector<ShaderElement> add(const std::string& string);

            std::string compile();
        };

    } // OddityEngine
} // Graphics

#endif //ODDITYENGINE_SHADER_H
