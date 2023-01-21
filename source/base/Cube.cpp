#include "Cube.h"

#include <algorithm>
#include <tuple>
#include <limits>

#include <string>

#include <cstdio>

using namespace std;

#include "../debug/Debug.h"

std::vector<vec3> cubeCorners() {
    size_t cornerCount = 8;
    std::vector<vec3> cornerData;

    for (int i = 0; i < cornerCount; i++) {
        cornerData.emplace_back(((i / 4) % 2) * 2 - 1, ((i / 2) % 2) * 2 - 1, ((i) % 2) * 2 - 1);
    }

    cornerData.emplace_back(0, 1, 0);

    return cornerData;
}

bool compair(tuple<int, vec3, float> first, tuple<int, vec3, float> second) {
    return get<2>(first) < get<2>(second);
}

Cube::Cube() {
    vector<vec3> verts = cubeCorners();

    vector<vector<tuple<int, vec3, float>>> lengths(verts.size());

    printf("%1.0f %1.0f %1.0f\n", pos.x, pos.y, pos.z);

    for (auto i = 0; i < verts.size(); i++) {
        auto dir = normalize(verts[i] - this->pos);
        Debug::add_point(verts[i], format("[{}]\n{:1.1} {:1.1} {:1.1}\n{:1.1} {:1.1} {:1.1}\n", i, verts[i].x, verts[i].y, verts[i].z, dir.x, dir.y, dir.z));
        Debug::add_point(verts[i] + dir, "*");
        points.emplace_back(verts[i], normalize(verts[i] - this->pos), i);
    }

    for (int v = 0; v < verts.size(); v++) {
        for (int o = 0; o < verts.size(); o++) {
            int offset = (v + o) % verts.size();
            lengths[v].emplace_back(offset, verts[offset], distance(verts[v], verts[offset]));
        }
        sort(lengths[v].begin(), lengths[v].end(), compair);

        auto min = get<2>(lengths[v][1]);
        for (auto i = 1; i < verts.size(); i++) {
            points[v].cons.emplace_back(&points[get<0>(lengths[v][i])], 2);
        }
    }

    for (auto p = 0; p < points.size(); p++) {
        auto len = points[p].cons.size();
        len = len > 3 ? 3 : len == 2 ? 1 : len;
        if (len < 2) {
            continue;
        }
        for (auto i = 0; i < len; i++) {
            points[p].insertSelf(&vertices);

            auto criss = normalize(cross(get<0>(points[p].cons[i])->pos - points[p].pos, get<0>(points[p].cons[(i + 1) % len])->pos - points[p].pos));
            auto cr0ss = normalize(cross(get<0>(points[p].cons[(i + 1) % len])->pos - points[p].pos, get<0>(points[p].cons[i])->pos - points[p].pos));

            Debug::add_point(points[p].pos + criss * vec3(0.2f), format("[0]", p));

            if(distance(criss, normalize(points[p].dir)) > distance(cr0ss, normalize(points[p].dir))) {
                iter_swap(points[p].cons.begin() + i, points[p].cons.begin() + (i + 1) % len);
                Debug::add_point(points[p].pos + criss * vec3(0.2f), format("[0]", p));
            }
            else {
                Debug::add_point(points[p].pos + criss * vec3(0.2f), format("[+]", p));
            }

            get<0>(points[p].cons[i])->insertSelf(&vertices);

            get<0>(points[p].cons[(i + 1) % len])->insertSelf(&vertices);

            //printf("Polygon: %d -> %f -> %d -> %f -> %d\n", p, distance(points[p].pos, get<0>(points[p].cons[i])->pos), get<0>(points[p].cons[i])->id, distance(points[p].pos, get<0>(points[p].cons[(i + 1) % len])->pos), get<0>(points[p].cons[(i + 1) % len])->id);

            get<1>(points[p].cons[i])--;
            if(find_if(get<0>(points[p].cons[i])->cons.begin(), get<0>(points[p].cons[i])->cons.end(), [this, &p] (const tuple<Point*, size_t> t) {return get<0>(t) == &points[p];}) != get<0>(points[p].cons[i])->cons.end()) {
                get<1>(*find_if(get<0>(points[p].cons[i])->cons.begin(), get<0>(points[p].cons[i])->cons.end(), [this, &p] (const tuple<Point*, size_t> t) {return get<0>(t) == &points[p];}))--;
            }
            if(get<1>(points[p].cons[i]) == 0) {
                printf("Deleted %d from %d\n", p, get<0>(points[p].cons[i])->id);
                get<0>(points[p].cons[i])->cons.erase(remove(get<0>(points[p].cons[i])->cons.begin(), get<0>(points[p].cons[i])->cons.end(), tuple(&points[p], 0)), get<0>(points[p].cons[i])->cons.end());
            }
            
            get<1>(points[p].cons[(i + 1) % len])--;
            if(find_if(get<0>(points[p].cons[(i + 1) % len])->cons.begin(), get<0>(points[p].cons[(i + 1) % len])->cons.end(), [this, &p] (const tuple<Point*, size_t> t) {return get<0>(t) == &points[p];}) != get<0>(points[p].cons[(i + 1) % len])->cons.end()) {
                get<1>(*find_if(get<0>(points[p].cons[(i + 1) % len])->cons.begin(), get<0>(points[p].cons[(i + 1) % len])->cons.end(), [this, &p] (const tuple<Point*, size_t> t) {return get<0>(t) == &points[p];}))--;
            }
            if(get<1>(points[p].cons[(i + 1) % len]) == 0) {
                printf("Deleted %d from %d\n", p, get<0>(points[p].cons[(i + 1) % len])->id);
                get<0>(points[p].cons[(i + 1) % len])->cons.erase(remove(get<0>(points[p].cons[(i + 1) % len])->cons.begin(), get<0>(points[p].cons[(i + 1) % len])->cons.end(), tuple(&points[p], 0)), get<0>(points[p].cons[(i + 1) % len])->cons.end());
            }
        }
    }
}
