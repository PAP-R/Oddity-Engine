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
        long double add_chance;
        long double learning_rate;

        Vector<Random_evolve> layers;

        void resize(size_t layer, size_t size);

        Network evolve();

    public:
        explicit Network(size_t input_count = 0, size_t output_count = 0, size_t mutations = 1, size_t evolutions = 1, long double add_chance = 0.05, long double learning_rate = 0.1);
        explicit Network(const Vector<>& csv, size_t mutations = 1, size_t evolutions = 1, long double add_chance = 0.05, long double learning_rate = 0.1);

        size_t get_input_count() const;
        size_t get_output_count() const;
        size_t get_mutations() const;
        size_t get_evolutions() const;
        long double get_add_chance() const;
        long double get_learning_rate() const;

        Vector<Random_evolve> get_layers() const;

        Vector<> apply(Vector<> input);

        long double test(const Vector<Vector<>>& input_list, const Vector<Vector<>>& output_list);

        static Vector<Network> train(Vector<Network> nets, const Vector<Vector<>>& input_list, const Vector<Vector<>>& output_list);

        static Vector<Network> train(Vector<Network> nets, const Vector<Vector<>>& input_list, const Vector<Vector<>>& output_list, const size_t epochs);

        operator std::string() const;

        operator Vector<> () const;

        Vector<> to_csv() const;

        void to_csv(Vector<>* result) const;

        size_t csv_count() const;

        static Network from_csv(const std::string& path);

        static Network from_csv(const Vector<>&csv);

        static Vector<Network> from_csv_vector(const std::string&path);
    };

    std::ostream& operator<<(std::ostream& os, const Network& network);
}

#endif //NETWORK_H
