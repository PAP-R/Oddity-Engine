#ifndef ODDITYENGINE_BUFFER_H
#define ODDITYENGINE_BUFFER_H

#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
using namespace glm;

#include <vector>
using namespace std;

template <typename T>
class Buffer {
private:
    GLuint ID;
    GLuint type;
    GLenum usage;

    vector<T> data;

public:
    Buffer(GLuint type, GLenum usage);

    void set_type(GLuint type);
    GLuint get_type();
    void set_usage(GLenum usage);

    size_t get_size();

    vec<2, uint32> add_data(vector<T> data);
    vec<2, uint32> set_data(vector<T> data, size_t offset = 0, size_t count = 0);
    void clear();

    operator unsigned int() const;
};

template<typename T>
void Buffer<T>::clear() {
    this->data.clear();
    glBindBuffer(this->type, ID);
    glBufferData(this->type, this->data.size() * sizeof(T), this->data.data(), this->usage);
}

template<typename T>
vec<2, uint32> Buffer<T>::set_data(vector<T> data, size_t offset, size_t count) {
    count = 0 < count && count < data.size() ? count : data.size();

    size_t size = offset + count;
    size = size > this->data.size() ? size : this->data.size();

    this->data.resize(size);

    copy(data.begin(), data.begin() + count, this->data.begin() + offset);

    glBindBuffer(this->type, ID);
    glBufferData(this->type, this->data.size() * sizeof(T), this->data.data(), this->usage);
    return {offset, count};
}

template<typename T>
Buffer<T>::Buffer(GLuint type, GLenum usage) : ID(), type(type), usage(usage) {
    glGenBuffers(1, &ID);
}

template<typename T>
void Buffer<T>::set_type(GLuint type) {
    this->type = type;
}

template<typename T>
GLuint Buffer<T>::get_type() {
    return this->type;
}

template<typename T>
void Buffer<T>::set_usage(GLenum usage) {
    this->usage = usage;
}

template<typename T>
size_t Buffer<T>::get_size() {
    return data.size();
}

template<typename T>
vec<2, uint32> Buffer<T>::add_data(vector<T> data) {
    vec<2, uint32> indices = {this->data.size(), data.size()};
    this->data.insert(this->data.end(), data.begin(), data.end());
    glBindBuffer(this->type, ID);
    glBufferData(this->type, this->data.size() * sizeof(T), this->data.data(), this->usage);
    return indices;
}

template<typename T>
Buffer<T>::operator unsigned int() const {
    return ID;
}

#endif //ODDITYENGINE_BUFFER_H
