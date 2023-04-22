#ifndef ODDITYENGINE_GRID_H
#define ODDITYENGINE_GRID_H

#include "glm/glm.hpp"
using namespace glm;

#include <map>
#include <vector>
using namespace std;
struct vec_compair {
    bool operator()(const vec<3, int> l, const vec<3, int> r) const {
        return tie(l.x, l.y, l.z) < tie(r.x, r.y, r.z);
    }
};

template <typename T>
class Grid {
    size_t chunkSize;
    map<vec<3, int>, vector<T>, vec_compair> grid;
    map<T, pair<vec<3, int>, vec<3, int>>> entries;
public:
    Grid(size_t chunkSize=4);

    void insert(vec<3, int> key, T value, vec3 size = vec3(1));
    void move(vec<3, int> key, T value, vec3 size = vec3(1));

    size_t get_chunksize();

    vector<T> get(vec<3, int> key);

    vec<3, int> in_grid(vec3 size);
};

template<typename T>
size_t Grid<T>::get_chunksize() {
    return chunkSize;
}

template<typename T>
vector<T> Grid<T>::get(vec<3, int> key) {
    return grid[key];
}

template<typename T>
void Grid<T>::move(vec<3, int> key, T value, vec3 size) {
    if (entries[value].first == key && entries[value].second == in_grid(size)) {
        return;
    }
    if (entries.contains(value)) {
        vec<3, int> keyTemp = entries[value].first;
        vec<3, int> sizeTemp = entries[value].second;
        for (int x = -sizeTemp.x; x <= sizeTemp.x; x++) {
            for (int y = -sizeTemp.y; y <= sizeTemp.y; y++) {
                for (int z = -sizeTemp.z; z <= sizeTemp.z; z++) {
                    vec<3, int> offset(x, y, z);

                    grid[keyTemp + offset].erase(remove(grid[keyTemp + offset].begin(), grid[keyTemp + offset].end(), value));
                    if (grid[keyTemp + offset].empty()) {
                        grid.erase(keyTemp + offset);
                    }
                }
            }
        }
    }

    insert(key, value, size);
}

template<typename T>
Grid<T>::Grid(size_t chunkSize) : chunkSize(chunkSize) {}

template<typename T>
void Grid<T>::insert(vec<3, int> key, T value, vec3 size) {
    vec<3, int> sizeInGrid = in_grid(size);

    entries.insert_or_assign(value, make_pair(key, sizeInGrid));

    for (int x = -sizeInGrid.x; x <= sizeInGrid.x; x++) {
        for (int y = -sizeInGrid.y; y <= sizeInGrid.y; y++) {
            for (int z = -sizeInGrid.z; z <= sizeInGrid.z; z++) {
                vec<3, int> offset(x, y, z);
                grid[key + offset].emplace_back(value);
            }
        }
    }
}

template<typename T>
vec<3, int> Grid<T>::in_grid(vec3 size) {
    return size / float(chunkSize);
}

#endif //ODDITYENGINE_GRID_H
