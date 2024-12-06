#include "dijkstra.h"

void Dijkstra::compute(GraphAdjList& graph, int start, int* parent, int* distance) {
    int numVertices = graph.numVertices();
    bool checked[numVertices];
    MinHeap heap;

    for (int v = 0; v < numVertices; v++) {
        parent[v] = -1;
        distance[v] = INT_MAX;
        checked[v] = false;
    }
    parent[start] = start;
    distance[start] = 0;

    heap.insert_or_update(distance[start], start);

    while (!heap.empty()) {
        int v1 = heap.extract_min();
        heap.pop();
        if (distance[v1] == INT_MAX) break;

        EdgeNode* edge = graph.getEdges(v1);
        while (edge) {
            int v2 = edge->otherVertex();
            if (!checked[v2]) {
                int cost = edge->length();
                if (distance[v1] + cost < distance[v2]) {
                    parent[v2] = v1;
                    distance[v2] = distance[v1] + cost;
                    heap.insert_or_update(distance[v2], v2);
                }
            }
            edge = edge->next();
        }
        checked[v1] = true;
    }
}

void Dijkstra2::compute(GraphAdjList& graph, int start, int* parent, int* distance) {
    int numVertices = graph.numVertices();
    bool checked[numVertices];
    MinHeap heap;

    for (int v = 0; v < numVertices; v++) {
        parent[v] = -1;
        distance[v] = INT_MAX;
        checked[v] = false;
    }
    parent[start] = start;
    distance[start] = 0;

    heap.insert_or_update(distance[start], start);

    while (!heap.empty()) {
        int v1 = heap.extract_min();
        heap.pop();
        if (distance[v1] == INT_MAX) break;

        EdgeNode* edge = graph.getEdges(v1);
        while (edge) {
            int v2 = edge->otherVertex();
            if (!checked[v2]) {
                int cost = edge->coefficient_lotes();
                if (distance[v1] + cost < distance[v2]) {
                    parent[v2] = v1;
                    distance[v2] = distance[v1] + cost;
                    heap.insert_or_update(distance[v2], v2);
                }
            }
            edge = edge->next();
        }
        checked[v1] = true;
    }
}

std::pair<std::vector<int>, std::vector<int>> Dijkstra3::compute(const GraphAdjList& graph, int start) {
    int numVertices = graph.numVertices();
    std::vector<bool> checked(numVertices, false);       // Vetor para marcar vértices processados
    std::vector<int> parent(numVertices, -1);            // Vetor para predecessores
    std::vector<int> distance(numVertices, INT_MAX);     // Vetor para distâncias mínimas
    MinHeap heap;

    // Inicializa o vértice de origem
    parent[start] = start;
    distance[start] = 0;
    heap.insert_or_update(distance[start], start);

    // Processa os vértices enquanto a heap não estiver vazia
    while (!heap.empty()) {
        int v1 = heap.extract_min(); // Obtém o vértice com menor distância
        heap.pop();

        if (distance[v1] == INT_MAX) break; // Se distância for infinita, não há mais caminhos

        // Percorre as arestas conectadas a v1
        EdgeNode* edge = graph.getEdges(v1);
        while (edge) {
            int v2 = edge->otherVertex();    // Obtém o outro vértice
            int cost = edge->coefficient_lotes(); // Obtém o custo da aresta

            // Relaxa a aresta, se necessário
            if (!checked[v2] && distance[v1] + cost < distance[v2]) {
                parent[v2] = v1;
                distance[v2] = distance[v1] + cost;
                heap.insert_or_update(distance[v2], v2); // Atualiza a distância na heap
            }

            edge = edge->next(); // Avança para a próxima aresta
        }

        // Marca v1 como processado
        checked[v1] = true;
    }

    // Retorna os vetores de predecessores e distâncias como um par
    return {parent, distance};
}
