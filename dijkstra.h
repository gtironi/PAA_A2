#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"
#include "heap.h"
#include <unordered_set>
#include <vector>

class Dijkstra {
public:
    static void compute(GraphAdjList& graph, int start, std::vector<int>& parent, std::vector<int>& distance,
                        bool useCost = true,
                        const std::unordered_set<vertex>& allowedVertices = std::unordered_set<vertex>());

    // New method to reconstruct the path from start to end
    static std::vector<vertex> reconstructPath(int start, int end, const std::vector<int>& parent);
};

#endif // DIJKSTRA_H
