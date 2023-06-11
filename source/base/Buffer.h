#ifndef ODDITYENGINE_BUFFER_H
#define ODDITYENGINE_BUFFER_H

#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
using namespace glm;

#include <vector>
using namespace std;

enum types {
    STATIC,
    DYNAMIC
};

template <typename T>
class Buffer {
private:
    GLuint ID;
    GLuint type;
    GLenum usage;

    vector<T> data;
    size_t dynamic_offset = 0;
    size_t size = 0;

public:
    Buffer(GLuint type, GLenum usage);

    void set_type(GLuint type);
    GLuint get_type();
    void set_usage(GLenum usage);

    size_t get_size();

    vec<2, uint32> add(T data);
    vec<2, uint32> add(vector<T> data);
    vec<2, uint32> set(T data, size_t offset = 0);
    vec<2, uint32> set(vector<T> data, size_t offset = 0, size_t count = 0);
    vec<2, uint32> add_dynamic(T data);
    vec<2, uint32> add_dynamic(vector<T> data, size_t count = 0);

    void apply();

    void clear();
    void clear_dynamic();

    operator unsigned int() const;
};

template<typename T>
void Buffer<T>::apply() {
    glBindBuffer(this->type, ID);
    glBufferData(this->type, this->size * sizeof(T), this->data.data(), this->usage);
}

template<typename T>
void Buffer<T>::clear() {
    this->data.clear();
    this->size = 0;
    this->dynamic_offset = 0;
}

template<typename T>
void Buffer<T>::clear_dynamic() {
    this->size = this->dynamic_offset;
}

template<typename T>
vec<2, uint32> Buffer<T>::add(T data) {
    vec<2, uint32> indices = {this->size, 1};
    this->data.emplace(this->data.begin(), data);
    this->dynamic_offset++;
    this->size++;
    return indices;
}

template<typename T>
vec<2, uint32> Buffer<T>::add(vector<T> data) {
    vec<2, uint32> indices = {this->size, data.size()};
    this->data.insert(this->data.begin(), data.begin(), data.end());
    this->dynamic_offset += data.size();
    this->size += data.size();
    return indices;
}

template<typename T>
vec<2, uint32> Buffer<T>::set(T data, size_t offset) {
    size_t size = offset + 1;
    size = size > this->size ? size : this->size;

    this->data.resize(size);

    this->data[offset] = data;

    this->size = size;
    return {offset, 1};
}

template<typename T>
vec<2, uint32> Buffer<T>::set(vector<T> data, size_t offset, size_t count) {
    count = 0 < count && count < data.size() ? count : data.size();

    size_t size = offset + count;
    size = size > this->size ? size : this->size;

    this->data.resize(size);

    copy(data.begin(), data.begin() + count, this->data.begin() + offset);

    this->size = size;
    return {offset, count};
}

template<typename T>
vec<2, uint32> Buffer<T>::add_dynamic(T data) {
    if (this->size + 1 > this->data.size()) {
        this->data.resize(this->size + 1);
    }

    vec<2, uint32> indices = {this->size, 1};
    this->data[this->size] = data;
    this->size++;
    return indices;
}

template<typename T>
vec<2, uint32> Buffer<T>::add_dynamic(vector<T> data, size_t count) {
    count = 0 < count && count < data.size() ? count : data.size();

    size_t size = this->size + count;
    if (size > this->data.size()) {
        this->data.resize(size);
    }

    vec<2, uint32> indices = {this->size, data.size()};
    copy(data.begin(), data.begin() + count, this->data.begin() + this->size);
    this->size = size;
    return indices;
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
    return size;
}

template<typename T>
Buffer<T>::operator unsigned int() const {
    return ID;
}

#endif //ODDITYENGINE_BUFFER_H
