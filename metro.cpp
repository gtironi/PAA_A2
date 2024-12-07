#include <algorithm>
#include <cmath>
#include <map>
#include <vector>
#include <string>

#include "metro.h"
#include "dijkstra.h"
#include "utils.h"
#include "graph.h"


// Função que recebe as estações e cria a linha de metrô.
GraphAdjList* createTreeMetro(GraphAdjList& graphStreets, const vector<vertex>& stations) {
    // Encontrando caminhos entre as estações com dijkstra;
    // Inicializando vetores de pais e distâncias
    int parent[graphStreets.numVertices()];
    int distance[graphStreets.numVertices()];
    // Vetor de origem
    int start = stations[0];

    // Alocando memória para lotesTypes
    int* lotesTypes = new int[4];

    // Rodando dijkstra para encontrar os caminhos que podem ser feitos
    // pelo metro.
    Dijkstra::compute(graphStreets, start, parent, distance);

    // Inicializando grafo de metrô
    GraphAdjList* graphMetro = new GraphAdjList(graphStreets.numVertices());

    // Iterando sobre todas as estações e as conectando
    for (int i = 0; i < stations.size() - 1; i++){
        // Vértice de início (estação que queremos conectar)
        vertex start = stations[i];
        
        // Conectando uma estação com todas as seguintes
        for (int j = i + 1; j < stations.size(); j++){
            // Vértice de destino (outra estação)
            vertex end = stations[j];
            // Pegando entre a dupla de estações
            IntList* path = pathVertices(start, end, parent);

            // Criando arestas pra cada par de vértices no caminho
            IntNode* v = path->head;
            while (v != nullptr) {
                // Se já estivermos no último vértice do camiho termina o loop
                if (v->next == nullptr){
                    break;
                }
                
                // Pegando valores do vértice atual e do próximo
                vertex v1 = v->data;
                vertex v2 = v->next->data;

                // Pegando arestas do vértice atual no grafo original
                EdgeNode* edge = graphStreets.getEdges(v1);
                while (edge){
                    // E quando achamos a aresta para os dois vértices que 
                    // queremos, criamos essa aresta no graphMetro
                    if(edge->otherVertex() == v2){
                        edge->getLotesType(lotesTypes);
                        graphMetro->addEdge(v1, v2, edge->bairro(), edge->length(), edge->maxSpeed(), edge->isOneway(), edge->numLotes(), lotesTypes);
                        break;
                    }

                    edge = edge->next();
                }
                
                v = v->next;
            }
        }
    }
    


    // Bidirecionalizando grafo
    GraphAdjList* graphMetroBidirectional = graphMetro->createBidirectionalCopy();
    
    // Liberando memória alocada.
    delete[] lotesTypes;
    
    return graphMetroBidirectional;
}

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

// Função que recebe um vértice v e vetor de estações stations e retorna
// se o vértice é uma estação ou não.
bool isStation(vertex v, const vector<vertex>& stations) {
    for (int i = 0; i < stations.size(); i++){
        if (stations[i] == v) { return true; }
    }
    
    return false;
}

// Função que recebe um vértice v, grafos de ruas e de metrô e retorna a
// estação de metrô mais próxima de v.
vertex closestMetroStation(vertex v, GraphAdjList& graphStreets, const vector<vertex>& stations) {
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
        if (isStation(v_i, stations) && distance[v_i] < closestDistance) {
            // Atualizamos as variáveis
            closestVertex = v_i;
            closestDistance = distance[v_i];
        }
    }

    return closestVertex;
}