#ifndef SHADER_DIR
#define SHADER_DIR "shaders"
#endif

#ifndef ODDITYENGINE_SHADER_H
#define ODDITYENGINE_SHADER_H

#include "GL/glew.h"

#include <string>

namespace OddityEngine {
    namespace Graphics {

        class Shader {
        protected:
            GLuint ID;
            GLuint type;

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
        };

    } // OddityEngine
} // Graphics

#endif //ODDITYENGINE_SHADER_H
