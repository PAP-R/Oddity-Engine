#ifndef ODDITYENGINE_HALFEDGE_H
#define ODDITYENGINE_HALFEDGE_H

#include <vector>
using namespace std;

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
using namespace glm;

#include <cstddef>
#include <iterator>

class HalfEdge {
private:
    size_t id = 0;
    HalfEdge* twin = nullptr;
    HalfEdge* previous = nullptr;
    HalfEdge* next = nullptr;
    HalfEdge* previousPoly = nullptr;
    HalfEdge* nextPoly = nullptr;

    vec3 source, destination;

    HalfEdge *getTriangleRec(vector<HalfEdge*> *edgeList, size_t *n);
    void insertTriangle(vector<vec3> *destination);

public:
    [[nodiscard]] HalfEdge *getPrevious() const;
    HalfEdge * setPrevious(HalfEdge *previous);

    [[nodiscard]] HalfEdge *getNext() const;
    HalfEdge * setNext(HalfEdge *next);

    [[nodiscard]] HalfEdge *get_next_outline() const;
    void set_next_outline(HalfEdge *next);
    void set_next_outline(vec3 destination);

    [[nodiscard]] HalfEdge *get_previous_outline() const;
    HalfEdge * set_previous_outline(HalfEdge *next);
    void set_previous_outline(vec3 source);

    HalfEdge *get_outline(int index);

    [[nodiscard]] HalfEdge *getTwin() const;
    HalfEdge * set_twin(HalfEdge *twin);
    HalfEdge * set_twin();

    vec3 get_source();
    void set_source(vec3 source);

    vec3 get_destination();
    void set_destination(vec3 destination);

    bool is_outline();
    size_t outline_size();

    void combine(HalfEdge *edge);

    HalfEdge(HalfEdge *previous, vec3 dest);

    HalfEdge(vec3 source, vec3 dest);

    static HalfEdge *create_outline(vector<vec3> points);

    HalfEdge *mirror(vec3 direction, vec3 scale);

    HalfEdge *extend(vec3 direction, vec3 scale=vec3(1));

    HalfEdge *getTriangle(size_t n);

    HalfEdge *traverse(const char direction[]);

    HalfEdge *getFirst();

    void insertPolygon(vector<vec3> *destination);

    HalfEdge * fill();

    static HalfEdge *connect(HalfEdge *first, HalfEdge *second);
    HalfEdge *connect_outline(HalfEdge *edge);

    static HalfEdge *create_outline_fill(vector<vec3> points);

    static HalfEdge *create_outline_fill(HalfEdge *pEdge, HalfEdge *pEdge1);

    HalfEdge *operator[](int i);

    HalfEdge *extend_fill(vec3 direction, vec3 scale);

    HalfEdge *copy(vec3 direction, vec3 scale);

    HalfEdge *twin_outline();
};

#endif //ODDITYENGINE_HALFEDGE_H