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
    map<T, vec<3, int>> entries;
public:
    Grid(size_t chunkSize=4);

    void insert(vec<3, int> key, T value);
    void move(vec<3, int> key, T value);

    vector<T> get(vec<3, int> key);
};

template<typename T>
vector<T> Grid<T>::get(vec<3, int> key) {
    return grid[key];
}

template<typename T>
void Grid<T>::move(vec<3, int> key, T value) {
    if (entries[value] == key) {
        return;
    }
    if (entries.contains(value)) {
        grid[entries[value]].erase(remove(grid[entries[value]].begin(), grid[entries[value]].end(), value));
        if (grid[entries[value]].empty()) {
            grid.erase(entries[value]);
        }
    }

    insert(key, value);
}

template<typename T>
Grid<T>::Grid(size_t chunkSize) : chunkSize(chunkSize) {}

template<typename T>
void Grid<T>::insert(vec<3, int> key, T value) {
    grid[key].emplace_back(value);
    entries.insert_or_assign(value, key);
}

#endif //ODDITYENGINE_GRID_H
