#ifndef ODDITYENGINE_HALFEDGE_H
#define ODDITYENGINE_HALFEDGE_H

#include <vector>
using namespace std;

#include <cstddef>

class HalfEdge {
private:
    HalfEdge* twin = nullptr;
    HalfEdge* previous = nullptr;
    HalfEdge* next = nullptr;

public:
    size_t source, dest;

    HalfEdge *getPrevious() const;
    void setPrevious(HalfEdge *previous);

    HalfEdge *getNext() const;
    void setNext(HalfEdge *next);

    HalfEdge *getTwin() const;
    void setTwin(HalfEdge *twin);

    HalfEdge(HalfEdge *previous, size_t dest);

    HalfEdge(size_t source, size_t dest);

    static HalfEdge *addPolygon(HalfEdge *twin, vector<size_t> points);

    static HalfEdge *addPolygon(vector<size_t> points);

    /// Test ///
    static void test(size_t pointCount, float radius);
};


#endif //ODDITYENGINE_HALFEDGE_H
