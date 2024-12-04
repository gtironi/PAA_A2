#include "mst_construction.h"
#include "dijkstra.h"
#include "prim.h"
#include <unordered_map>
#include <vector>
#include <climits>
#include <set>

void MSTConstruction::constructMST(GraphAdjList& graph,
                                   const std::unordered_map<std::string, vertex>& busStops,
                                   std::vector<std::pair<vertex, vertex>>& mstEdges,
                                   std::set<std::pair<vertex, vertex>>& finalEdges) {
    // Map bus stops to indices
    std::unordered_map<vertex, int> busStopIndices;
    std::vector<vertex> busStopList;
    int index = 0;
    for (const auto& pair : busStops) {
        busStopList.push_back(pair.second);
        busStopIndices[pair.second] = index++;
    }

    int numBusStops = busStopList.size();

    // Matrix of costs and paths between bus stops
    std::vector<std::vector<int>> costMatrix(numBusStops, std::vector<int>(numBusStops, INT_MAX));
    std::vector<std::vector<std::vector<vertex>>> pathsMatrix(numBusStops, std::vector<std::vector<vertex>>(numBusStops));

    // Compute minimum excavation costs and paths between all pairs of bus stops
    for (int i = 0; i < numBusStops; ++i) {
        std::vector<int> parent(graph.numVertices(), -1);
        std::vector<int> distance(graph.numVertices(), INT_MAX);

        Dijkstra::compute(graph, busStopList[i], parent, distance, /*useCost=*/true);

        for (int j = 0; j < numBusStops; ++j) {
            if (i != j) {
                costMatrix[i][j] = distance[busStopList[j]];
                // Reconstruct path
                std::vector<vertex> path = Dijkstra::reconstructPath(busStopList[i], busStopList[j], parent);
                pathsMatrix[i][j] = path;
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

    // Collect the MST edges and expand to actual paths
    for (int i = 0; i < numBusStops; ++i) {
        if (parent[i] != -1 && parent[i] != i) {
            int u_index = i;
            int v_index = parent[i];
            vertex u = busStopList[u_index];
            vertex v = busStopList[v_index];

            mstEdges.push_back({u, v});

            // Get the path between u and v
            std::vector<vertex> path = pathsMatrix[u_index][v_index];

            // Convert path to edges and add to finalEdges
            for (size_t k = 0; k < path.size() - 1; ++k) {
                vertex from = path[k];
                vertex to = path[k + 1];
                if (from > to) std::swap(from, to); // Ensure consistent ordering
                finalEdges.insert({from, to});
            }
        }
    }
}
