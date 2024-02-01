#ifndef SORTEDVECTOR_H
#define SORTEDVECTOR_H

#include "Vector.h"

namespace OddityEngine {
    template<typename T = float, typename S = float, std::enable_if_t<std::is_arithmetic_v<S>, bool> = true>
    class SortableVector : public Vector<T> {
    protected:
        Vector<S> _scores;

    public:
        explicit SortableVector(size_t size) : Vector<T>(size), _scores(size, std::numeric_limits<S>::infinity()) {
        }

        SortableVector(size_t size, T value, S score = std::numeric_limits<S>::infinity()) : Vector<T>(size, value), _scores(size, score) {
        }

        SortableVector& resize(size_t size) {
            Vector<T>::resize(size);
            _scores.resize(size);
            return *this;
        }

        size_t sorted_insert(const T& value, S score) {
            for (size_t i = 0; i < _scores.size(); i++) {
                if (score < _scores[i]) {
                    _scores.insert(i, score);
                    Vector<T>::insert(i, value);
                    return i;
                }
            }
            _scores.insert(_scores.size(), score);
            this->insert(this->size(), value);
            return this->size() - 1;
        }

        Vector<S> scores() {
            return _scores;
        }
    };
}

#endif //SORTEDVECTOR_H
