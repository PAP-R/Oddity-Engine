#include "Cube.h"

#include <algorithm>
#include <tuple>
#include <limits>

#include <cstdio>

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
        auto max = numeric_limits<float>::max();
        for (auto i = 1; i < verts.size() && get<2>(lengths[v][i]) <= min; i++) {
            if (false && get<2>(lengths[v][i]) > min) {
                max = get<2>(lengths[v][i]);
            }
            points[v].cons.emplace_back(&points[get<0>(lengths[v][i])], 2);
        }
    }

    int c = 0;

    for (auto p = 0; p < points.size(); p++) {
        printf("%d: %d\n", p, points[p].cons.size());
        auto len = points[p].cons.size();
        for(auto i = 0; i < len; i++) {
            printf("%d\t", get<0>(points[p].cons[i])->id);
        }
        printf("\n");
        len = len > 3 ? 3 : len == 2 ? 1 : len;
        if (len < 2) {
            continue;
        }
        for (auto i = 0; i < len; i++) {
            points[p].insertSelf(&vertices);
            get<0>(points[p].cons[i])->insertSelf(&vertices);
            get<0>(points[p].cons[(i + 1) % len])->insertSelf(&vertices);

            printf("Triangle: %d -> %f -> %d -> %f -> %d\n", p, distance(points[p].pos, get<0>(points[p].cons[i])->pos), get<0>(points[p].cons[i])->id, distance(points[p].pos, get<0>(points[p].cons[(i + 1) % len])->pos), get<0>(points[p].cons[(i + 1) % len])->id);

            get<1>(points[p].cons[i])--;
            get<1>(*find_if(get<0>(points[p].cons[i])->cons.begin(), get<0>(points[p].cons[i])->cons.end(), [this, &p] (const tuple<Point*, size_t> t) {return get<0>(t) == &points[p];}))--;
            if(get<1>(points[p].cons[i]) == 0) {
                printf("Deleted %d from %d\n", p, get<0>(points[p].cons[i])->id);
                get<0>(points[p].cons[i])->cons.erase(remove(get<0>(points[p].cons[i])->cons.begin(), get<0>(points[p].cons[i])->cons.end(), tuple(&points[p], 0)), get<0>(points[p].cons[i])->cons.end());
            }
            
            get<1>(points[p].cons[(i + 1) % len])--;
            get<1>(*find_if(get<0>(points[p].cons[(i + 1) % len])->cons.begin(), get<0>(points[p].cons[(i + 1) % len])->cons.end(), [this, &p] (const tuple<Point*, size_t> t) {return get<0>(t) == &points[p];}))--;
            if(get<1>(points[p].cons[(i + 1) % len]) == 0) {
                printf("Deleted %d from %d\n", p, get<0>(points[p].cons[(i + 1) % len])->id);
                get<0>(points[p].cons[(i + 1) % len])->cons.erase(remove(get<0>(points[p].cons[(i + 1) % len])->cons.begin(), get<0>(points[p].cons[(i + 1) % len])->cons.end(), tuple(&points[p], 0)), get<0>(points[p].cons[(i + 1) % len])->cons.end());
            }

            c++;
        }
    }

    printf("\nf: %d\tv: %d\tt: %d / %d\n", vertices.size(), vertices.size()/3, vertices.size()/9, c);
}
