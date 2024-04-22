#ifndef SHADER_DIR
#define SHADER_DIR "shaders"
#endif

#ifndef ODDITYENGINE_SHADER_H
#define ODDITYENGINE_SHADER_H

#include "GL/glew.h"
#include "Util/Vector.h"

#include <string>

namespace OddityEngine {
    namespace Graphics {
        struct ShaderElement {
            std::string name;
            std::string content;
        };

        class Shader {
        protected:
            GLuint ID = 0;
            GLuint type = 0;
            Vector<ShaderElement> elements;

        public:
            Shader() = default;
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

            static Vector<ShaderElement> parse(const std::string& string);
        };

    } // OddityEngine
} // Graphics

#endif //ODDITYENGINE_SHADER_H
