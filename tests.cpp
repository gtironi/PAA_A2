#include <iostream>
#include <cassert>
#include <climits>
#include "graph.h"
#include "dijkstra.h"
#include "prim.h"

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

int main() {
    testDijkstra();
    testPrim();
    testCoeficiente();
    return 0;
}
