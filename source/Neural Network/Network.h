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
        double get_add_chance() const;
        double get_learning_rate() const;

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
        double add_chance;
        double learning_rate;

        Network evolve(const Network& net);

    public:
        Trainer(size_t mutations = 1, size_t evolutions = 1, double add_chance = 0.05, double learning_rate = 0.1);

        double test(const Network& net, const Vector<Vector<>>& input_list, const Vector<Vector<>>& output_list);

        Vector<Network> train(Vector<Network> nets, const Vector<Vector<>>& input_list, const Vector<Vector<>>& output_list);

        Vector<Network> train(Vector<Network> nets, const Vector<Vector<>>& input_list, const Vector<Vector<>>& output_list, const size_t epochs);
    };

    std::ostream& operator<<(std::ostream& os, const Network& network);
}

#endif //NETWORK_H
