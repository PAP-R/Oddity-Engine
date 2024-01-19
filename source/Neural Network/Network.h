#ifndef NETWORK_H
#define NETWORK_H

#include <Util/Vector.h>

#include "Internal/Layer.h"
#include "Internal/Random_evolve.h"
#include "../Util/SortableVector.h"

namespace OddityEngine::NeuralNetwork {
    class Network {
    protected:
        size_t input_count;
        size_t output_count;
        size_t mutations;
        size_t evolutions;
        double add_chance;
        double learning_rate;

        Vector<Random_evolve> layers;

        void resize(const size_t layer, const size_t size);

        Network evolve();

    public:
        explicit Network(size_t input_count = 0, size_t output_count = 0, size_t mutations = 1, size_t evolutions = 1, double add_chance = 0.05, double learning_rate = 0.1);

        size_t get_input_count() const;
        size_t get_output_count() const;
        size_t get_mutations() const;
        size_t get_evolutions() const;
        double get_add_chance() const;
        double get_learning_rate() const;

        Vector<Random_evolve> get_layers() const;

        Vector<double> apply(Vector<double> input);

        double test(const Vector<Vector<double>>& input_list, const Vector<Vector<double>>& output_list);

        static Vector<Network> train(Vector<Network> nets, const Vector<Vector<double>>& input_list, const Vector<Vector<double>>& output_list);

        static Vector<Network> train(Vector<Network> nets, const Vector<Vector<double>>& input_list, const Vector<Vector<double>>& output_list, const size_t epochs);

        operator Vector<Vector<>>() const;

        operator std::string() const;

        operator Vector<> () const;

        Vector<double> to_csv() const;

        void to_csv(Vector<double>* result) const;

        static Network from_csv(const std::string& path);

        static Network from_csv(const Vector<double>&csv);

        static Vector<Network> from_csv_vector(const std::string&path);
    };

    std::ostream& operator<<(std::ostream& os, const Network& network);
}

#endif //NETWORK_H
