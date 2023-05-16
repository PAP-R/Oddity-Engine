#include "Buffer.h"

#include "Tracer.h"

Tracer::Tracer(vec2 size, Camera* camera) : vertex_shader(GL_VERTEX_SHADER, "shaders/ray.vert"), fragment_shader(GL_FRAGMENT_SHADER, "shaders/ray.frag"), screensize(size), camera(camera), buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW), objectbuffer(GL_SHADER_STORAGE_BUFFER, GL_STATIC_DRAW) {
    time = 0;

    screen = {
            -10.0f, -10.0f, 1.0f,
            10.0f, -10.0f, 1.0f,
            0.0f, 10.0f, 1.0f
    };
    buffer.add_data(screen);

    vector<bufferobject> spheres = {
            {{1, 1, 1, 1}, {0, 0, -2, 0}, {1, 1, 1, 0}, 0},
            {{0.5, 0, 1, 1}, {0, 0, 6, 0}, {1, 1, 1, 0}, 0},
    };
    objectbuffer.add_data(spheres);

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

    vector<bufferobject> spheres = {
            {{0, 1, 0, 0.1}, {sin(time) * 3, 0, cos(time) * 3, 0}, {1, 1, 1, 0}, 0},
            {{1, 1, 1, 1}, {0, 0, 6, 0}, {1, 1, 1, 0}, 0},
            {{200, 0, 0, 1}, {sin(time) * 20, 200, cos(time) * 20, 0}, {100, 25, 25, 0}, 0},
            {{1, 1, 1, 1}, {sin(time) * 3, cos(time) * 3, 5, 0}, {1, 1, 1, 0}, 0},
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

//
//    vec3 up{0, 1, 0};

//    vec3 right = normalize(cross(o->dir, up));

//    right = all(isnan(right)) ? up : right;

//    float angle = orientedAngle(normalize(o->dir), normalize(up), right);
//

    mat4 model = mat4(1);//rotate(scale(translate(mat4(1.0f), vec3(0)), vec3(1)), angle, right);

    mat4 mvp = projection * view * model;

    GLuint matrix = glGetUniformLocation(program, "MVP");

    glUniformMatrix4fv(matrix, 1, GL_FALSE, &mvp[0][0]);

    glBindBufferBase(this->objectbuffer.get_type(), 3, this->objectbuffer);

    glEnableVertexAttribArray(0);

    glBindBuffer(this->buffer.get_type(), this->buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);
}
