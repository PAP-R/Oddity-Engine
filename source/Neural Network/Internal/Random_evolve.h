#ifndef RANDOM_EVOLVE_H
#define RANDOM_EVOLVE_H

#include <numbers>

#include "Layer.h"
#include <Math/random.h>
#include <Util/Vector_overload.h>

namespace OddityEngine::NeuralNetwork {
    class Random_evolve : public Layer {
    protected:
        inline static Vector<std::function<long double(long double)>> available_functions = {
            [](const long double x){return -x;},
            [](const long double x){return 1 / x;},
            [](const long double x){return sqrt(x);},
            [](const long double x){return x * x;},
            [](const long double x){return x * x * x;},
            [](const long double x){return sin(x);},
            [](const long double x){return cos(x);},
            [](const long double x){return tanh(x);},
            [](const long double x){return 1 / (1 + pow(std::numbers::e, -x));},
        };

        long double weight_chance;
        long double bias_chance;
        long double function_chance;

    public:
        Random_evolve(const size_t input_count = 1, const size_t output_count = 1, long double weight_chance = 0.4, long double bias_chance = 0.4, long double function_chance = 0.2) : Layer(input_count, output_count), weight_chance(weight_chance), bias_chance(bias_chance), function_chance(function_chance) {
            long double total_chance = weight_chance + bias_chance + function_chance;
            this->weight_chance /= total_chance;
            this->bias_chance /= total_chance;
            this->function_chance /= total_chance;
        }

        Random_evolve(const Matrix<>& weights, const Vector<>& bias, const Vector<Vector<size_t>>& functions, long double weight_chance = 0.4, long double bias_chance = 0.4, long double function_chance = 0.2) : Layer(weights, bias, functions), weight_chance(weight_chance), bias_chance(bias_chance), function_chance(function_chance) {
            long double total_chance = weight_chance + bias_chance + function_chance;
            this->weight_chance /= total_chance;
            this->bias_chance /= total_chance;
            this->function_chance /= total_chance;
        }

        Random_evolve(const Vector<long double>& csv, long double weight_chance = 0.4, long double bias_chance = 0.4, long double function_chance = 0.2) : Layer(csv[3], csv[5]), weight_chance(weight_chance), bias_chance(bias_chance), function_chance(function_chance) {
            long double total_chance = weight_chance + bias_chance + function_chance;
            auto csv_step = csv.auto_slice();

            this->weights = csv_step[0].auto_slice();
            this->bias = csv_step[1];
            this->functions = csv_step[2].auto_slice<size_t>();

            this->weight_chance /= total_chance;
            this->bias_chance /= total_chance;
            this->function_chance /= total_chance;
        }

        void evolve(long double rate) override {
            size_t index;
            auto random = Math::random<>(0, 1);
            if (random < weight_chance) {
                index = Math::random<size_t>(0, weights.size() - 1);
                weights[index / weights.size(1)][index % weights.size(1)] += Math::random(-rate, rate);
            }
            else if (random < weight_chance + bias_chance) {
                index = Math::random<size_t>(0, bias.size() - 1);
                bias[index] += Math::random(-rate, rate);
            }
            else {
                index = Math::random<size_t>(0, functions.size() - 1);
                functions[index].push_back(Math::random<size_t>(1, available_functions.size() - 1));
            }
        }

        Vector<> forward(const Vector<>& input) override {
            this->input = input;
            this->output = this->weights * input + this->bias;
            for (size_t i = 0; i < this->output.size(); i++) {
                if (!functions[i].empty()) {
                    for (auto f : functions[i]) {
                        this->output[i] = available_functions[f](this->output[i]);
                    }
                }
            }
            return this->output;
        }

        Vector<> backward(const Vector<>& output_gradient, long double learning_rate) override {
            return output_gradient;
        }

        Vector<> to_csv() const {
            Vector<> result;

            this->to_csv(&result);

            return result;
        }

        virtual void to_csv(Vector<>* result) const {
            result->push_back(this->csv_count());
            result->push_back(3);
            weights.to_csv(result);
            bias.to_csv(result);
            functions.to_csv(result);
        }

        size_t csv_count() const {
            size_t count = weights.csv_count();
            count = bias.csv_count();
            count = functions.csv_count();
            return 2 + weights.csv_count() + bias.csv_count() + functions.csv_count();
        }

        static Random_evolve from_csv(const Vector<>& csv) {
            auto random_evolve_csv = csv.auto_slice();

            auto chances = random_evolve_csv[0];

            auto wbf = random_evolve_csv[1].auto_slice();

            auto weights = wbf[0].auto_slice();
            auto bias = wbf[1];
            auto functions_csv = wbf[2].auto_slice();
            Vector<Vector<size_t>> functions(functions_csv.size());

            for (size_t i = 0; i < functions_csv.size(); i++) {
                functions[i] = {functions_csv[i].begin(), functions_csv[i].end()};
            }

            size_t idx = 1;

            Random_evolve result(weights, bias, functions, chances[idx++], chances[idx++], chances[idx++]);

            return result;
        }

        friend std::ostream& operator << (std::ostream& os, const Random_evolve& layer);
    };

    inline std::ostream& operator << (std::ostream& os, const Random_evolve& layer) {
        os << layer.weights << "\n" << layer.bias << "\n" << layer.functions;

        return os;
    }
}



#endif //RANDOM_EVOLVE_H
