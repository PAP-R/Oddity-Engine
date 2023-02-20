#include <algorithm>
#include "HalfEdge.h"
#include "source/base/tools/Debug.h"

#include <fmt/core.h>
#include <iostream>

using namespace fmt;

size_t nextID = 0;
vector<HalfEdge*> edges;
/// Constructors ///
HalfEdge::HalfEdge(HalfEdge *previous, vec3 destination) : previous(previous), source(previous->destination), destination(destination) {id = nextID++; edges.emplace_back(this);}

HalfEdge::HalfEdge(vec3 source, vec3 destination) : source(source), destination(destination) {id = nextID++; edges.emplace_back(this);}

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
HalfEdge *HalfEdge::set_twin(HalfEdge *twin) {
    if (twin && this->source == twin->destination && this->destination == twin->source) {
//        if (this->twin) {
//            this->twin->combine(twin);
//        }
//        else {
            this->twin = twin;
            twin->twin = this;
//        }
    }

    return twin;
}

HalfEdge *HalfEdge::set_twin() {
    return this->twin ? this->twin : set_twin(new HalfEdge(this->destination, this->source));
}

HalfEdge *HalfEdge::setPrevious(HalfEdge *previous) {
    this->previous = previous;
    if (previous) previous->next = this;
    return this;
}

HalfEdge *HalfEdge::setNext(HalfEdge *next) {
    this->next = next;
    if (next) next->previous = this;
    return this;
}

/// Creators ///
HalfEdge *HalfEdge::create_outline(vector<vec3> points) {
    if (points.size() < 3) {
        return nullptr;
    }

    auto *edge = new HalfEdge(points[0], points[1]);
    auto t = edge;

    for (size_t i = 1; i < points.size(); i++) {
        t->set_next_outline(points[(i + 1) % points.size()]);
        t->setNext(t->nextPoly);
        t = t->get_next_outline();
    }

    t->set_next_outline(edge);
    t->setNext(edge);

    return edge;
}

HalfEdge *HalfEdge::fill() {
    auto outline = this;
    auto outlineSize = outline->outline_size();
    HalfEdge *t;
    HalfEdge *last;
    while (outlineSize > 4) {
        last = nullptr;
        for (size_t i = 1; i < outlineSize; i += 2) {
            t = connect(outline->get_outline(i - 1), outline->get_outline(i))->set_twin();
            t->setPrevious(last);
            t->set_previous_outline(last);
            last = t;
        }
        outline = outline->previous->twin;
        while (t->twin) t = t->twin->previous;
        if (t == this->previousPoly) {
            last->setNext(outline);
            last->set_next_outline(outline);
        }
        else {
            last->setNext(this->previousPoly);
            outline->setPrevious(this->previousPoly);
        }

        outlineSize = outline->outline_size();

    }

    if (outlineSize == 4) {
        connect(outline, outline->get_next_outline())->set_twin(connect(outline->get_outline(2), outline->get_outline(3)));
    }

    return this;
}

HalfEdge *HalfEdge::create_outline_fill(vector<vec3> points) {
    auto t = create_outline(points);
    t->fill();
    return t;
}

HalfEdge *HalfEdge::connect(HalfEdge *first, HalfEdge *second) {
    auto t = new HalfEdge(second->destination, first->source);
    if (first->get_destination() == second->get_source()) {
        first->setNext(second);
        first->setPrevious(t);
        second->setNext(t);
    }

    return t;
}

/// Traversal ///
HalfEdge *HalfEdge::getTriangleRec(vector<HalfEdge*> *edgeList, size_t *n) {
    HalfEdge *tn = nullptr, *tp = nullptr;
    edgeList->emplace_back(this);
    edgeList->emplace_back(this->previous);
    edgeList->emplace_back(this->next);
    auto t = this;
    if (*n == 0) {
        return this;
    }
    for (size_t i = 0; i < 2 && *n > 0; i++) {
        t = t->next;
        if (t->twin && find(edgeList->begin(), edgeList->end(), t->twin) == edgeList->end()) {
            *n -= 1;
            tp = t->twin->getTriangleRec(edgeList, n);
            tn = tp ? tp : tn;
        }
    }

    return tn;
}

HalfEdge *HalfEdge::getTriangle(size_t n) {
    HalfEdge *tn = nullptr, *tp = nullptr;
    vector<HalfEdge*> edgeList{this};
    tn = getTriangleRec(&edgeList, &n);
    if (n > 0 && this->twin && find(edgeList.begin(), edgeList.end(), this->twin) == edgeList.end()) {
        n -= 1;
        tn = tn ? tn : this->twin->getTriangleRec(&edgeList, &n);
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

template <> struct fmt::formatter<vec3> {
    char presentation = 'v';

    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        auto it = ctx.begin(), end = ctx.end();
        if (it != end && (*it == 'v')) presentation = *it++;
        return it;
    }

    template <typename FormatContext>
    auto format(const vec3& p, FormatContext& ctx) const -> decltype(ctx.out()) {
        // ctx.out() is an output iterator to write to.
        return fmt::format_to(ctx.out(), "[{:.1f} {:.1f} {:.1f}]", p.x, p.y, p.z);
    }
};

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
    destination->emplace_back(this->destination);
    destination->emplace_back(this->next->destination);
}

