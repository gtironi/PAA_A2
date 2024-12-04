#include "dijkstra.h"
#include <climits> // For INT_MAX
#include <vector>

void Dijkstra::compute(GraphAdjList& graph, int start, int* parent, int* distance,
                       bool useCost,
                       const std::unordered_set<vertex>& allowedVertices) {
    int numVertices = graph.numVertices();
    std::vector<bool> checked(numVertices, false);
    MinHeap heap;

    for (int v = 0; v < numVertices; v++) {
        parent[v] = -1;
        distance[v] = INT_MAX;
    }
    parent[start] = start;
    distance[start] = 0;

    heap.insert_or_update(distance[start], start);

    while (!heap.empty()) {
        int v1 = heap.extract_min();
        heap.pop();

        if (distance[v1] == INT_MAX) break;
        if (!allowedVertices.empty() && allowedVertices.find(v1) == allowedVertices.end()) continue;

        EdgeNode* edge = graph.getEdges(v1);
        while (edge) {
            int v2 = edge->otherVertex();

            if (!allowedVertices.empty() && allowedVertices.find(v2) == allowedVertices.end()) {
                edge = edge->next();
                continue;
            }

            if (!checked[v2]) {
                int weight = useCost ? edge->cost() : 1; // Use cost or distance
                if (distance[v1] + weight < distance[v2]) {
                    parent[v2] = v1;
                    distance[v2] = distance[v1] + weight;
                    heap.insert_or_update(distance[v2], v2);
                }
            }
            edge = edge->next();
        }
        checked[v1] = true;
    }
}
