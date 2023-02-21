#include "Graphics.h"

#include "source/base/tools/LoadTools.h"

#include "source/base/tools/BufferTools.h"

Graphics::Graphics(const std::string &vertexShader, const std::string &fragmentShader) {
    this->program = loadShaders(vertexShader, fragmentShader);
}

Graphics::~Graphics() {
    for (int i = this->buffers.size() - 1; i >= 0; i--) {
        this->deleteData(i);
    }
}

void Graphics::addData(size_t perSize, vector<float> data, GLenum type, GLenum buffermode) {
    this->buffers.emplace_back(createBuffer<float>(data, type, buffermode));
    this->blockSize.emplace_back(perSize);
    this->type.emplace_back(type);
}

void Graphics::addData(size_t perSize, vector<GLuint> data, GLenum type, GLenum buffermode) {
    this->buffers.emplace_back(createBuffer<GLuint>(data, type, buffermode));
    this->blockSize.emplace_back(perSize);
    this->type.emplace_back(type);
}

void Graphics::editData(size_t buffer, vector<float> data, GLenum buffermode) {
    editBufferData<float>(this->buffers[buffer], data, this->type[buffer], buffermode);
}

void Graphics::deleteData(size_t buffer) {
    deleteBuffer(this->buffers[buffer]);
    this->buffers.erase(this->buffers.begin() + buffer);
}

bool Graphics::operator==(const Graphics &rhs) const {
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

bool Graphics::operator!=(const Graphics &rhs) const {
    return !(rhs == *this);
}

GLuint Graphics::get_program() {
    return program;
}

size_t Graphics::get_buffer_size() {
    return buffers.size();
}

vector<GLuint> Graphics::get_buffers() {
    return buffers;
}

vector<size_t> Graphics::get_blocksize() {
    return blockSize;
}

size_t Graphics::get_size() {
    return size;
}

void Graphics::loop(float deltaSeconds) {
    Object::loop(deltaSeconds);
}

vector<GLenum> Graphics::get_type() {
    return type;
}

void Graphics::set_size(size_t size) {
    Graphics::size = size;
}



