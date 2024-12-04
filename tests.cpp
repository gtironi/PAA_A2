#include <iostream>
#include <cassert>
#include "graph.h"
#include "dijkstra.h"
#include "task1.h"

void testDijkstra() {
    // Criar grafo de teste
    GraphAdjList graph(6);

    // Adiciona arestas
    graph.addEdge(0, 1, "Centro", 4, 1, true);
    graph.addEdge(0, 2, "Centro", 2, 1, true);
    graph.addEdge(1, 3, "Centro", 5, 1, true);
    graph.addEdge(2, 3, "Centro", 8, 1, true);
    graph.addEdge(3, 4, "Centro", 6, 1, true);
    graph.addEdge(4, 5, "Centro", 3, 1, true);

    // Resultados esperados
    int expectedParent[6] = {0, 0, 0, 1, 3, 4};
    int expectedDistance[6] = {0, 4, 2, 9, 15, 18};

    // Executar Dijkstra
    int parent[6];
    int distance[6];
    Dijkstra::compute(graph, 0, parent, distance);

    // Verificar resultados
    for (int i = 0; i < 6; i++) {
        assert(parent[i] == expectedParent[i]);
        assert(distance[i] == expectedDistance[i]);
    }

    std::cout << "Teste Dijkstra passou!\n";
}

void testPrim() {
    // Criar grafo de teste
    GraphAdjList graph(6);

    // Adiciona arestas
    graph.addEdge(0, 1, "Centro", 4, 1, true);
    graph.addEdge(0, 2, "Centro", 2, 1, true);
    graph.addEdge(1, 3, "Centro", 5, 1, true);
    graph.addEdge(2, 3, "Centro", 8, 1, true);
    graph.addEdge(3, 4, "Centro", 6, 1, true);
    graph.addEdge(4, 5, "Centro", 3, 1, true);

    // Resultados esperados (Prim MST sempre forma uma árvore de custo mínimo)
    int expectedParent[6] = {0, 0, 0, 1, 3, 4};

    // Executar Prim
    int parent[6];
    Prim::mst(graph, parent);

    // Verificar resultados
    for (int i = 0; i < 6; i++) {
        assert(parent[i] == expectedParent[i]);
    }

    std::cout << "Teste Prim passou!\n";
}

int testTask1() {
    // Criar o grafo (exemplo)
    GraphAdjList graph(7);
    graph.addEdge(0, 1, "Centro", 10, 50, false);
    graph.addEdge(1, 2, "Centro", 15, 50, false);
    graph.addEdge(2, 3, "Zona Sul", 12, 50, false);
    graph.addEdge(3, 4, "Zona Sul", 8, 50, false);
    graph.addEdge(3, 5, "Zona Sul", 20, 50, false);
    graph.addEdge(4, 5, "Zona Sul", 8, 50, false);
    graph.addEdge(5, 6, "Zona Sul", 22, 50, false);

    // Gerar regiões
    vector<vector<vertex>> regions = generateRegions(graph);

    // Determinar estações de metrô
    vector<vertex> stations = determineStations(graph, regions);

    // Exibir estações
    for (size_t i = 0; i < stations.size(); i++) {
        std::cout << "Region " << i + 1 << " - Estation: " << stations[i] << endl;
    }

    // Conectando estações - construir MST
    int totalCost = minimumCostToConnectStations(graph, stations);
    std::cout << totalCost << endl;

    return 0;
}


int main() {
    testDijkstra();
    testPrim();
    testTask1();
    return 0;
}
