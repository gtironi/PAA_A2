#ifndef TRAFFIC_API_H
#define TRAFFIC_API_H

#include <cstdlib>
#include <cmath>

#include "graph.h"

using namespace std;

class EdgeNode; // Forward declaration para evitar dependências circulares

class TrafficAPI {
public:
    TrafficAPI();

    // Atualiza os multiplicadores de tráfego para todas as arestas
    void updateTraffic(GraphAdjList& graph);

private:
    // Gera um valor aleatório priorizando valores maiores entre 0 e 1
    float generateRandomMultiplier();
};

#endif // TRAFFIC_API_H
