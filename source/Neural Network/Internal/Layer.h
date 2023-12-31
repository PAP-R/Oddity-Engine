#ifndef LAYER_H
#define LAYER_H

#include <Math/Matrix.h>
#include <Math/Vector.h>

namespace OddityEngine::NeuralNetwork {
    class Layer {
    protected:
        inline static double default_weight = 1;
        inline static double default_bias = 0;
        inline static double(*default_function)(double) = [](const double x){return x;};

        size_t _input_size;
        size_t _output_size;

        Matrix<> weights;
        Vector<> bias;
        Vector<double(*)(double)> functions;
        Vector<> input;
        Vector<> output;

    public:
        Layer(const size_t input_count, const size_t output_count) : _input_size(input_count), _output_size(output_count), weights(output_count, input_count, default_weight), bias(output_count, default_bias), functions(output_count, [](const double x){return x;}, true), input(input_count), output(output_count) {}
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

        virtual Layer* copy() = 0;

        virtual void evolve(double rate) = 0;
        virtual Vector<double> forward(Vector<double> input) = 0;
        virtual Vector<double> backward(Vector<double> output_gradient, float learning_rate) = 0;

        friend std::ostream& operator << (std::ostream& os, const Layer& layer);
        friend std::ostream& operator << (std::ostream& os, const Layer* layer);
    };

    inline std::ostream& operator << (std::ostream& os, const Layer& layer) {
        os << layer.input_size() << " -> " << layer.output_size() << "\n" << layer.weights << "\n\t + \n" << layer.bias;

        return os;
    }

    inline std::ostream& operator << (std::ostream& os, const Layer* layer) {
        os << *layer;

        return os;
    }
}


#endif //LAYER_H
