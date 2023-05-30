#include "Buffer.h"

#include "Tracer.h"

Tracer::Tracer(vec2 size, Camera* camera) : vertex_shader(GL_VERTEX_SHADER, "shaders/ray.vert"), fragment_shader(GL_FRAGMENT_SHADER, "shaders/betterray.frag"), screensize(size), camera(camera), buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW), objectbuffer(GL_SHADER_STORAGE_BUFFER, GL_STATIC_DRAW), vertexbuffer(GL_SHADER_STORAGE_BUFFER, GL_STATIC_DRAW), screencamera(vec3(0), vec2(0), 90) {
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
    this->time += dtime;
    this->time = time > 360 ? time - 360 : time;

//    camera->fov = 90 + 45 * sin(time/ 2);
//    camera->position = vec3(0, sin(time) / 2, 0);
//    camera->angle = vec2(-time, 0);

    float time = radians(0.0f);
    float time2 = radians(90.0f);
//    float time = this->time + radians(45.0f);
    vector<buffervertex> vertices = {
            {{1 * sin(time), 1, -1 * cos(time), 0}, {0, 0, 1, 1}, {1, 1, 1, 1}},
            {{1 * sin(time), -1, -1 * cos(time), 0}, {1, 0, 0, 1}, {1 * -cos(time), -1, -1 * sin(time), 0}},
            {{-1 * sin(time), -1, 1 * cos(time), 0}, {0, 1, 0, 1}, {-1 * -cos(time), -1, 1 * sin(time), 0}},
            {{-1 * sin(time), -1, 1 * cos(time), 0}, {0, 0, 1, 1}, {-1 * -cos(time), -1, 1 * sin(time), 0}},
            {{-1 * sin(time), 1, 1 * cos(time), 0}, {1, 0, 0, 1}, {-1 * -cos(time), 1, 1 * sin(time), 0}},
            {{1 * sin(time), 1, -1 * cos(time), 0}, {0, 1, 0, 1}, {1 * -cos(time), 1, -1 * sin(time), 0}},
            {{1 * sin(time2), -1 * cos(time2), 1, 0}, {0, 0, 1, 1}, {1, 1, 1, 1}},
            {{1 * sin(time2), -1 * cos(time2), -1, 0}, {1, 0, 0, 1}, {1 * -cos(time), -1, -1 * sin(time), 0}},
            {{-1 * sin(time2), 1 * cos(time2), -1, 0}, {0, 1, 0, 1}, {-1 * -cos(time), -1, 1 * sin(time), 0}},
            {{-1 * sin(time2), 1 * cos(time2), -1, 0}, {0, 0, 1, 1}, {-1 * -cos(time), -1, 1 * sin(time), 0}},
            {{-1 * sin(time2), 1 * cos(time2), 1, 0}, {1, 0, 0, 1}, {-1 * -cos(time), 1, 1 * sin(time), 0}},
            {{1 * sin(time2), -1 * cos(time2), 1, 0}, {0, 1, 0, 1}, {1 * -cos(time), 1, -1 * sin(time), 0}},
            {{1 * sin(time2), 1, -1 * cos(time2), 0}, {0, 0, 1, 1}, {1, 1, 1, 1}},
            {{1 * sin(time2), -1, -1 * cos(time2), 0}, {1, 0, 0, 1}, {1 * -cos(time), -1, -1 * sin(time), 0}},
            {{-1 * sin(time2), -1, 1 * cos(time2), 0}, {0, 1, 0, 1}, {-1 * -cos(time), -1, 1 * sin(time), 0}},
            {{-1 * sin(time2), -1, 1 * cos(time2), 0}, {0, 0, 1, 1}, {-1 * -cos(time), -1, 1 * sin(time), 0}},
            {{-1 * sin(time2), 1, 1 * cos(time2), 0}, {1, 0, 0, 1}, {-1 * -cos(time), 1, 1 * sin(time), 0}},
            {{1 * sin(time2), 1, -1 * cos(time2), 0}, {0, 1, 0, 1}, {1 * -cos(time), 1, -1 * sin(time), 0}},
    };
    vertexbuffer.set_data(vertices);

    vector<bufferobject> objects = {
            {{1, 1, 1, 1}, {1, 1, 1, 0.3}, {-2, 0, 3, 0}, {2, 2, 20, 0}, MESH, 0, 6},
            {{1, 1, 1, 1}, {1, 1, 1, 0.3}, {2, 0, 3, 0}, {2, 2, 20, 0}, MESH, 0, 6},
//            {{1, 1, 1, 1}, {1, 1, 1, 0.4}, {0, -2, 3, 0}, {2, 2, 4, 0}, MESH, 6, 6},
//            {{1, 1, 1, 1}, {1, 1, 1, 0.4}, {0, 2, 3, 0}, {2, 2, 4, 0}, MESH, 6, 6},
//            {{1, 1, 1, 1}, {1, 1, 1, 0.3}, {0, -1, 5, 0}, {1, 1, 1, 0}, MESH, 12, 6},
//            {{0, 0, 0, 0}, {1, 1, 1, 0.5}, {0, 0, 0, 0}, {50, 25, 25, 0}, SPHERE},
//            {{0.5, 0.5, 0.5, 1}, {1, 1, 1, 1}, {0, 0, 10, 0}, {2, 1, 1, 0}, SPHERE},
//            {{1, 1, 1, 0}, {0, 0, 0, 0}, {1, 0, 8, 0}, {0.5, 1, 1, 0}, SPHERE},
//            {{1, 1, 1, 1}, {0, 0, 0, 0}, {-1, 0, 8, 0}, {0.5, 1, 1, 0}, SPHERE},

            {{1, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 10 + cos(this->time) * 5, 0}, {0.5, 0.5, 0.5, 0}, SPHERE, 12, 6},
            {{1, 0, 0, 1}, {1, 0, 0, 1}, {-2, -1, 10 + cos(this->time) * 5, 0}, {0.5, 0.5, 0.5, 0}, MESH, 12, 6},

            {{1, 0, 0, 1}, {1, 0, 0, 1}, {sin(this->time) * 2, 0, 5 + cos(this->time) * 2, 0}, {0.5, 1, 1, 0}, SPHERE},
//            {{1, 0, 0, 1}, {1, 0, 0, 1}, {sin(this->time + radians(90.0f)) * 1.5, 0, 7 + cos(this->time + radians(90.0f)) * 1.5, 0}, {0.5, 1, 1, 0}, SPHERE},
//            {{1, 0, 0, 1}, {1, 0, 0, 1}, {sin(this->time + radians(180.0f)) * 2, 0, 5 + cos(this->time + radians(180.0f)) * 2, 0}, {0.5, 1, 1, 0}, SPHERE},
//            {{1, 0, 0, 1}, {1, 0, 0, 1}, {sin(this->time + radians(270.0f)) * 2, 0, 5 + cos(this->time + radians(270.0f)) * 2, 0}, {0.5, 1, 1, 0}, SPHERE},
    };

    objectbuffer.set_data(objects);

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
    glBindBufferBase(this->vertexbuffer.get_type(), 4, this->vertexbuffer);

    glEnableVertexAttribArray(0);

    glBindBuffer(this->buffer.get_type(), this->buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);
}
