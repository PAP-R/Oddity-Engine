#include "Buffer.h"

#include "Tracer.h"

Tracer::Tracer(vec2 size, Camera* camera) : vertex_shader(GL_VERTEX_SHADER, "shaders/ray.vert"), fragment_shader(GL_FRAGMENT_SHADER, "shaders/ray.frag"), screensize(size), camera(camera), buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW), objectbuffer(GL_SHADER_STORAGE_BUFFER, GL_STATIC_DRAW), screencamera(vec3(0), vec2(0), 90) {
    time = 0;

    screen = {
            -10.0f, -10.0f, 1.0f,
            10.0f, -10.0f, 1.0f,
            0.0f, 10.0f, 1.0f
    };
    buffer.add_data(screen);

    vertex_shader.compile();
    fragment_shader.compile();

    program = createProgram(vertex_shader, fragment_shader);

    printf("Done Compiling\n");
}

Tracer::~Tracer() {

}

void Tracer::loop(double dtime) {
    time += dtime;
    time = time > 360 ? time - 360 : time;

//    camera->fov = 90 + 45 * sin(time/ 2);
//    camera->position = vec3(0, sin(time) / 2, 0);
//    camera->angle = vec2(-time, 0);

    vector<bufferobject> spheres = {
            {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 7, 0}, {200, 25, 25, 0}, 0},
            {{1, 1, 1, 1}, {1, 1, 1, 0}, {0, 0, 7 + sin(time), 0}, {1, 1, 1, 0}, 0},
            {{0, 0, 0, 1}, {1, 1, 1, 1}, {sin(time) * 3, cos(time) * 3, 7, 0}, {1, 1, 1, 0}, 0},
            {{0, 0, 0, 1}, {1, 1, 1, 1}, {sin(time + radians(180.0)) * 3, cos(time + radians(180.0)) * 3, 7, 0}, {1, 1, 1, 0}, 0},
            {{0, 1, 0, 0.5}, {0, 0, 0, 0}, {sin(time + 45) * 3, 0, 7 + cos(time + 45) * 3, 0}, {1, 1, 1, 0}, 0},
//            {{0, 0, 1, 0.5}, {0, 0, 0, 0}, {0, cos(time + 90) * 3, 7 + sin(time + 90) * 3, 0}, {1, 1, 1, 0}, 0},
    };

    objectbuffer.set_data(spheres);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    mat4 projection = perspective(radians(camera->fov), float(screensize.x) / float(screensize.y), 0.1f, 100.0f);

    mat4 view = lookAt(camera->position, camera->position + camera->direction(), camera->up());

    GLint cameraUBO = glGetUniformLocation(program, "CAMERAPOS");

    glUniform3f(cameraUBO, camera->position.x, camera->position.y, camera->position.z);

    GLint shaderTime = glGetUniformLocation(program, "TIME");

    glUniform1f(shaderTime, time);

    glUniform1ui(glGetUniformLocation(program, "bounces"), this->bounces);

    mat4 model = mat4(1);

    mat4 mvp = projection * view;
    mat4 vp = projection * lookAt(screencamera.position, screencamera.position + screencamera.direction(), screencamera.up());

    GLuint matrix = glGetUniformLocation(program, "MVP");

    glUniformMatrix4fv(glGetUniformLocation(program, "VIEW"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(program, "PROJECTION"), 1, GL_FALSE, &vp[0][0]);

    glBindBufferBase(this->objectbuffer.get_type(), 3, this->objectbuffer);

    glEnableVertexAttribArray(0);

    glBindBuffer(this->buffer.get_type(), this->buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);
}
