#include <iostream>
#include <cassert>

#include "graph.h"

int main() {
    // Grafo de táxi pra teste
    GraphAdjList graphTaxi(13);

    int stdSizeEdge = 100;
    int stdMaxSpeed = 60;
    bool oneWay = true;

    // Adiciona arestas
    graphTaxi.addEdge(0, 1, "Centro", stdSizeEdge, stdMaxSpeed, !oneWay);
    graphTaxi.addEdge(0, 2, "Centro", stdSizeEdge, stdMaxSpeed, !oneWay);
    graphTaxi.addEdge(1, 3, "Centro", stdSizeEdge, stdMaxSpeed, !oneWay);
    graphTaxi.addEdge(3, 4, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphTaxi.addEdge(3, 5, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphTaxi.addEdge(3, 7, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphTaxi.addEdge(4, 6, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphTaxi.addEdge(5, 7, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphTaxi.addEdge(6, 8, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphTaxi.addEdge(6, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphTaxi.addEdge(7, 8, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphTaxi.addEdge(7, 9, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphTaxi.addEdge(8, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphTaxi.addEdge(9, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphTaxi.addEdge(10, 12, "Centro", stdSizeEdge, stdMaxSpeed, !oneWay);
    graphTaxi.addEdge(11, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);

    // Grafo de metro pra teste
    GraphAdjList graphMetro(13);

    // Copiando aresta dos vértices 4 a 9
    graphMetro.addEdge(3, 4, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(3, 5, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(3, 7, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(4, 6, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(5, 7, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(6, 8, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(6, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(7, 8, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(7, 9, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(8, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(9, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);

    GraphAdjList* graphMetroBidirectional = graphMetro.createBidirectionalCopy();

    cout << "Printando grafo de metro: " << endl;
    graphMetroBidirectional->print();   

    return 0;
}