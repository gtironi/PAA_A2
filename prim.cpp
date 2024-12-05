#include "prim.h"

void initialize(GraphAdjList& graph, int* parent, bool* inTree, int* vertexCost) {
    int numVertices = graph.numVertices();
    for (int v = 0; v < numVertices; v++) {
        parent[v] = -1;
        inTree[v] = false;
        vertexCost[v] = INT_MAX;
    }
    parent[0] = 0;
    inTree[0] = true;

    EdgeNode* edge = graph.getEdges(0);
    while (edge) {
        int v2 = edge->otherVertex();
        parent[v2] = 0;
        vertexCost[v2] = edge->length();
        edge = edge->next();
    }
}

void Prim::mst(GraphAdjList& graph, int* parent) {
    int numVertices = graph.numVertices();
    bool inTree[numVertices];
    int vertexCost[numVertices];

    initialize(graph, parent, inTree, vertexCost);

    MinHeap heap;
    for (int v = 1; v < numVertices; v++) {
        heap.insert_or_update(vertexCost[v], v);
    }

    while (!heap.empty()) {
        int v1 = heap.extract_min();
        heap.pop();
        if (vertexCost[v1] == INT_MAX) break;

        inTree[v1] = true;

        EdgeNode* edge = graph.getEdges(v1);
        while (edge) {
            int v2 = edge->otherVertex();
            int cost = edge->length();
            if (!inTree[v2] && cost < vertexCost[v2]) {
                vertexCost[v2] = cost;
                parent[v2] = v1;
                heap.insert_or_update(vertexCost[v2], v2);
            }
            edge = edge->next();
        }
    }
}
