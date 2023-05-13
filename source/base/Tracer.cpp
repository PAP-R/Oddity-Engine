#include "Tracer.h"

Tracer::Tracer(vec2 size, Camera* camera) : vertex_shader(GL_VERTEX_SHADER), fragment_shader(GL_FRAGMENT_SHADER), screensize(size), camera(camera), buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW) {
    screen = {
            -10.0f, -10.0f, 1.0f,
            10.0f, -10.0f, 1.0f,
            0.0f, 10.0f, 1.0f
    };

    buffer.add_data(screen);

    vertex_shader.add_in(0, "vec3 vertexpos");
    vertex_shader.add_out("vec3 fragmentpos");

    vertex_shader.add_uniform("mat4 MVP");

    vertex_shader.add_main("gl_Position = MVP * vec4(vertexpos, 1.0);");
    vertex_shader.add_main("fragmentpos = vertexpos;");

    fragment_shader.add_in("vec3 fragmentpos");
    fragment_shader.add_out("vec4 color");
    fragment_shader.add_uniform("float TIME");
    fragment_shader.add_uniform("mat4 MVP");

    fragment_shader.add_function("vec4 sphere_collision(vec3 pos, float radius, vec3 ray) {\n"
                                 "vec3 rayn = normalize(ray);\n"
                                 "vec3 closest = dot(rayn, pos) * rayn;\n"
                                 "float discp = distance(closest, pos);\n"
                                 "if (discp <= radius) {"
                                 "float diff = sqrt((radius * radius) - (discp * discp));\n"
                                 "return vec4(closest - (diff * rayn), 1);\n"
                                 "}\n"
                                 "else {\n"
                                 "return vec4(closest, 0);\n"
                                 "}\n"
                                 "}");

    fragment_shader.add_main("vec3 ball = (vec4(0.0, 0.1 * sin(TIME), 2.0, 1.0) * MVP).xyz;");
    fragment_shader.add_main("float dot_radius = 1.0;");
    fragment_shader.add_main("vec4 collision = sphere_collision(ball, dot_radius, fragmentpos);");
    fragment_shader.add_main("color = vec4(abs((vec3((collision.xyz - ball)))) * collision.w, 1.0);\n");
//    fragment_shader.add_main("color = vec4(vec3(collision.w / dot_radius), 1.0);\n");
//    fragment_shader.add_main("if (collision.w == 1) {\n"
////                             "color = vec4(vec3(distance(collision.xyz, ball) / dot_radius), 1);\n"
//                             "color = vec4(collision.xyz, 1);\n"
//                             "}\n"
//                             "else {\n"
////                             "color = vec4(pc, 1.0);\n"
////                             "color = vec4(vec3(distance(ray, ball)), 1.0);\n"
//                             "color = vec4(vec3(distance(collision.xyz, ball) / 5), 1.0);\n"
//                             "}");
//    fragment_shader.add_main("color = vec4(fragmentpos.x, fragmentpos.y, 0.0, 1.0);");

    vertex_shader.compile();
    fragment_shader.compile();

    program = createProgram(vertex_shader, fragment_shader);

    printf("Done Compiling\n");
}

Tracer::~Tracer() {

}

void Tracer::loop(double dtime) {
    time += dtime;

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

    glEnableVertexAttribArray(0);

    glBindBuffer(this->buffer.get_type(), this->buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);
}
