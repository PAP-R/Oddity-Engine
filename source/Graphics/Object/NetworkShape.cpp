#include "NetworkShape.h"

#include <Graphics/Buffer/Buffer.h>
#include <Graphics/Buffer/Bufferobject.h>

namespace OddityEngine::Graphics {
    Buffer<float>* active_net_buffer = nullptr;

    void set_shape_buffer(Buffer<float>* buffer) {
        active_net_buffer = buffer;
    }

    Buffer<float>* create_net_buffer() {
        set_shape_buffer(new Buffer<float>());
        return active_net_buffer;
    }

    NetworkShape::NetworkShape(NeuralNetwork::Network& network) : Shape(Shape::SDF) {
        if (active_net_buffer == nullptr) create_net_buffer();

        auto csv = network.to_csv();

        auto buffer_csv = create_buffer_object_list(active_net_buffer, csv);

        auto shape = this->shape.get();
        shape.vertex_start = (*buffer_csv)[0].get_index();

        this->shape.set(shape);
    }

    void NetworkShape::bind_buffer() {
        glBindBufferBase(active_shape_buffer->get_type(), NETWORK, *active_net_buffer);
    }
}
