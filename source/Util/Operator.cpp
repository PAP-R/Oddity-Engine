#include "Operator.h"

namespace OddityEngine::Util {
    Vector<std::pair<std::string, GLuint>> Operator::add_operation(const std::string &code) {
        auto names = operation_program.shaders.front().add(code);

        Vector<std::pair<std::string, GLuint>> indices;
        for (const auto& n : names) {
            auto index = operation_program.shaders.front().selector_index(OPERATOR_SELECTOR, n);
            indices.emplace_back(n, index);
            operation_indices.add(n, index);
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

    void Operator::add_buffer(GLuint* buffer, GLuint binding) {
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

        parameter_buffer.bind_base(Graphics::PARAMETER);

        for (auto b : other_buffers) {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, b.first, *b.second);
        }

        for (size_t i = start; i < count; i++) {
            operation_buffers[i].bind_base(Graphics::OPERATION);
            glDispatchCompute(operation_buffers[i].get_count(), 1, 1);
            glMemoryBarrier(GL_ALL_BARRIER_BITS);
            glFinish();
        }
    }

    GLuint Operator::get_opertation(const std::string &name) {
        auto indices = operation_indices.get(name);
        if (indices != nullptr) {
            return indices->back();
        }
        return 0;
    }
}