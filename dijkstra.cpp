#include "dijkstra.h"
#include <climits> // Para INT_MAX

void Dijkstra::compute(GraphAdjList& graph, int start, int* parent, int* distance) {
    int numVertices = graph.numVertices();
    bool checked[numVertices];
    MinHeap heap;

    for (int v = 0; v < numVertices; v++) {
        parent[v] = -1;
        distance[v] = INT_MAX;
        checked[v] = false;
    }
    parent[start] = start;
    distance[start] = 0;

    heap.insert_or_update(distance[start], start);

    while (!heap.empty()) {
        int v1 = heap.extract_min();
        heap.pop();
        if (distance[v1] == INT_MAX) break;
        
        EdgeNode* edge = graph.getEdges(v1);
        while (edge) {
            int v2 = edge->otherVertex();
            if (!checked[v2]) {
                int cost = edge->cost();
                if (distance[v1] + cost < distance[v2]) {
                    parent[v2] = v1;
                    distance[v2] = distance[v1] + cost;
                    heap.insert_or_update(distance[v2], v2);
                }
            }
            edge = edge->next();
        }
        checked[v1] = true;
    }
}
