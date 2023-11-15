#include "Shape.h"

namespace OddityEngine::Graphics {
    Buffer<buffershape>* Shape::active_shape_buffer = nullptr;
    Buffer<glm::vec3>* Shape::active_vertex_buffer = nullptr;
    Buffer<glm::vec3>* Shape::active_normal_buffer = nullptr;
    Buffer<glm::vec3>* Shape::active_uv_buffer = nullptr;

    Buffer<buffershape> *Shape::create_shape_buffer() {
        set_shape_buffer(new Buffer<buffershape>());
        return active_shape_buffer;
    }

    void Shape::set_shape_buffer(Buffer<buffershape> *buffer) {
        active_shape_buffer = buffer;
    }

    Buffer<buffershape> *Shape::get_shape_buffer() {
        return active_shape_buffer;
    }

    Buffer<glm::vec3> *Shape::create_vertex_buffer() {
        set_vertex_buffer(new Buffer<glm::vec3>());
        return active_vertex_buffer;
    }

    void Shape::set_vertex_buffer(Buffer<glm::vec3> *buffer) {
        active_vertex_buffer = buffer;
    }

    Buffer<glm::vec3> *Shape::get_vertex_buffer() {
        return active_vertex_buffer;
    }

    Buffer<glm::vec3> *Shape::create_normal_buffer() {
        set_normal_buffer(new Buffer<glm::vec3>());
        return active_vertex_buffer;
    }

    void Shape::set_normal_buffer(Buffer<glm::vec3> *buffer) {
        active_normal_buffer = buffer;
    }

    Buffer<glm::vec3> *Shape::get_normal_buffer() {
        return active_vertex_buffer;
    }

    Buffer<glm::vec3> *Shape::create_uv_buffer() {
        set_uv_buffer(new Buffer<glm::vec3>());
        return active_uv_buffer;
    }

    void Shape::set_uv_buffer(Buffer<glm::vec3> *buffer) {
        active_uv_buffer = buffer;
    }

    Buffer<glm::vec3> *Shape::get_uv_buffer() {
        set_uv_buffer(new Buffer<glm::vec3>());
        return active_uv_buffer;
    }

    void Shape::bind_buffer() {
        glBindBufferBase(active_shape_buffer->get_type(), SHAPE, *active_shape_buffer);
        glBindBufferBase(active_vertex_buffer->get_type(), VERTEX, *active_vertex_buffer);
        glBindBufferBase(active_normal_buffer->get_type(), NORMAL, *active_normal_buffer);
        glBindBufferBase(active_uv_buffer->get_type(), UV, *active_uv_buffer);
    }

    Shape::Shape(Shapes shape, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec3>& uvs) : shape(active_shape_buffer) {
        if (active_shape_buffer == nullptr) create_shape_buffer();
        if (active_vertex_buffer == nullptr) create_vertex_buffer();
        if (active_normal_buffer == nullptr) create_normal_buffer();
        if (active_uv_buffer == nullptr) create_uv_buffer();

        float min_x = 0, max_x = 0, min_y = 0, max_y = 0, min_z = 0, max_z = 0;

        for (auto v : vertices) {
            if (v.x < min_x)
                min_x = v.x;
            if (v.x > max_x)
                max_x = v.x;
            if (v.y < min_y)
                min_y = v.y;
            if (v.y > max_y)
                max_y = v.y;
            if (v.z < min_z)
                min_z = v.z;
            if (v.z > max_z)
                max_z = v.z;
        }

        bounding_box_center = {(min_x + max_x) / 2, (min_y + max_y) / 2, (min_z + max_z) / 2};
        bounding_box_size = {abs(min_x - max_x), abs(min_y - max_y), abs(min_z - max_z)};

        this->vertices = create_buffer_object_list(active_vertex_buffer, vertices);
        this->normals = create_buffer_object_list(active_normal_buffer, normals);
        this->uvs = create_buffer_object_list(active_uv_buffer, uvs);

        this->shape.set({
            shape,
            static_cast<GLuint>((*this->vertices)[0].get_index()),
            static_cast<GLuint>(this->vertices->size()),
            static_cast<GLuint>((*this->normals)[0].get_index()),
            static_cast<GLuint>(this->normals->size()),
            static_cast<GLuint>((*this->uvs)[0].get_index()),
            static_cast<GLuint>(this->uvs->size()),
            bounding_box_center,
            bounding_box_size
        });
    }

    GLuint Shape::index() {
        return shape.get_index();
    }
}