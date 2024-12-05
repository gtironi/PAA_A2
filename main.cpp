#include <climits> // Para INT_MAX
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <limits>     // Para numeric_limits
#include <queue>
#include <set>

#include "graph.h"
#include "dijkstra.h"
#include "prim.h"
#include "list.h"
// #include "bus.h"
#include "taxi.h"

using namespace std;


void melhorRota(GraphAdjList& graph, int origem, int destino, int custoMax) {
    NodeList rota;
    GraphAdjList* graphCompleto = graph.createBidirectionalCopy();
    GraphAdjList* graphDirecionado = graph.clone();
    float resto = 0;
    // Selecione as arestas mínimas por bairro
    // auto arestasMinimas = selecionarArestasMinimasPorBairro(graph);
    // Encontra o ciclo para a rota de onibus
    // auto ciclo = encontrarCicloArestasMinimas(graph, arestasMinimas);

    float tempoTaxi = rotaTaxi(*graphCompleto, *graphDirecionado, rota, origem, destino, custoMax, resto);
    cout << "ROTA SOMENTE COM TAXI:" << endl;
    rota.print();
    rota.clear();
    // float tempoOnibus = rotaOnibus(*graphCompleto, *graphDirecionado, ciclo, rota, origem, destino, custoMax);
    // cout << "ROTA COM ÔNIBUS:" << endl;
    // rota.print();
    // rota.clear();
    return;
}


int main() {
    // Grafo de táxi pra teste
    GraphAdjList graph(13);

    int stdSizeEdge = 100;
    int stdMaxSpeed = 60;
    bool oneWay = true;
    int lotesType[4] = {1, 0, 0, 0};
    int numLotes = 100;

    // Adiciona arestas
    graph.addEdge(0, 1, "Bairro1", stdSizeEdge, stdMaxSpeed, !oneWay, numLotes, lotesType);
    graph.addEdge(0, 2, "Bairro1", stdSizeEdge, stdMaxSpeed, !oneWay, numLotes, lotesType);
    graph.addEdge(1, 3, "Bairro1", stdSizeEdge, stdMaxSpeed, !oneWay, numLotes, lotesType);
    graph.addEdge(3, 4, "Bairro1", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graph.addEdge(3, 5, "Bairro1", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graph.addEdge(3, 7, "Bairro1", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graph.addEdge(4, 6, "Bairro2", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graph.addEdge(5, 7, "Bairro2", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graph.addEdge(6, 8, "Bairro2", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graph.addEdge(6, 10, "Bairro2", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graph.addEdge(7, 8, "Bairro2", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graph.addEdge(7, 9, "Bairro3", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graph.addEdge(8, 10, "Bairro3", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graph.addEdge(9, 10, "Bairro3", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graph.addEdge(10, 12, "Bairro3", stdSizeEdge, stdMaxSpeed, !oneWay, numLotes, lotesType);
    graph.addEdge(11, 10, "Bairro3", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);

    // defina aqui os vértices de origem e destino e o custo máximo, respectivamente
    melhorRota(graph, 0, 3, 100000); // a função irá printar o melhor caminho e o tempo gasto

    return 0;
}