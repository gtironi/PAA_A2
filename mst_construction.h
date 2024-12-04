#ifndef MST_CONSTRUCTION_H
#define MST_CONSTRUCTION_H

#include "graph.h"
#include <unordered_map>
#include <string>
#include <vector>

class MSTConstruction {
public:
    // Method to construct the MST connecting bus stops
    static void constructMST(GraphAdjList& graph,
                             const std::unordered_map<std::string, vertex>& busStops,
                             std::vector<std::pair<vertex, vertex>>& mstEdges);
};

#endif // MST_CONSTRUCTION_H
