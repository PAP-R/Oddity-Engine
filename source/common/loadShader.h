#ifndef ODDITYENGINE_LOADSHADER_H
#define ODDITYENGINE_LOADSHADER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;

GLuint loadShaders(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
GLuint loadFileShaders(const char * vertex_file_path,const char * fragment_file_path);

#endif //ODDITYENGINE_LOADSHADER_H
