#include <iostream>
#include <cassert>
#include <climits>
#include "graph.h"
#include "dijkstra.h"
#include "task1.h"
#include "tarefa_1.h"

void testDijkstra() {
    // Criar grafo de teste
    GraphAdjList graph(6);

    int lotesType[4] = {1, 0, 0, 0};

    // Adiciona arestas
    graph.addEdge(0, 1, "Centro", 4, 1, true, 1, lotesType);
    graph.addEdge(0, 2, "Centro", 2, 1, true, 1, lotesType);
    graph.addEdge(1, 3, "Centro", 5, 1, true, 1, lotesType);
    graph.addEdge(2, 3, "Centro", 8, 1, true, 1, lotesType);
    graph.addEdge(3, 4, "Centro", 6, 1, true, 1, lotesType);
    graph.addEdge(4, 5, "Centro", 3, 1, true, 1, lotesType);

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

    int lotesType[4] = {1, 0, 0, 0};

    // Adiciona arestas
    graph.addEdge(0, 1, "Centro", 4, 1, true, 1, lotesType);
    graph.addEdge(0, 2, "Centro", 2, 1, true, 1, lotesType);
    graph.addEdge(1, 3, "Centro", 5, 1, true, 1, lotesType);
    graph.addEdge(2, 3, "Centro", 8, 1, true, 1, lotesType);
    graph.addEdge(3, 4, "Centro", 6, 1, true, 1, lotesType);
    graph.addEdge(4, 5, "Centro", 3, 1, true, 1, lotesType);

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
    GraphAdjList graph(10);

    int lotesType[4] = {1, 0, 0, 0};

    graph.addEdge(0, 1, "Centro", 10, 50, false, 1, lotesType);
    graph.addEdge(1, 2, "Centro", 15, 50, false, 1, lotesType);
    graph.addEdge(2, 3, "Zona Sul", 12, 50, false, 1, lotesType);
    graph.addEdge(3, 4, "Zona Sul", 8, 50, false, 1, lotesType);
    graph.addEdge(3, 5, "Zona Sul", 20, 50, false, 1, lotesType);
    graph.addEdge(4, 5, "Zona Sul", 8, 50, false, 1, lotesType);
    graph.addEdge(5, 6, "Zona Sul", 22, 50, false, 1, lotesType);
    graph.addEdge(0, 6, "Barra", 25, 50, false, 1, lotesType);
    graph.addEdge(0, 7, "Barra", 30, 50, false, 1, lotesType);
    graph.addEdge(7, 6, "Barra", 35, 50, false, 1, lotesType);
    graph.addEdge(7, 5, "Barra", 40, 50, false, 1, lotesType);

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
    cout << "Custo total para conectar estações: " << totalCost << endl;

    return 0;
}

void testCoeficiente() {
    // Criar grafo de teste
    GraphAdjList graph(6);

    int lotesType1[4] = {3, 20, 14, 9};  //1
    int lotesType0[4] = {1, 10, 0, 0};  //INT_MAX
    int lotesType2[4] = {20, 0, 2, 2};  //5

    // Adiciona arestas com diferentes lotesType para testar a fórmula
    graph.addEdge(0, 1, "Centro", 4, 1, true, 1, lotesType0);
    graph.addEdge(0, 2, "Centro", 2, 1, true, 1, lotesType1);
    graph.addEdge(0, 3, "Centro", 2, 1, true, 1, lotesType2);

    // Verificar se o coeficiente foi calculado corretamente para cada aresta
    EdgeNode* edge = graph.getEdges(0);

    assert(edge->coefficient_lotes() == 5);

    edge = edge->next();
    assert(edge->coefficient_lotes() == 1);

    edge = edge->next();
    assert(edge->coefficient_lotes() == INT_MAX);

    std::cout << "Teste de coeficiente passou!\n";
}

int testTask1() {
    // Criar o grafo (exemplo)
    GraphAdjList graph(10);

    int lotesType[4] = {1, 0, 0, 0};

    graph.addEdge(0, 1, "Centro", 10, 50, false, 1, lotesType);
    graph.addEdge(1, 2, "Centro", 15, 50, false, 1, lotesType);
    graph.addEdge(2, 3, "Zona Sul", 12, 50, false, 1, lotesType);
    graph.addEdge(3, 4, "Zona Sul", 8, 50, false, 1, lotesType);
    graph.addEdge(3, 5, "Zona Sul", 20, 50, false, 1, lotesType);
    graph.addEdge(4, 5, "Zona Sul", 8, 50, false, 1, lotesType);
    graph.addEdge(5, 6, "Zona Sul", 22, 50, false, 1, lotesType);
    graph.addEdge(0, 6, "Barra", 25, 50, false, 1, lotesType);
    graph.addEdge(0, 7, "Barra", 30, 50, false, 1, lotesType);
    graph.addEdge(7, 6, "Barra", 35, 50, false, 1, lotesType);
    graph.addEdge(7, 5, "Barra", 40, 50, false, 1, lotesType);

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
    cout << "Custo total para conectar estações: " << totalCost << endl;

    return 0;
}


int main() {
    testDijkstra();
    testPrim();
    testTask1();
    //testCoeficiente();
    return 0;
}