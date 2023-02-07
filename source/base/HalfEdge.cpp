#include "HalfEdge.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <algorithm>
#include <iterator>
#include <numeric>

using namespace glm;

/// Constructors ///
HalfEdge::HalfEdge(HalfEdge *previous, size_t dest) : previous(previous), source(previous->dest), dest(dest) {}

HalfEdge::HalfEdge(size_t source, size_t dest) : source(source), dest(dest) {
    //printf("%d -> %d\n", source, dest);
}

/// Getters ///
HalfEdge *HalfEdge::getTwin() const {
    return twin;
}

HalfEdge *HalfEdge::getPrevious() const {
    return previous;
}

HalfEdge *HalfEdge::getNext() const {
    return next;
}

/// Setters ///
void HalfEdge::setTwin(HalfEdge *twin) {
    if (twin && this->source == twin->dest && this->dest == twin->source) {
        HalfEdge::twin = twin;
        twin->twin = this;
        printf("== Twins: [\n\t%d -> %d\n\t%d <- %d\n]\n", this->source, this->dest, twin->dest, twin->source);
    }
    else if (twin) {
        printf("== Not Twins: [\n\t%d -> %d\n\t%d <- %d\n]\n", this->source, this->dest, twin->dest, twin->source);
    }
    else {
        printf("== Not Twins: [\n\t%d -> %d\n\tNULL\n]\n", this->source, this->dest);
    }
}

void HalfEdge::setPrevious(HalfEdge *previous) {
    HalfEdge::previous = previous;
}

void HalfEdge::setNext(HalfEdge *next) {
    HalfEdge::next = next;
    next->setPrevious(this);
}

/// Creators ///
HalfEdge *HalfEdge::addPolygon(vector<size_t> points) {
    HalfEdge* edge = nullptr;
    if (points.size() == 3) {
        printf("\t%d -> %d -> %d\n", points[0], points[1], points[2]);
        edge = new HalfEdge(points[0], points[1]);
        auto* t = edge;
        for (size_t i = 1; i < points.size(); i++) {
            t->setNext(new HalfEdge(points[i], points[(i + 1) % points.size()]));
            t = t->next;
        }

        t->setNext(edge);

        return edge;
    }

    else if (points.size() > 3) {
        HalfEdge * t;
        bool toggle = false;
        vector<size_t> midpoints;
        copy_if(points.begin(), points.end(), back_inserter(midpoints), [&toggle](int){return toggle = !toggle;});
        auto * firstEdge = addPolygon(midpoints);
        edge = firstEdge;
        for (size_t i = 0; i < points.size() - 1; i += 2) {
            t = addPolygon(vector<size_t>{points[i], points[(i + 1) % points.size()], points[(i + 2) % points.size()]});
            if (edge == nullptr) {
                firstEdge = t;
                edge = t->next;
            }
            else if (edge != t && t->previous) {
                edge->setTwin(t->previous);
            }
            edge = edge->next;
            while(edge->twin && edge->twin->next) {
                edge = edge->twin->next;
            }
        }

        while (firstEdge && firstEdge->twin && firstEdge->twin->next) {
            firstEdge = firstEdge->twin->next;
        }

        return firstEdge;
    }

    return nullptr;
}

HalfEdge *HalfEdge::addPolygon(HalfEdge *twin, vector<size_t> points) {
    vector<size_t> polyPoints = {twin->dest, twin->source};
    for (auto p : points) {
        polyPoints.emplace_back(p);
    }
    twin->setTwin(addPolygon(polyPoints));
    return twin->twin;
}

/// Test ///

void HalfEdge::test(size_t pointCount, float radius) {
    vector<vec3> points;
    vector<size_t> pointI;
    for (size_t i = 0; i < pointCount; i++) {
        points.emplace_back(sin(i * 2 * pi<float>() / pointCount), cos(i * 2 * pi<float>() / pointCount), radius);
        pointI.emplace_back(i);
    }

    HalfEdge::addPolygon(pointI);
}

