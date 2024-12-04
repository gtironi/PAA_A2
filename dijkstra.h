#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"
#include "heap.h"
#include <vector>
#include <unordered_set>

class Dijkstra {
public:
    static void compute(GraphAdjList& graph, int start, int* parent, int* distance,
                        bool useCost = true,
                        const std::unordered_set<vertex>& allowedVertices = std::unordered_set<vertex>());
};

#endif // DIJKSTRA_H
