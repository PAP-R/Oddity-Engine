#ifndef ODDITYENGINE_MESH_H
#define ODDITYENGINE_MESH_H

#include <vector>

#include "source/object/HalfEdge.h"

class Mesh {
protected:
    HalfEdge *edge;
    vector<vec3> points;
    vector<float> vertices;
    vector<float> uv;

    vec2 texPos{-1.0f};
    vec2 texSize{2.0f};

public:
    Mesh();
    explicit Mesh(const vector<vec3> &points);

    void set_points(vector<vec3> points);

    vector<float> getVertices();
};


#endif //ODDITYENGINE_MESH_H
