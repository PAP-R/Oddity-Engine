#ifndef LAYER_H
#define LAYER_H

#include <Math/Matrix.h>
#include <Math/Vector.h>

namespace OddityEngine::NeuralNetwork {
    class Layer {
    protected:
        Math::Matrix<float> weights;
        Math::Vector<> bias;
        Math::Vector<> input;
        Math::Vector<> output;

    public:
        virtual ~Layer() = default;

        virtual Math::Vector<> forward(Math::Vector<> input) = 0;
        virtual Math::Vector<> backward(Math::Vector<> output_gradient, float learning_rate) = 0;
    };
}


#endif //LAYER_H
