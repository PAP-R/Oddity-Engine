#include "GraphicsObject.h"

#include "source/base/tools/LoadTools.h"

#include "source/base/tools/BufferTools.h"

GraphicsObject::GraphicsObject(const vec3 &pos, const vec3 &dir, const string &vertexShader, const string &fragmentShader) : pos(pos), dir(dir) {
    this->program = loadShaders(vertexShader, fragmentShader);
}

void GraphicsObject::addData(size_t perSize, vector<float> data, GLenum buffermode) {
    this->buffers.emplace_back(createBuffer<float>(data, buffermode));
    this->blockSize.emplace_back(perSize);
    this->size.emplace_back(data.size());
}

void GraphicsObject::editData(size_t buffer, vector<float> data, GLenum buffermode) {
    editBufferData<float>(this->buffers[buffer], data, buffermode);
    this->size[buffer] = data.size();
}

void GraphicsObject::deleteData(size_t buffer) {
    deleteBuffer(this->buffers[buffer]);
    this->buffers.erase(this->buffers.begin() + buffer);
}

void GraphicsObject::selfdestruct() {
    for (int i = this->buffers.size() - 1; i >= 0; i--) {
        this->deleteData(i);
    }
}

bool GraphicsObject::operator==(const GraphicsObject &rhs) const {
    return pos.x == rhs.pos.x &&
           pos.y == rhs.pos.y &&
           pos.z == rhs.pos.z &&
           dir.x == rhs.dir.x &&
           dir.y == rhs.dir.y &&
           dir.z == rhs.dir.z &&
           program == rhs.program &&
           buffers == rhs.buffers &&
           blockSize == rhs.blockSize &&
           size == rhs.size;
}

bool GraphicsObject::operator!=(const GraphicsObject &rhs) const {
    return !(rhs == *this);
}



