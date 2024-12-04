#include "bus_stop_selection.h"
#include "dijkstra.h"
#include <unordered_set>
#include <climits>
#include <vector>
#include <iostream>

void BusStopSelection::selectBusStops(GraphAdjList& graph,
                                      std::unordered_map<std::string, vertex>& busStops) {
    // Map neighborhoods to their vertices
    std::unordered_map<std::string, std::vector<vertex>> bairroVertices;

    int numVertices = graph.numVertices();
    for (vertex v = 0; v < numVertices; ++v) {
        std::string bairro = graph.getVertexBairro(v);
        if (!bairro.empty()) {
            bairroVertices[bairro].push_back(v);
        }
    }

    // For each neighborhood, find the vertex with the smallest maximum distance
    for (const auto& pair : bairroVertices) {
        const std::string& bairro = pair.first;
        const std::vector<vertex>& vertices = pair.second;

        vertex optimalVertex = -1;
        int minMaxDistance = INT_MAX;

        std::unordered_set<vertex> allowedVertices(vertices.begin(), vertices.end());

        for (vertex v : vertices) {
            std::vector<int> parent(numVertices, -1);
            std::vector<int> distance(numVertices, INT_MAX);

            Dijkstra::compute(graph, v, parent.data(), distance.data(), /*useCost=*/false, allowedVertices);

            int maxDistance = 0;
            for (vertex u : vertices) {
                if (distance[u] > maxDistance) {
                    maxDistance = distance[u];
                }
            }

            if (maxDistance < minMaxDistance) {
                minMaxDistance = maxDistance;
                optimalVertex = v;
            }
        }

        if (optimalVertex != -1) {
            busStops[bairro] = optimalVertex;
        } else {
            std::cerr << "Could not find a bus stop for neighborhood " << bairro << "\n";
        }
    }
}
