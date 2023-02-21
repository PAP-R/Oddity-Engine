#include "Mesh.h"
#include "GL/glew.h"

Mesh::Mesh() {}

Mesh::Mesh(const vector<vec3> &points) : Polygon{points} {}

vector<float> Mesh::getVertices() {
    vector<float> vertices;

    vec3 mid(0);

    for (auto p : this->points) {
        mid += p;
    }

    mid /= this->points.size();

    for (auto p : points) {
        for(size_t i = 0; i < 3; i++) {
            vertices.emplace_back(p[i]);
        }

        uv.emplace_back((p.x - mid.x - texPos.x) / texSize.x);
        uv.emplace_back((p.z - mid.z - texPos.y) / texSize.y);
    }

    return vertices;
}

void Mesh::set_points(vector<vec3> points) {
    clear();
    add_face(points);
}

vector<GLuint> Mesh::getIndices() {
    vector<GLuint> indices;
    for (auto triangle : triangles) {
        for(size_t i = 0; i < 3; i++) {
            indices.emplace_back(triangle[i]);
        }
    }
    return indices;
}

size_t Mesh::get_index_count() {
    return triangles.size() * 3;
}
