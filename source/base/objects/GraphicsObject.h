#ifndef ODDITYENGINE_GRAPHICSOBJECT_H
#define ODDITYENGINE_GRAPHICSOBJECT_H

#include "glm/glm.hpp"
using namespace glm;

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <string>
#include <vector>
#include <tuple>
#include "source/base/tools/LoadTools.h"

using namespace std;

class GraphicsObject {
public:
    vec3 pos;
    vec3 dir;
    vec3 scale;
    GLuint program;
    vector<GLuint> buffers;
    vector<size_t> blockSize;
    vector<size_t> size;

    GraphicsObject(const vec3 &pos=vec3(0), const vec3 &dir=vec3(0, 1, 0), const vec3 &scale=vec3(1, 1, 1), const string &vertexShader=loadShader("shaders/vert.shader"), const string &fragmentShader=loadShader("shaders/frag.shader"));

    void addData(size_t perSize, vector<float> data, GLenum buffermode);
    void editData(size_t buffer, vector<float> data, GLenum buffermode);
    void deleteData(size_t buffer);

    void selfdestruct();

    bool operator==(const GraphicsObject &rhs) const;

    bool operator!=(const GraphicsObject &rhs) const;
};


#endif //ODDITYENGINE_GRAPHICSOBJECT_H
