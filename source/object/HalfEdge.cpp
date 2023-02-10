#include <algorithm>
#include "HalfEdge.h"


/// Constructors ///
HalfEdge::HalfEdge(HalfEdge *previous, vec3 dest) : previous(previous), source(previous->dest), dest(dest) {}

HalfEdge::HalfEdge(vec3 source, vec3 dest) : source(source), dest(dest) {}

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
        this->twin = twin;
        twin->twin = this;
    }
}

void HalfEdge::setPrevious(HalfEdge *previous) {
    this->previous = previous;
}

void HalfEdge::setNext(HalfEdge *next) {
    this->next = next;
    next->setPrevious(this);
}

/// Creators ///
HalfEdge *HalfEdge::addPolygon(vector<vec3> points) {
    HalfEdge* edge = nullptr;
    if (points.size() == 3) {
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
        vector<vec3> midpoints;
        copy_if(points.begin(), points.end(), back_inserter(midpoints), [&toggle](vec3){return toggle = !toggle;});
        auto * firstEdge = addPolygon(midpoints);
        edge = firstEdge;
        for (size_t i = 0; i < points.size() - 1; i += 2) {
            t = addPolygon(vector{points[i], points[(i + 1) % points.size()], points[(i + 2) % points.size()]});
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

HalfEdge *HalfEdge::addPolygon(HalfEdge *twin, vector<vec3> points) {
    vector<vec3> polyPoints = {twin->dest, twin->source};
    for (auto p : points) {
        polyPoints.emplace_back(p);
    }
    twin->setTwin(addPolygon(polyPoints));
    return twin->twin;
}

/// Traversal ///
HalfEdge *HalfEdge::getTriangleRec(size_t *n) {
    HalfEdge *tn = nullptr, *tp = nullptr;
    if(*n == 0) {
        return this->getFirst();
    }
    if (*n > 0 && this->next->twin) {
        *n -= 1;
        tn = this->next->twin->getTriangleRec(n);
    }
    if (*n > 0 && this->previous->twin) {
        *n -= 1;
        tp = this->previous->twin->getTriangleRec(n);
    }

    return tn ? tn : tp;
}

HalfEdge *HalfEdge::getTriangle(size_t n) {
    HalfEdge *tn = nullptr, *tp = nullptr;
    if(n == 0) {
        return this->getFirst();
    }
    if (n > 0 && this->twin) {
        n -= 1;
        tn = this->twin->next->getTriangleRec(&n);
    }
    if (n > 0 && this->next->twin) {
        n -= 1;
        tn = this->next->twin->getTriangleRec(&n);
    }
    if (n > 0 && this->previous->twin) {
        n -= 1;
        tp = this->previous->twin->getTriangleRec(&n);
    }

    return tn ? tn : tp;
}

HalfEdge *HalfEdge::getFirst() {
    auto *start = this;
    while (length(start->next->source) < length(start->source)) {
        start = start->next;
    }
    while (length(start->previous->source) < length(start->source)) {
        start = start->previous;
    }
    return start;
}

void HalfEdge::insertPolygon(vector<vec3> *destination) {
    size_t i = 1;
    auto *tri = this;
    while (tri) {
        tri->insertTriangle(destination);
        tri = this->getTriangle(i++);
    }
}

void HalfEdge::insertTriangle(vector<vec3> *destination) {
    destination->emplace_back(this->source);
    destination->emplace_back(this->dest);
    destination->emplace_back(this->next->dest);
}

/// Test ///
void HalfEdge::testCreation(size_t pointCount, float radius) {
    vector<vec3> points;
    vector<vec3> pointI;
    for (size_t i = 0; i < pointCount; i++) {
        points.emplace_back(sin(i * 2 * pi<float>() / pointCount), cos(i * 2 * pi<float>() / pointCount), radius);
        pointI.emplace_back(points.back());
    }

    HalfEdge::addPolygon(pointI);
}

void HalfEdge::testGetting(size_t pointCount) {
    vector<vec3> points;
    vector<vec3> pointI;
    for (size_t i = 0; i < pointCount; i++) {
        points.emplace_back(sin(i * 2 * pi<float>() / pointCount), cos(i * 2 * pi<float>() / pointCount), 0.0f);
        pointI.emplace_back(points.back());
    }

    auto * he = HalfEdge::addPolygon(pointI);
    for (size_t i = 0; i < points.size(); i++) {
        printf("Triangle %zd", i);
        auto *r = he->getTriangle(i);
        if (r) {
            printf(": %1.0f -> %1.0f -> %1.0f\n", r->source, r->dest, r->next->dest);
        }
        else {
            printf(": doen't exist\n");
        }
    }
}

void HalfEdge::testInsert(size_t pointCount) {
    vector<vec3> points;
    vector<vec3> pointI;
    for (size_t i = 0; i < pointCount; i++) {
        points.emplace_back(sin(i * 2 * pi<float>() / pointCount), cos(i * 2 * pi<float>() / pointCount), 0.0f);
        pointI.emplace_back(points.back());
    }

    auto * he = HalfEdge::addPolygon(pointI);

    vector<vec3> result;

    he->insertPolygon(&result);
}

bool HalfEdge::hasTwin() {
    if (twin != nullptr) {
        return true;
    }
    return false;
}

