#ifndef SORTEDVECTOR_H
#define SORTEDVECTOR_H
#include "Vector.h"

namespace OddityEngine {
    template<typename T = double, typename S = double>
    class SortableVector : public Vector<T> {
    protected:
        Vector<S> _scores;

    public:
        explicit SortableVector(size_t size) : Vector<T>(size), _scores(size) {
        }

        SortableVector(size_t size, T value) : Vector<T>(size, value), _scores(size) {
        }

        SortableVector& resize(size_t size) {
            Matrix<T>::resize_rows(size);
            _scores.resize(size);
            return *this;
        }

        size_t sorted_insert(T value, S score) {
            for (int i = 0; i < _scores.size(); i++) {
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
