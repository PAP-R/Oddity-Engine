#ifndef NETWORK_H
#define NETWORK_H

#include <Util/Vector.h>

#include "Internal/Layer.h"
#include "Internal/Random_evolve.h"
#include "../Util/SortableVector.h"

#define MAX_THROUGHPUT 10

namespace OddityEngine::NeuralNetwork {
    class Trainer;
    class Network {
    protected:
        size_t input_count;
        size_t output_count;

        Vector<Random_evolve> layers;

        void resize(size_t layer, size_t size);

        friend Trainer;

    public:
        explicit Network(size_t input_count = 0, size_t output_count = 0);
        explicit Network(const Vector<>& csv);

        size_t get_input_count() const;
        size_t get_output_count() const;
        size_t get_mutations() const;
        size_t get_evolutions() const;
        float get_add_chance() const;
        float get_learning_rate() const;

        Vector<Random_evolve> get_layers() const;

        Vector<> apply(const Vector<>& input) const;

        operator std::string() const;

        operator Vector<> () const;

        Vector<> to_csv() const;

        void to_csv(Vector<>* result) const;

        size_t csv_count() const;
    };

    class Trainer {
    private:
        size_t mutations;
        size_t evolutions;
        float add_chance;
        float learning_rate;
        std::function<float(const Network&, const Vector<Vector<>>&, const Vector<Vector<>>&)> test_function;

        Network evolve(const Network& net);

        static float test(const Network& net, const Vector<Vector<>>& input_list, const Vector<Vector<>>& output_list);

    public:
        Trainer(size_t mutations = 1, size_t evolutions = 1, float add_chance = 0.05, float learning_rate = 0.1, const std::function<float(const Network&, const Vector<Vector<>>&, const Vector<Vector<>>&)>& test_function = test);

        Vector<Network> train(Vector<Network> nets, const Vector<Vector<>>& input_list, const Vector<Vector<>>& output_list);

        Vector<Network> train(Vector<Network> nets, const Vector<Vector<>>& input_list, const Vector<Vector<>>& output_list, const size_t epochs);

        Vector<Network> train(Vector<Network> nets, size_t count, std::function<Vector<>(void)> input, std::function<Vector<>(Vector<>)> output, size_t epochs = 1);
    };

    std::ostream& operator<<(std::ostream& os, const Network& network);
}

#endif //NETWORK_H
