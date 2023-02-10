#ifndef ODDITYENGINE_HALFEDGE_H
#define ODDITYENGINE_HALFEDGE_H

#include <vector>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
using namespace glm;

#include <cstddef>
#include <iterator>

class HalfEdge {
private:
    HalfEdge* twin = nullptr;
    HalfEdge* previous = nullptr;
    HalfEdge* next = nullptr;

    HalfEdge *getTriangleRec(size_t *n);
    void insertTriangle(vector<vec3> *destination);

public:
    vec3 source, dest;

    HalfEdge *getPrevious() const;
    void setPrevious(HalfEdge *previous);

    HalfEdge *getNext() const;
    void setNext(HalfEdge *next);

    HalfEdge *getTwin() const;
    void setTwin(HalfEdge *twin);
    bool hasTwin();

    HalfEdge(HalfEdge *previous, vec3 dest);

    HalfEdge(vec3 source, vec3 dest);

    static HalfEdge * addPolygon(HalfEdge *twin, vector<vec3> points);

    static HalfEdge * addPolygon(vector<vec3> points);

    HalfEdge *getTriangle(size_t n);

    HalfEdge *getFirst();

    void insertPolygon(vector<vec3> *destination);

    /// Test ///
    static void testCreation(size_t pointCount, float radius);
    static void testGetting(size_t pointCount);
    static void testInsert(size_t pointCount);
};

#endif //ODDITYENGINE_HALFEDGE_H