#ifndef LOSSES_H
#define LOSSES_H

#include <cmath>

#include "Math/Vector.h"

namespace OddityEngine::NeuralNetwork {
    static float mse(Math::Vector<> value_true, Math::Vector<> value_predicted) {
        float result = 0;
        for (int i = 0; i < value_true.size(); i++) {
            result += pow(value_true[i] - value_predicted[i], 2);
        }
        return result / static_cast<float>(value_true.size());
    }

    static Math::Vector<> mse_prime(Math::Vector<> value_true, Math::Vector<> value_predicted) {
        return (value_predicted - value_true) / value_true.size() * 2;
    }
}


#endif //LOSSES_H
