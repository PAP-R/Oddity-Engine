#ifndef ODDITYENGINE_PROGRAM_H
#define ODDITYENGINE_PROGRAM_H

#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include <string>
#include <vector>

#include "Util/Value.h"

namespace OddityEngine {
    namespace Graphics {

        class Program {
        protected:
            GLuint ID;

            std::vector<Util::ValueInterface*> values;

            size_t add_value_interface(Util::ValueInterface* value_interface);

        public:
            Program() = default;
            /**
             * Creates an Opengl program from a vertex and a fragment shader
             * @param vertex vertex shader
             * @param fragment fragment shader
             */
            Program(std::initializer_list<GLuint> shaders);
            // Program(std::string vertex, std::string fragment);
            ~Program();

            /***
             * Finds uniform location in shader
             * @param name variable name
             * @return location
             */
            GLint uniform_location(const std::string& name);

            template<typename T>
            size_t add_value(T value) {
                return add_value_interface(new Util::Value(value));
            }

            void apply();

            GLuint get_ID() const;

            /**
             * Inserts ID for program
             * @return program ID
             */
            operator GLuint() const;
        };

    } // OddityEngine
} // Graphics

#endif //ODDITYENGINE_PROGRAM_H
