#include "Mesh.h"

Mesh::Mesh() {}

Mesh::Mesh(const vector<vec3> &points) : points(points) {
    edge = HalfEdge::addPolygon(points);
}

vector<float> Mesh::getVertices() {
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
    this->points = points;
    edge = HalfEdge::addPolygon(points);
}
