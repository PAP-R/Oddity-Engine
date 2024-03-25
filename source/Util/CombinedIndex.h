#ifndef COMBINEDINDEX_H
#define COMBINEDINDEX_H

#include <GL/glew.h>

#include "Vector.h"

namespace OddityEngine::Util::CombinedIndex {
    struct alignas(4) Index {
        GLuint offset;
        GLuint base;
        GLuint count;
        GLuint indices;
    };

    // inline Index generate(const Vector<GLuint>& indices) {
    //     Index result = {0, 0, 0, indices.size(), 0};
    //
    //     for (const auto i : indices) {
    //         result.indices *= i;
    //         result.indices += i;
    //         result.count++;
    //     }
    //
    //     result.last = indices.back();
    //
    //     return result;
    // }
    //
    // inline Vector<GLuint> get(Index index) {
    //     Vector<GLuint> result = {index.last};
    //
    //     for (size_t i = 0; i < index.count; i++) {
    //         index.indices /= index.last;
    //         GLuint temp = index.indices % index.last;
    //         index.last = temp;
    //         result.push_back(temp);
    //         index.indices -= index.last;
    //     }
    //
    //     return result;
    // }

    inline Vector<GLuint> get(Index index) {
        Vector<GLuint> result;

        for (size_t i = 0; i < index.count; i++) {
            result.push_back(index.offset + index.indices % index.base);
            index.indices /= index.base;
        }

        return result;
    }

    inline Index generate(const Vector<GLuint>& indices) {
        GLuint min = indices.front();
        GLuint max = min;

        for (const auto i : indices) {
            if (i > max) {
                max = i;
            }
            if (i < min) {
                min = i;
            }
        }

        Index result = {min, max + 1 - min, indices.size(), 0};

        for (const auto i : indices) {
            result.indices = result.indices * result.base + i - min;
        }

        return result;
    }
}

#endif //COMBINEDINDEX_H
