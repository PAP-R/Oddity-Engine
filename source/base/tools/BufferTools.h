#ifndef ODDITYENGINE_BUFFERTOOLS_H
#define ODDITYENGINE_BUFFERTOOLS_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <vector>
using namespace std;

template <typename T>
GLuint createBuffer(vector<T> data, GLenum type, GLenum mode) {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(type, buffer);
    glBufferData(type, data.size() * sizeof(T), data.data(), mode);
    return buffer;
}

template <typename T>
void editBufferData(GLuint buffer, vector<T> data, GLenum type, GLenum buffermode) {
    glBindBuffer(type, buffer);
    glBufferData(type, data.size() * sizeof(T), data.data(), buffermode);
}

void deleteBuffer(GLuint buffer) {
    glDeleteBuffers(1, &buffer);
}

#endif //ODDITYENGINE_BUFFERTOOLS_H
