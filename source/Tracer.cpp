#include "source/base/Buffer.h"

#include "Tracer.h"

#include <utility>

#include "source/base/Loader.h"

Tracer::Tracer(vec2 size, Camera* camera) : vertex_shader(GL_VERTEX_SHADER, "shaders/ray.vert"), fragment_shader(GL_FRAGMENT_SHADER, "shaders/betterray.frag"), screensize(size), camera(camera), buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW), objectbuffer(GL_SHADER_STORAGE_BUFFER, GL_STATIC_DRAW), vertexbuffer(GL_SHADER_STORAGE_BUFFER, GL_STATIC_DRAW), screencamera(vec3(0), vec3(0), 90) {
    time = 0;

    screen = {
            -10.0f, -10.0f, 1.0f,
            10.0f, -10.0f, 1.0f,
            0.0f, 10.0f, 1.0f
    };
    buffer.add_data(screen);

    float time = radians(0.0f);
    float time2 = radians(90.0f);
//    float time = this->time + radians(45.0f);

    vertexbuffer.add_data(obj_to_vert(Loader::obj("models/plane.obj")));

    auto cube = obj_to_vert(Loader::obj("models/torus.obj"));

    auto cubei = vertexbuffer.add_data(cube);

    vector<bufferobject> objects = {
            {{0, 0, 0, 1}, {1, 1, 1, 0.5}, transform(vec3(0, 0, -5), vec3(0, 0, 45), vec3(1)), MESH, cubei.x, cubei.y},
    };

    objectbuffer.add_data(objects);

    vertex_shader.compile();
    fragment_shader.compile();

    program = createProgram(vertex_shader, fragment_shader);

    printf("Done Compiling\n");
}

Tracer::~Tracer() {

}

void Tracer::loop(double dtime) {
    this->time += dtime;
    this->time = time > 360 ? time - 360 : time;

    vector<bufferobject> objects = {
            {{0, 0, 0, 1}, {1, 0.7882, 0.1333, 1}, transform(vec3(0), vec3(0), vec3(50)), SPHERE},
//
            {{0.9, 0.9, 0.9, 1}, {1, 1, 1, 0.5}, transform(vec3(-2, 0, 0), vec3(0, 90, 0), vec3(2)), MESH, 0, 6},
            {{0.9, 0.9, 0.9, 1}, {1, 1, 1, 0.5}, transform(vec3(2, 0, 0), vec3(0, 90, 0), vec3(2)), MESH, 0, 6},
            {{0.9, 0.9, 0.9, 1}, {1, 1, 1, 0.5}, transform(vec3(0, -2, 0), vec3(90, 0, 0), vec3(2)), MESH, 0, 6},
            {{0.9, 0.9, 0.9, 1}, {1, 1, 1, 0.5}, transform(vec3(0, 2, 0), vec3(90, 0, 0), vec3(2)), MESH, 0, 6},
//            {{1, 1, 1, 1}, {1, 1, 1, 0.3}, {0, -1, 5, 0}, {1, 1, 1, 0}, MESH, 12, 6},
//            {{0.5, 0.5, 0.5, 1}, {1, 1, 1, 1}, {0, 0, 10, 0}, {2, 1, 1, 0}, SPHERE},
//            {{1, 1, 1, 0}, {0, 0, 0, 0}, {1, 0, 8, 0}, {0.5, 1, 1, 0}, SPHERE},
//            {{1, 1, 1, 1}, {0, 0, 0, 0}, {-1, 0, 8, 0}, {0.5, 1, 1, 0}, SPHERE},

//            {{0, 0.9, 0, 1}, {0, 1, 0, 0.5}, {0, 1, 10 + cos(this->time) * 5, 0}, {0.5, 0.5, 0.5, 0}, SPHERE, 12, 6},
//            {{0, 0, 0.9, 1}, {0, 0, 1, 0.5}, {-2, -1, 10 + cos(this->time) * 5, 0}, {0.5, 0.5, 0.5, 0}, MESH, 12, 6},

//            {{1, 0, 1, 0.1}, {0, 0, 0, 0}, {0, 0, 3 + cos(this->time) * 2, 0}, {1, 1, 1, 0}, MESH, 12, 6},

//            {{0.9, 0, 0, 1}, {1, 0, 0, 0.5}, {sin(this->time) * 1, cos(this->time) * 1, 4, 0}, {0.5, 1, 1, 0}, SPHERE},
//            {{1, 0, 0, 1}, {1, 0, 0, 1}, {sin(this->time + radians(90.0f)) * 1.5, 0, 7 + cos(this->time + radians(90.0f)) * 1.5, 0}, {0.5, 1, 1, 0}, SPHERE},
//            {{1, 0, 0, 1}, {1, 0, 0, 1}, {sin(this->time + radians(180.0f)) * 2, 0, 5 + cos(this->time + radians(180.0f)) * 2, 0}, {0.5, 1, 1, 0}, SPHERE},
//            {{1, 0, 0, 1}, {1, 0, 0, 1}, {sin(this->time + radians(270.0f)) * 2, 0, 5 + cos(this->time + radians(270.0f)) * 2, 0}, {0.5, 1, 1, 0}, SPHERE},
    };

    objectbuffer.set_data(objects, 1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    mat4 screenprojection = perspective(radians(screencamera.fov), float(screensize.x) / float(screensize.y), 0.1f, 100.0f);

    mat4 projection = perspective(radians(camera->fov), float(screensize.x) / float(screensize.y), 0.1f, 100.0f);
    mat4 view = lookAt(vec3(0), camera->direction(), camera->up());

    glUniform3f(glGetUniformLocation(program, "CAMERAPOS"), camera->position.x, camera->position.y, camera->position.z);

    GLint shaderTime = glGetUniformLocation(program, "TIME");

    glUniform1f(shaderTime, time);

    glUniform1ui(glGetUniformLocation(program, "bounces"), this->bounces);

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
    return translate(mat4(1), translation) * glm::toMat4(quat(radians(rotation))) * glm::scale(mat4(1), scale);
}

vector<buffervertex> Tracer::obj_to_vert(Loader::Object object) {
    vector<buffervertex> vertices;
    for (auto f : object.faces) {
        vertices.emplace_back(vec4(object.vertices[f[0] - 1], 1), vec4(object.colors[f[0] - 1], 1), vec4(object.normals[f[2] - 1], 1), object.uvs[f[1] - 1]);
    }

    return vertices;
}

size_t Tracer::add_objects(vector<bufferobject> objects) {
    return objectbuffer.add_data(std::move(objects))[0];
}

size_t Tracer::set_objects(vector<bufferobject> objects, size_t offset, size_t count) {
    return objectbuffer.set_data(std::move(objects), offset, count)[0];
}

size_t Tracer::add_vertices(vector<buffervertex> verticess) {
    return vertexbuffer.add_data(std::move(verticess))[0];
}

size_t Tracer::set_vertices(vector<buffervertex> vertices, size_t offset, size_t count) {
    return vertexbuffer.set_data(std::move(vertices), offset, count)[0];
}
