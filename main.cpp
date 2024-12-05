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
#include <ctime>

#include "graph.h"
#include "dijkstra.h"
#include "prim.h"
#include "list.h"
#include "bus.h"
#include "taxi.h"
#include "trafficAPI.h"
#include "utills.h"

using namespace std;


int main() {
    // Grafo de táxi pra teste
    GraphAdjList graph(10);

    int stdSizeEdge = 2;
    int stdMaxSpeed = 70;
    bool oneWay = true;
    int lotesType[4] = {1, 0, 0, 0};
    int numLotes = 100;

    // Criação do grafo de teste com 10 vértices
    graph.addEdge(0, 1, "BairroA", 10, 50, false, 1, lotesType);
    graph.addEdge(1, 2, "BairroA", 20, 40, false, 1, lotesType);
    graph.addEdge(2, 3, "BairroA", 15, 45, false, 1, lotesType);
    graph.addEdge(3, 4, "BairroA", 20, 35, false, 1, lotesType);
    graph.addEdge(1, 3, "BairroA", 5, 35, false, 1, lotesType);
    graph.addEdge(0, 4, "BairroA", 10, 35, false, 1, lotesType);
    graph.addEdge(4, 5, "BairroA", 30, 40, false, 1, lotesType);
    graph.addEdge(9, 0, "BairroB", 25, 45, false, 1, lotesType);
    graph.addEdge(5, 6, "BairroB", 10, 60, false, 1, lotesType);
    graph.addEdge(6, 7, "BairroB", 25, 55, false, 1, lotesType);
    graph.addEdge(6, 9, "BairroB", 25, 55, false, 1, lotesType);
    graph.addEdge(7, 8, "BairroB", 35, 50, false, 1, lotesType);
    graph.addEdge(8, 9, "BairroB", 40, 45, false, 1, lotesType);
    // // Adiciona arestas
    // graph.addEdge(0, 1, "Bairro1", stdSizeEdge, stdMaxSpeed, !oneWay, numLotes, lotesType);
    // graph.addEdge(0, 2, "Bairro1", stdSizeEdge, stdMaxSpeed, !oneWay, numLotes, lotesType);
    // graph.addEdge(1, 3, "Bairro1", stdSizeEdge, stdMaxSpeed, !oneWay, numLotes, lotesType);
    // graph.addEdge(3, 4, "Bairro1", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    // graph.addEdge(3, 5, "Bairro1", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    // graph.addEdge(3, 7, "Bairro1", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    // graph.addEdge(4, 6, "Bairro2", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    // graph.addEdge(5, 7, "Bairro2", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    // graph.addEdge(6, 8, "Bairro2", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    // graph.addEdge(6, 10, "Bairro2", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    // graph.addEdge(7, 8, "Bairro2", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    // graph.addEdge(7, 9, "Bairro3", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    // graph.addEdge(8, 10, "Bairro3", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    // graph.addEdge(9, 10, "Bairro3", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    // graph.addEdge(10, 12, "Bairro3", stdSizeEdge, stdMaxSpeed, !oneWay, numLotes, lotesType);
    // graph.addEdge(11, 10, "Bairro3", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);

    // Defina aqui os vértices de origem e destino e o custo máximo, respectivamente
    melhorRota(graph, 0, 8, 10); // a função irá printar o melhor caminho e o tempo gasto

    return 0;
}