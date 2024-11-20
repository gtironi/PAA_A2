#include <iostream>
#include "graph.h"
#include "dijkstra.h"
#include "prim.h"

int main() {
    // Exemplo de grafo com 6 vértices
    GraphAdjList graph(6);

    // Adiciona arestas (exemplo)
    graph.addEdge(0, 1, "Centro", 4);
    graph.addEdge(0, 2, "Centro", 2);
    graph.addEdge(1, 3, "Centro", 5);
    graph.addEdge(2, 3, "Centro", 8);
    graph.addEdge(3, 4, "Centro", 6);
    graph.addEdge(4, 5, "Centro", 3);
    
    graph.print();

    // Executando Dijkstra
    int parentDijkstra[6];
    int distanceDijkstra[6];
    Dijkstra::compute(graph, 0, parentDijkstra, distanceDijkstra);

    std::cout << "Dijkstra:\n";
    for (int i = 0; i < 6; i++) {
        std::cout << "Vertex: " << i << ", Parent: " << parentDijkstra[i]
                  << ", Distance: " << distanceDijkstra[i] << "\n";
    }
    
    

    // Executando Prim
    int parentPrim[6];
    Prim::mst(graph, parentPrim);

    std::cout << "\nPrim MST:\n";
    for (int i = 0; i < 6; i++) {
        std::cout << "Vertex: " << i << ", Parent: " << parentPrim[i] << "\n";
    }

    return 0;
}
