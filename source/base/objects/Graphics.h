#ifndef ODDITYENGINE_GRAPHICS_H
#define ODDITYENGINE_GRAPHICS_H

#include "glm/glm.hpp"
using namespace glm;

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <string>
#include <tuple>
#include "source/base/tools/LoadTools.h"
#include "Mesh.h"
#include "Object.h"

#include <vector>
using namespace std;

class Graphics : public virtual Object, public Mesh {
protected:
    GLuint program;
    vector<GLuint> buffers;
    vector<size_t> blockSize;
    size_t size;
    vector<GLenum> type;

public:
    explicit Graphics(const std::string &vertexShader=loadShader("shaders/vert.shader"), const std::string &fragmentShader=loadShader("shaders/wavefrag.shader"));

    ~Graphics();

    GLuint get_program();
    size_t get_buffer_size();
    vector<GLuint> get_buffers();
    vector<size_t> get_blocksize();
    size_t get_size();
    void set_size(size_t size);
    vector<GLenum> get_type();

    void deleteData(size_t buffer);

    bool operator==(const Graphics &rhs) const;

    bool operator!=(const Graphics &rhs) const;

    void loop(float deltaSeconds);

    void editData(size_t buffer, vector<float> data, GLenum buffermode);

    void addData(size_t perSize, vector<float> data, GLenum type, GLenum buffermode);

    void addData(size_t perSize, vector<GLuint> data, GLenum type, GLenum buffermode);
};


#endif //ODDITYENGINE_GRAPHICS_H
