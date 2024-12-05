#include "metro.h"


// ======================================================================
// Funções de metro

// Recebe grafo do metrô, vértices de origem e destino e retorna caminho no 
// formato de lista
IntList* createPathMetro(GraphAdjList& graphMetro, int start, int end){
    // Inicializando vetores de pais e distâncias
    int parent[graphMetro.numVertices()];
    int distance[graphMetro.numVertices()];
    // Rodando dijkstra para encontrar os caminhos que podem ser feitos
    // pelo metro.
    Dijkstra::compute(graphMetro, start, parent, distance);

    // Retornando caminho a partir do parent retornado pelo dijkstra
    return pathVertices(start, end, parent);
}

// Recebe o grafo do metro e o caminho que vai ser percorrido e retorna 
// o tempo total gasto nesse caminho
float timeMetro(GraphAdjList& graphMetro, IntList& path, const int SPEED_METRO){
    
    float tempoTotal = 0;

    // Percorrendo caminho
    IntNode* currentVertex = path.head;

    while (currentVertex != nullptr){
        vertex v_1 = currentVertex->data;

        // Pegando arestas do vértice v_1
        EdgeNode * edge = graphMetro.getEdges(v_1);

        while (edge){
            vertex v_2 = edge->otherVertex();

            // Se não tivermos encontrado a aresta que queremos do caminho
            if (currentVertex->next && v_2 == currentVertex->next->data){
                // Tamanho da aresta
                int custo = edge->cost();
                // Calculando tempo dividino pela velocidade do metro
                tempoTotal += custo;
            }

            edge = edge->next();
        }

        currentVertex = currentVertex->next;
    }
    
    return tempoTotal/float(SPEED_METRO);
}

// Função pra adicionar rota do metro numa rota já dada

bool vertexInGraphMetro(vertex v, GraphAdjList& graphMetro){
    // Pra saber se o vértice pertence ao grafo de metrô ele precisa ter
    // pelo menos uma aresta 

    if (graphMetro.getEdges(v)) { return true; }
    else { return false; }
}

// Função que recebe um vértice v, grafos de ruas e de metrô e retorna a
// estação de metrô mais próxima de v.
vertex closestMetroStation(vertex v, GraphAdjList& graphStreets, GraphAdjList& graphMetro) {
    // Rodando dijkstra pra encontrar caminhos no grafo.
    int parent[graphStreets.numVertices()];
    int distance[graphStreets.numVertices()];
    Dijkstra::compute(graphStreets, v, parent, distance);

    int closestVertex;
    int closestDistance = INT_MAX;

    // Percorrendo vetor de distâncias para encontrar estação mais próxima.
    for (vertex v_i = 0; v_i < graphStreets.numVertices(); v_i++){
        // Se o vértice pertencer ao grafo de metrô e tiver distância 
        // menor que a menor até agora
        if (vertexInGraphMetro(v_i, graphMetro) && distance[v_i] < closestDistance) {
            // Atualizamos as variáveis
            closestVertex = v_i;
            closestDistance = distance[v_i];
        }
    }

    return closestVertex;
}