#ifndef ODDITYENGINE_BUFFERTOOLS_H
#define ODDITYENGINE_BUFFERTOOLS_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <vector>
using namespace std;

template <typename T>
GLuint createBuffer(vector<T> data, GLenum buffermode) {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), buffermode);
    return buffer;
}

template <typename T>
void editBufferData(GLuint buffer, vector<T> data, GLenum buffermode) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), buffermode);
}

void deleteBuffer(GLuint buffer) {
    glDeleteBuffers(1, &buffer);
}

#endif //ODDITYENGINE_BUFFERTOOLS_H
