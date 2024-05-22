#ifndef ODDITYENGINE_OPERATOR_H
#define ODDITYENGINE_OPERATOR_H

#include "GL/glew.h"

#include "Graphics/Shader/Program.h"
#include "Graphics/Buffer/Buffer.h"

#include "Trie.h"

#define OPERATOR_SELECTOR "operation_selector"

namespace OddityEngine::Util {


    struct alignas(8) OperationCall {
        GLuint operation;
        GLuint parameter_start;
        GLuint parameter_count;
    };

    class Operator {
    protected:
        Graphics::Program operation_program = Graphics::Program({Graphics::Shader(GL_COMPUTE_SHADER, "operator.comp")});
        Util::Trie<GLuint> operation_indices;
        Vector<Graphics::Buffer<OperationCall>> operation_buffers;
        Graphics::Buffer<GLfloat> parameter_buffer = Graphics::Buffer<GLfloat>();
        Vector<std::pair<GLuint, GLuint*>> other_buffers = Vector<std::pair<GLuint, GLuint*>>();

    public:
        Vector<std::pair<std::string, GLuint>> add_operation(const std::string& code);
        void add_call(size_t step, const OperationCall& call);

        GLuint get_opertation(const std::string& name);

        void add_buffer(GLuint* buffer, GLuint binding);

        void run(size_t start = 0, size_t count = 0);
    };
}

#endif //ODDITYENGINE_OPERATOR_H
