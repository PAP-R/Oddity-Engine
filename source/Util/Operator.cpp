#include "Operator.h"

namespace OddityEngine::Util {
    Vector<GLuint> Operator::add_operation(const std::string &code) {
        auto names = operation_shader.add(code);

        Vector<GLuint> indices;
        for (auto n : names) {
            indices.emplace_back(operation_shader.selector_index(OPERATOR_SELECTOR, n));
        }

        operation_program.recompile();

        return indices;
    }

    void Operator::add_call(size_t step, const OperationCall& call) {
        if (operation_buffers.size() <= step) {
            operation_buffers.resize(step + 1);
        }

        operation_buffers[step].insert_back(call);
    }

    void Operator::add_buffer(GLuint binding, GLuint buffer) {
        other_buffers.emplace_back(binding, buffer);
    }

    void Operator::run(size_t start, size_t count) {
        if (count == 0 || start + count > operation_buffers.size()) {
            count = operation_buffers.size();
        }
        else {
            count += start;
        }

        glUseProgram(operation_program);

        for (auto b : other_buffers) {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, b.first, b.second);
        }

        for (size_t i = start; i < count; i++) {
            operation_buffers[i].bind_base(Graphics::OPERATION);
            glDispatchCompute(operation_buffers[i].get_count(), 1, 1);
            glMemoryBarrier(GL_ALL_BARRIER_BITS);
            glFinish();
        }
    }
}