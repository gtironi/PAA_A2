#ifndef TAXI_H
#define TAXI_H

#include <iostream>
#include "graph.h"
#include "dijkstra.h"
#include "list.h"
// #include "metro.h"

using namespace std;

const float TAXA_TAXI = 4.0f;
const float CUSTO_MIN_TAXI = 9.0f;
const int A_PE_CODE = 0;
const int TAXI_CODE = 1;

float calculaTempoTaxi(GraphAdjList& graph, int origem, int destino, int* parentTaxi);
float calculaTempoCaminhada(GraphAdjList& graph, int origem, int destino, int* parentCaminhada);
float rotaTaxi(GraphAdjList& graphCompleto, GraphAdjList& graphDirecionado, NodeList& rota, int origem, int destino, int custoMax, float&);
// void melhorRota(GraphAdjList& graph, int origem, int destino, int custoMax);

#endif // TAXI_H