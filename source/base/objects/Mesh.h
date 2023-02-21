#ifndef ODDITYENGINE_MESH_H
#define ODDITYENGINE_MESH_H

#include <vector>
using namespace std;

#include "source/base/objects/geometry/HalfEdge.h"
#include "source/base/objects/geometry/Polygon.h"
#include "GL/glew.h"

class Mesh : public Polygon{
protected:
    vector<float> uv;

    vec2 texPos{-1.0f};
    vec2 texSize{2.0f};

public:
    Mesh();
    explicit Mesh(const vector<vec3> &points);

    void set_points(vector<vec3> points);

    vector<float> getVertices();
    vector<GLuint> getIndices();
    size_t get_index_count();
};


#endif //ODDITYENGINE_MESH_H
