#include "trafficAPI.h"

TrafficAPI::TrafficAPI() {}

void TrafficAPI::updateTraffic(GraphAdjList& graph) {

    int numVertices = graph.numVertices();

    for (int v = 0; v < numVertices; v++){
        EdgeNode* edge = graph.getEdges(v);
        while (edge) {
            if (rand() % 2 == 0) {
                // Deixa o multiplicador para metade das arestas (aproximadamente)
                edge->traffic_multiplier = 1.0f;
            } else {
                // Define um valor aleatório para a outra metade
                edge->traffic_multiplier = generateRandomMultiplier();
            }
            edge = edge->next();
        }
    }
}

float TrafficAPI::generateRandomMultiplier() {
    // Gera um número aleatório entre 0.15 e 0.95, priorizando valores maiores
    float num = 0.15f + 1.5 * (rand() % 100) / 100.0f;

    return std::min(num, 0.95f);
}
