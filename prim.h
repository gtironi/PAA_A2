#ifndef PRIM_H
#define PRIM_H

#include "graph.h"
#include "heap.h"
#include <vector>

class Prim {
public:
    static void mst(GraphAdjList& graph, std::vector<int>& parent);
};

#endif // PRIM_H
