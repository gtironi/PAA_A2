#include <iostream>
#include <cassert>
#include "graph.h"
#include "dijkstra.h"
#include "prim.h"

void testDijkstra() {
    // Create test graph
    GraphAdjList graph(6);

    // Add edges
    graph.addEdge(0, 1, "Centro", 4, 1, true);
    graph.addEdge(0, 2, "Centro", 2, 1, true);
    graph.addEdge(1, 3, "Centro", 5, 1, true);
    graph.addEdge(2, 3, "Centro", 8, 1, true);
    graph.addEdge(3, 4, "Centro", 6, 1, true);
    graph.addEdge(4, 5, "Centro", 3, 1, true);

    // Expected results
    int expectedParent[6] = {0, 0, 0, 1, 3, 4};
    int expectedDistance[6] = {0, 4, 2, 9, 15, 18};

    // Run Dijkstra
    int parent[6];
    int distance[6];
    Dijkstra::compute(graph, 0, parent, distance);

    // Verify results
    for (int i = 0; i < 6; i++) {
        assert(parent[i] == expectedParent[i]);
        assert(distance[i] == expectedDistance[i]);
    }

    std::cout << "Dijkstra test passed!\n";
}

void testPrim() {
    // Create test graph
    GraphAdjList graph(6);

    // Add edges
    graph.addEdge(0, 1, "Centro", 4, 1, true);
    graph.addEdge(0, 2, "Centro", 2, 1, true);
    graph.addEdge(1, 3, "Centro", 5, 1, true);
    graph.addEdge(2, 3, "Centro", 8, 1, true);
    graph.addEdge(3, 4, "Centro", 6, 1, true);
    graph.addEdge(4, 5, "Centro", 3, 1, true);

    // Expected results (Prim MST forms a minimum cost tree)
    int expectedParent[6] = {0, 0, 0, 1, 3, 4};

    // Run Prim
    std::vector<int> parent;
    Prim::mst(graph, parent);

    // Verify results
    for (int i = 0; i < 6; i++) {
        assert(parent[i] == expectedParent[i]);
    }

    std::cout << "Prim test passed!\n";
}

int main() {
    testDijkstra();
    testPrim();
    return 0;
}
