#ifndef TAXI_H
#define TAXI_H

#include <iostream>
#include "graph.h"
#include "dijkstra.h"
#include "list.h"

using namespace std;

float calculaTempoTaxi(GraphAdjList& graph, int origem, int destino, int filho, int distRestante, int* parentTaxi);
float calculaTempoCaminhada(GraphAdjList& graph, int origem, int destino, int* parentCaminhada);
float rotaTaxi(GraphAdjList& graphCompleto, GraphAdjList& graphDirecionado, NodeList& rota, int origem, int destino, int custoMax);
void melhorRota(GraphAdjList& graph, int origem, int destino, int custoMax);

#endif // TAXI_H