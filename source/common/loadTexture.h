#ifndef ODDITYENGINE_LOADTEXTURE_H
#define ODDITYENGINE_LOADTEXTURE_H

#include <filesystem>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

GLuint loadDDS(const char * imagepath);
GLuint loadBMP_custom(const char * imagepath);

#endif // ODDITYENGINE_LOADTEXTURE_H
