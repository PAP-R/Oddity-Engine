#ifndef ODDITYENGINE_LOADTOOLS_H
#define ODDITYENGINE_LOADTOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <string>

using namespace std;

#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
using namespace glm;

string loadShader(const char * path);

GLuint loadShaders(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
GLuint loadFileShaders(const char * vertex_file_path,const char * fragment_file_path);

GLuint loadDDS(const char * imagepath);
GLuint loadBMP_custom(const char * imagepath);

#endif //ODDITYENGINE_LOADTOOLS_H
