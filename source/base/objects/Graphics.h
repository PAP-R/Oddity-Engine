#ifndef ODDITYENGINE_GRAPHICS_H
#define ODDITYENGINE_GRAPHICS_H

#include "glm/glm.hpp"
using namespace glm;

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <string>
#include <vector>
#include <tuple>
#include "source/base/tools/LoadTools.h"
#include "Mesh.h"
#include "Object.h"

using namespace std;

class Graphics : public virtual Object, public Mesh {
protected:
    GLuint program;
    vector<GLuint> buffers;
    vector<size_t> blockSize;
    vector<size_t> size;

public:
    explicit Graphics(const string &vertexShader=loadShader("shaders/vert.shader"), const string &fragmentShader=loadShader("shaders/wavefrag.shader"));

    ~Graphics();

    GLuint get_program();
    size_t get_buffer_size();
    vector<GLuint> get_buffers();
    vector<size_t> get_blocksize();
    vector<size_t> get_size();

    void addData(size_t perSize, vector<float> data, GLenum buffermode);
    void editData(size_t buffer, vector<float> data, GLenum buffermode);
    void deleteData(size_t buffer);

    bool operator==(const Graphics &rhs) const;

    bool operator!=(const Graphics &rhs) const;

    void loop(float deltaSeconds);
};


#endif //ODDITYENGINE_GRAPHICS_H
