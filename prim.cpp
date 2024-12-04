#include "prim.h"
#include <climits>
#include <vector>

void Prim::mst(GraphAdjList& graph, std::vector<int>& parent) {
    int numVertices = graph.numVertices();
    std::vector<int> key(numVertices, INT_MAX);
    std::vector<bool> inMST(numVertices, false);
    MinHeap heap;

    parent.assign(numVertices, -1);

    key[0] = 0;
    parent[0] = 0;
    heap.insert_or_update(key[0], 0);

    while (!heap.empty()) {
        int u = heap.extract_min();
        heap.pop();
        inMST[u] = true;

        EdgeNode* edge = graph.getEdges(u);
        while (edge != nullptr) {
            int v = edge->otherVertex();
            int weight = edge->cost();

            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                heap.insert_or_update(key[v], v);
            }

            edge = edge->next();
        }
    }
}
