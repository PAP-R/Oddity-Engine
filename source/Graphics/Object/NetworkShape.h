#ifndef NETWORKSHAPE_H
#define NETWORKSHAPE_H

#include "Shape.h"
#include <Neural Network/Network.h>

namespace OddityEngine::Graphics {
    class NetworkShape : public Shape {
    public:
        NetworkShape(NeuralNetwork::Network& network) : Shape(Shapes::SDF) {

        }
    };
}



#endif //NETWORKSHAPE_H
