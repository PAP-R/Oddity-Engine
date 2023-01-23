//
// Created by jan on 23/01/2023.
//

#ifndef ODDITYENGINE_HALFEDGE_H
#define ODDITYENGINE_HALFEDGE_H


class HalfEdge {
    HalfEdge* twin;
    size_t head, tail;
};


#endif //ODDITYENGINE_HALFEDGE_H
