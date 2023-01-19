#include "Cube.h"

#include <algorithm>

using namespace std;

std::vector<vec3> cubeCorners() {
    size_t cornerCount = 8;
    std::vector<vec3> cornerData;

    for (int i = 0; i < cornerCount; i++) {
        cornerData.emplace_back(((i / 4) % 2) * 2 - 1, ((i / 2) % 2) * 2 - 1, ((i) % 2) * 2 - 1);
        printf("[%3f / %3f / %3f]\n", cornerData.back().x, cornerData.back().y, cornerData.back().z);
    }

    return cornerData;
}

bool compair(tuple<int, vec3, float> first, tuple<int, vec3, float> second) {
    return get<2>(first) < get<2>(second);
}

Cube::Cube() {
    vector<vec3> verts = cubeCorners();

    vector<vector<tuple<int, vec3, float>>> lengths(verts.size());

    for (auto i = 0; i < verts.size(); i++) {
        points.emplace_back(verts[i], i);
    }

    for (int v = 0; v < verts.size(); v++) {
        for (int o = 0; o < verts.size(); o++) {
            int offset = (v + o) % verts.size();
            lengths[v].emplace_back(offset, verts[offset], distance(verts[v], verts[offset]));
        }
        sort(lengths[v].begin(), lengths[v].end(), compair);

        auto min = get<2>(lengths[v][1]);
        for (auto i = 1; i < verts.size() && get<2>(lengths[v][i]) <= min; i++) {
            points[v].cons.emplace_back(&points[get<0>(lengths[v][i])]);
        }
        printf("%d: %d\t", v, points[v].cons.size());
    }

    int c = 0;

    for (auto p : points) {
        auto len = p.cons.size();
        for (auto i = 0; i < len; i++) {
            p.insertSelf(&vertices);
            p.cons[i]->insertSelf(&vertices);
            p.cons[(i + 1) % len]->insertSelf(&vertices);
            c++;
        }
    }

    printf("\nf: %d\tv: %d\tt: %d / %d\n", vertices.size(), vertices.size()/3, vertices.size()/9, c);
}
