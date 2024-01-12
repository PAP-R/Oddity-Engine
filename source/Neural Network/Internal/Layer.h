#ifndef LAYER_H
#define LAYER_H

#include <Util/Matrix.h>
#include <Util/Vector.h>
#include <Util/Vector_overload.h>

namespace OddityEngine::NeuralNetwork {
    class Layer {
    protected:
        inline static double default_weight = 1;
        inline static double default_bias = 0;

        size_t _input_size;
        size_t _output_size;

        Matrix<> weights;
        Vector<> bias;
        Vector<Vector<size_t>> functions;
        Vector<> input;
        Vector<> output;

    public:
        Layer(const size_t input_count = 1, const size_t output_count = 1) : _input_size(input_count), _output_size(output_count), weights(output_count, input_count, default_weight), bias(output_count, default_bias), functions(output_count), input(input_count, 0), output(output_count, 0) {}
        Layer(const Matrix<>& weights, const Vector<>& bias, const Vector<Vector<size_t>>& functions) : _input_size(weights.columns()), _output_size(weights.rows()), weights(weights), bias(bias), functions(functions), input(weights.columns(), 0), output(weights.rows(), 0) {}
        virtual ~Layer() = default;

        virtual void resize_input(const size_t size) {
            weights.resize_columns(size);
            input.resize(size);
            _input_size = size;
        }

        virtual void resize_output(const size_t size) {
            weights.resize_rows(size);
            bias.resize(size);
            functions.resize(size);
            output.resize(size);
            _output_size = size;
        }

        [[nodiscard]] size_t input_size() const {
            return _input_size;
        }

        [[nodiscard]] size_t output_size() const {
            return _output_size;
        }

        virtual void evolve(double rate) = 0;
        virtual Vector<double> forward(const Vector<double>& input) = 0;
        virtual Vector<double> backward(const Vector<double>& output_gradient, double learning_rate) = 0;

        friend std::ostream& operator << (std::ostream& os, const Layer& layer);
        friend std::ostream& operator << (std::ostream& os, const Layer* layer);
    };

    inline std::ostream& operator << (std::ostream& os, const Layer& layer) {
        os << layer.weights << "\n" << layer.bias << "\n" << layer.functions;

        return os;
    }

    inline std::ostream& operator << (std::ostream& os, const Layer* layer) {
        os << *layer;

        return os;
    }
}


#endif //LAYER_H
