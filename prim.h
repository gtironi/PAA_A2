#ifndef PRIM_H
#define PRIM_H

#include "graph.h"
#include "heap.h"

#include <climits> // Para INT_MAX

class Prim {
public:
    static void mst(GraphAdjList& graph, int* parent);
};

#endif // PRIM_H
