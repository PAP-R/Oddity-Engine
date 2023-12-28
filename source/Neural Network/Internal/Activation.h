#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Layer.h"

template<size_t node_count>
class Activation : public Layer<node_count, node_count> {
public:
    enum ACTIVATION {
        TANH
    };

protected:
    ACTIVATION activation;

public:
    Activation(ACTIVATION activation) : activation(activation) {}
};



#endif //ACTIVATION_H
