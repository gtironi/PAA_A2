#ifndef MST_CONSTRUCTION_H
#define MST_CONSTRUCTION_H

#include "graph.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <set>

class MSTConstruction {
public:
    // Modified method to construct the MST and collect final edges
    static void constructMST(GraphAdjList& graph,
                             const std::unordered_map<std::string, vertex>& busStops,
                             std::vector<std::pair<vertex, vertex>>& mstEdges,
                             std::set<std::pair<vertex, vertex>>& finalEdges);
};

#endif // MST_CONSTRUCTION_H
