#ifndef ACTIVATION_H
#define ACTIVATION_H

#include <cmath>

namespace OddityEngine::NeuralNetwork {
    namespace Activation {
        enum Activation {
            DEFAULT,
            SQUARE,
            SIN,
            COS,
            TANH,
            EXP,
            LAST,
        };
    }

    template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
    static T activate(const Activation::Activation activation, T in) {
        switch (activation) {
            case Activation::DEFAULT:
                return in;
            case Activation::SQUARE:
                return in * in;
            case Activation::SIN:
                return sin(in);
            case Activation::COS:
                return cos(in);
            case Activation::TANH:
                return tanh(in);
            case Activation::EXP:
                return pow(std::numbers::e, in);
        }
    };
}



#endif //ACTIVATION_H
