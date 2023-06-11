#include "source/base/Buffer.h"

#include "Tracer.h"

#include <utility>

#include "source/base/Loader.h"

Tracer::Tracer(vec2 size, Camera* camera) : vertex_shader(GL_VERTEX_SHADER, "shaders/ray.vert"), fragment_shader(GL_FRAGMENT_SHADER, "shaders/ray.frag"), screensize(size), camera(camera), buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW), objectbuffer(GL_SHADER_STORAGE_BUFFER, GL_STATIC_DRAW), vertexbuffer(GL_SHADER_STORAGE_BUFFER, GL_STATIC_DRAW), screencamera(vec3(0), vec3(0), 90) {
    time = 0;

    screen = {
            -10.0f, -10.0f, 1.0f,
            10.0f, -10.0f, 1.0f,
            0.0f, 10.0f, 1.0f
    };
    buffer.add(screen);

    vertex_shader.compile();
    fragment_shader.compile();

    program = createProgram(vertex_shader, fragment_shader);

    printf("Done Compiling\n");
}

Tracer::~Tracer() {

}

void Tracer::set_camera(Camera* camera) {
    this->camera = camera;
}

void Tracer::set_size(vec2 size) {
    this->screensize = size;
}

void Tracer::loop(double dtime) {
    this->time += dtime;
    this->time = time > 360 ? time - 360 : time;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    mat4 screenprojection = perspective(radians(screencamera.fov), float(screensize.x) / float(screensize.y), 0.1f, 100.0f);

    mat4 projection = perspective(radians(camera->fov), float(screensize.x) / float(screensize.y), 0.1f, 100.0f);
    mat4 view = lookAt(vec3(0), camera->direction(), camera->up());

    glUniform3f(glGetUniformLocation(program, "CAMERAPOS"), camera->position.x, camera->position.y, camera->position.z);

    GLint shaderTime = glGetUniformLocation(program, "TIME");

    glUniform1f(shaderTime, time);

    glUniform1ui(glGetUniformLocation(program, "bounces"), this->bounces);
    glUniform1ui(glGetUniformLocation(program, "spread"), this->spread);

    mat4 model = mat4(1);

    mat4 mvp = projection * view;
    mat4 vp = screenprojection * lookAt(screencamera.position, screencamera.position + screencamera.direction(), screencamera.up());

    glUniformMatrix4fv(glGetUniformLocation(program, "VIEW"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(program, "PROJECTION"), 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(program, "SCREENPROJECTION"), 1, GL_FALSE, &vp[0][0]);

    glBindBufferBase(this->objectbuffer.get_type(), 3, this->objectbuffer);
    glBindBufferBase(this->vertexbuffer.get_type(), 4, this->vertexbuffer);

    glEnableVertexAttribArray(0);

    glBindBuffer(this->buffer.get_type(), this->buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);
}

mat4 Tracer::transform(vec3 translation, vec3 rotation, vec3 scale) {
    return translate(mat4(1), translation) * glm::toMat4(quat(rotation)) * glm::scale(mat4(1), scale);
}

vector<buffervertex> Tracer::obj_to_vert(Loader::Object object) {
    vector<buffervertex> vertices;
    for (auto f : object.faces) {
        vertices.emplace_back(vec4(object.vertices[f[0] - 1], 1), vec4(object.colors[f[0] - 1], 1), vec4(object.normals[f[2] - 1], 1), object.uvs[f[1] - 1]);
    }

    return vertices;
}

void Tracer::apply_buffers() {
    buffer.apply();
    objectbuffer.apply();
    vertexbuffer.apply();
}

void Tracer::clear_buffers_dynamic() {
    buffer.clear_dynamic();
    objectbuffer.clear_dynamic();
    vertexbuffer.clear_dynamic();
}

mat4 Tracer::rotate(vec3 rotation) {
    return glm::toMat4(quat(rotation));
}
