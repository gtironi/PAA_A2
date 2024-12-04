#include "mst_construction.h"
#include "dijkstra.h"
#include "prim.h"
#include <unordered_map>
#include <vector>
#include <climits>

void MSTConstruction::constructMST(GraphAdjList& graph,
                                   const std::unordered_map<std::string, vertex>& busStops,
                                   std::vector<std::pair<vertex, vertex>>& mstEdges) {
    // Map bus stops to indices
    std::unordered_map<vertex, int> busStopIndices;
    std::vector<vertex> busStopList;
    int index = 0;
    for (const auto& pair : busStops) {
        busStopList.push_back(pair.second);
        busStopIndices[pair.second] = index++;
    }

    int numBusStops = busStopList.size();

    // Matrix of costs between bus stops
    std::vector<std::vector<int>> costMatrix(numBusStops, std::vector<int>(numBusStops, INT_MAX));

    // Compute minimum excavation costs between all pairs of bus stops
    for (int i = 0; i < numBusStops; ++i) {
        std::vector<int> parent(graph.numVertices(), -1);
        std::vector<int> distance(graph.numVertices(), INT_MAX);

        Dijkstra::compute(graph, busStopList[i], parent.data(), distance.data(), /*useCost=*/true);

        for (int j = 0; j < numBusStops; ++j) {
            if (i != j) {
                costMatrix[i][j] = distance[busStopList[j]];
            }
        }
    }

    // Create a complete graph of bus stops
    GraphAdjList busStopGraph(numBusStops);
    for (int i = 0; i < numBusStops; ++i) {
        for (int j = i + 1; j < numBusStops; ++j) {
            int cost = costMatrix[i][j];
            busStopGraph.addEdge(i, j, "", cost, 0, false);
        }
    }

    // Apply Prim's algorithm
    std::vector<int> parent;
    Prim::mst(busStopGraph, parent);

    // Collect the MST edges
    for (int i = 0; i < numBusStops; ++i) {
        if (parent[i] != -1 && parent[i] != i) {
            vertex u = busStopList[i];
            vertex v = busStopList[parent[i]];
            mstEdges.push_back({u, v});
        }
    }
}
