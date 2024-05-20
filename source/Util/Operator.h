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
        Graphics::Shader operation_shader = Graphics::Shader(GL_COMPUTE_SHADER, "operator.comp");
        Graphics::Program operation_program = Graphics::Program({operation_shader});
        Util::Trie<GLuint> operation_indices;
        Vector<Graphics::Buffer<OperationCall>> operation_buffers;
        Vector<std::pair<GLuint, GLuint>> other_buffers;

    public:
        Vector<GLuint> add_operation(const std::string& code);
        void add_call(size_t step, const OperationCall& call);

        void add_buffer(GLuint binding, GLuint buffer);

        void run(size_t start = 0, size_t count = 0);
    };
}

#endif //ODDITYENGINE_OPERATOR_H