vec3 HalfEdge::get_source() {
    return this->source;
}

void HalfEdge::set_source(vec3 source) {
    this->source = source;
    if (this->twin)
        this->twin->destination = source;
    if (this->previous)
        this->previous->destination = source;
    if (this->previousPoly)
        this->previousPoly->destination = source;
}

vec3 HalfEdge::get_destination() {
    return this->destination;
}

void HalfEdge::set_destination(vec3 destination) {
    this->destination = destination;
    if (this->twin)
        this->twin->source = destination;
    if (this->next)
        this->next->source = destination;
    if (this->next)
        this->nextPoly->source = destination;
}

bool HalfEdge::is_outline() {
    return this->previousPoly && this->nextPoly;
}

HalfEdge *HalfEdge::traverse(const char direction[]) {
    auto *t = this;
    for (size_t i = 0; direction[i] != '\0' && t != nullptr; i++) {
        switch(direction[i]) {
            case 't':
                t = t->twin;
                break;
            case 'p':
                t = t->previous;
                break;
            case 'P':
                t = t->previousPoly;
                break;
            case 'n':
                t = t->next;
                break;
            case 'N':
                t = t->previousPoly;
                break;
        }
    }

    return t;
}

HalfEdge *HalfEdge::get_next_outline() const {
    return this->nextPoly;
}

void HalfEdge::set_next_outline(HalfEdge *next) {
    this->nextPoly = next;
    if (next) next->previousPoly = this;
}

void HalfEdge::set_next_outline(vec3 destination) {
    set_next_outline(new HalfEdge(this->destination, destination));
}

size_t HalfEdge::outline_size() {
    if (!is_outline()) {
        return 0;
    }

    size_t s = 1;

    auto t = this->nextPoly;

    for (s; t != this && t != nullptr; s++) {
        t = t->nextPoly;
    }

    return s;
}

HalfEdge *HalfEdge::get_outline(int index) {
    if (index == 0)
        return this;

    auto t = this;

    if (index > 0) {
        for (int i = 0; i < index; i++) {
            t = t->nextPoly;
        }
    }

    else {
        for (int i = 0; i > index; i--) {
            t = t->previousPoly;
        }
    }

    return t;
}

HalfEdge *HalfEdge::mirror(vec3 direction, vec3 scale) {
    auto t = this;

    vector<vec3> points;
    do {
        points.emplace_back((t->destination + direction) * scale);
        t = t->get_previous_outline();
    } while (t != this);

    return create_outline(points);
}

HalfEdge *HalfEdge::copy(vec3 direction, vec3 scale) {
    auto t = this;

    vector<vec3> points;
    do {
        points.emplace_back((t->source + direction) * scale);
        t = t->get_next_outline();
    } while (t != this);

    return create_outline(points);
}

HalfEdge *HalfEdge::extend(vec3 direction, vec3 scale) {
    auto t = this;
    auto tCopy = this->copy(direction, scale);
    HalfEdge *tCopyNext;

    do {
        tCopyNext = tCopy->get_next_outline();
        t->set_twin(HalfEdge::create_outline_fill(t->set_twin(), tCopy));
        t = t->get_next_outline();
        tCopy->set_next_outline(tCopyNext);
        tCopy = tCopyNext;
    } while (t != this);

    return tCopy;
}

HalfEdge *HalfEdge::extend_fill(vec3 direction, vec3 scale) {
    auto t = this;
    auto tMirrored = this->mirror(direction, scale)->fill();

    do {
        t->set_twin(HalfEdge::create_outline_fill(t, tMirrored));
        t = t->get_next_outline();
        tMirrored = tMirrored->get_previous_outline();
    } while (t != this);

    return this;
}

HalfEdge *HalfEdge::get_previous_outline() const {
    return previousPoly;
}

HalfEdge *HalfEdge::set_previous_outline(HalfEdge *previous) {
    this->previousPoly = previous;
    if (previous) previous->nextPoly = this;
    return this;
}

void HalfEdge::set_previous_outline(vec3 source) {
    set_previous_outline(new HalfEdge(source, this->source));
}

HalfEdge *HalfEdge::connect_outline(HalfEdge *edge) {
    auto t = new HalfEdge(this->destination, edge->source);

    this->set_next_outline(t);
    edge->set_previous_outline(t);

    return this;
}

HalfEdge *HalfEdge::create_outline_fill(HalfEdge *edge, HalfEdge *opposite) {
    edge->connect_outline(opposite);
    opposite->connect_outline(edge);
    edge->fill();
    return edge;
}

HalfEdge *HalfEdge::operator[](int index) {
    auto t = this;
    if (index > 0) {
        for (int i = 0; i < index; i++) {
            t = t->getNext();
        }
    }
    else if (index < 0) {
        for (int i = 0; i > index; i--) {
            t = t->getPrevious();
        }
    }
    return t;
}

void HalfEdge::combine(HalfEdge *edge) {
    if (!this->twin) this->set_twin(edge->getTwin());
    if (!this->previous) this->setPrevious(edge->getPrevious());
    if (!this->previousPoly) this->set_previous_outline(edge->get_previous_outline());
    if (!this->next) this->setNext(edge->getNext());
    if (!this->nextPoly) this->set_next_outline(edge->get_next_outline());
    delete edge;
}
