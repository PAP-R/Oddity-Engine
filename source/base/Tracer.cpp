#include "Buffer.h"

#include "Tracer.h"

Tracer::Tracer(vec2 size, Camera* camera) : vertex_shader(GL_VERTEX_SHADER), fragment_shader(GL_FRAGMENT_SHADER), screensize(size), camera(camera), buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW), objectbuffer(GL_SHADER_STORAGE_BUFFER, GL_STATIC_DRAW) {
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

    vertex_shader.add_in("0", "vec3 vertexpos");
    vertex_shader.add_out("vec3 fragmentpos");

    vertex_shader.add_uniform("mat4 MVP");

    vertex_shader.add_main("gl_Position = MVP * vec4(vertexpos, 1.0);");
    vertex_shader.add_main("fragmentpos = vertexpos;");

    fragment_shader.add("struct bufferobject {\n"
                        "vec4 color;\n"
                        "vec4 pos;\n"
                        "vec4 scale;\n"
                        "uint type;\n"
                        "};");

    fragment_shader.add("struct Collision {\n"
                        "vec4 color;\n"
                        "vec4 pos;\n"
                        "int object;\n"
                        "};");

    fragment_shader.add_buffer("std140, std430, binding = 3", "buffer objectbuffer {\n"
                                                "bufferobject objects[];\n"
                                                "};");

    fragment_shader.add_in("vec3 fragmentpos");
    fragment_shader.add_out("vec4 color");
    fragment_shader.add_uniform("float TIME");
    fragment_shader.add_uniform("mat4 MVP");

    fragment_shader.add_constant("uint SPHERE = 0");

    fragment_shader.add_function("vec4 sphere_collision(vec3 pos, float radius, vec3 raypos, vec3 ray) {\n"
                                 "vec3 rayn = normalize(ray);\n"
                                 "vec3 posn = pos - raypos;\n"
                                 "float len = dot(rayn, posn);\n"
                                 "if (len <= 0) {"
                                 "return vec4(0);\n"
                                 "}\n"
                                 "vec3 closest = len * rayn;\n"
                                 "float discp = distance(closest, posn);\n"
                                 "if (discp <= radius) {"
                                 "float diff = sqrt((radius * radius) - (discp * discp));\n"
                                 "return vec4(closest - (diff * rayn), 1);\n"
                                 "}\n"
                                 "else {\n"
                                 "return vec4(closest, 0);\n"
                                 "}\n"
                                 "}");

    fragment_shader.add_function("Collision ray(vec3 position, vec3 direction) {\n"
                                 "vec3 ray = normalize(direction);\n"
                                 "vec4 result = vec4(1. / 0.), temp = vec4(0), color = vec4(0);\n"
                                 "int index = -1;\n"
                                 "for (int i = 0; i < objects.length(); i++) {\n"
                                 "switch (objects[i].type) {\n"
                                 "case SPHERE:\n"
                                 "temp = sphere_collision(objects[i].pos.xyz, objects[i].scale.x, position, ray);"
                                 "}\n"
                                 "if (temp.w == 1 && distance(temp.xyz, position) < distance(result.xyz, position)) {\n"
                                 "result = temp;\n"
                                 "color = objects[i].color;\n"
                                 "index = i;\n"
                                 "}\n"
                                 "}\n"
                                 "Collision col;\n"
                                 "col.color = color;\n"
                                 "col.pos = result;\n"
                                 "col.object = index;\n"
                                 "return col;\n"
                                 "}");

    fragment_shader.add_function("vec4 multiray(vec3 pos, vec3 dir, uint count, uint spread) {\n"
                                 "Collision result;\n"
                                 "vec4 color = vec4(0);\n"
                                 "vec3 nextpos = pos;\n"
                                 "vec3 nextdir = dir;\n"
                                 "for (int i = 0; i < count; i++) {\n"
                                 "result = ray(nextpos, nextdir);\n"
                                 "nextpos = result.pos.xyz;\n"
                                 "if (result.color.w < 1) {"
                                 "color += ray(nextpos, nextdir).color * result.color.w;\n"
                                 "}\n"
                                 "if (result.object == -1) {"
                                 "return color;"
                                 "}"
                                 "color += result.color;\n"
                                 "nextdir = reflect(nextdir, normalize(result.pos.xyz - objects[result.object].pos.xyz));\n"
                                 "}\n"
                                 "return color;\n"
                                 "}");

//    fragment_shader.add_main("Collision collision = ray(fragmentpos);");
//    fragment_shader.add_main("color = vec4(abs((vec3(collision))) * collision.w, 1.0);\n");
//    fragment_shader.add_main("color = vec4(vec3(objects[0].type, objects[0].pos), 1.0);\n");
    fragment_shader.add_main("color = multiray(vec3(0), fragmentpos, 10, 1);");

    vertex_shader.compile();
    fragment_shader.compile();

    program = createProgram(vertex_shader, fragment_shader);

    printf("Done Compiling\n");
}

Tracer::~Tracer() {

}

void Tracer::loop(double dtime) {
    time += dtime;

    vector<bufferobject> spheres = {
            {{1, 1, 1, 0.2}, {sin(time) * 3, 0, cos(time) * 4, 0}, {1, 1, 1, 0}, 0},
            {{0.5, 0, 1, 1}, {0, 0, 6, 0}, {1, 1, 1, 0}, 0},
            {{200, 200, 200, 1}, {0, 200, 0, 0}, {100, 25, 25, 0}, 0},
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
