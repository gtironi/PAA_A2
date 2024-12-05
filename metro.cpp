#include <algorithm>
#include <cmath>
#include <map>
#include <vector>
#include <string>

#include "metro.h"
#include "dijkstra.h"
#include "utils.h"
#include "graph.h"


// ======================================================================
// Funções de metro

vector<vector<vertex>> getRegionsAsMatrix(const GraphAdjList& graph) {
    map<string, vector<vertex>> regions;

    // Percorrer todos os vértices
    for (vertex v = 0; v < graph.numVertices(); ++v) {
        EdgeNode* edge = graph.getEdges(v);
        
        // Verificar todas as arestas de um vértice
        while (edge != nullptr) {
            string bairro = edge->bairro(); // Nome da região
            
            // Se ainda não tiver registrado essa região, cria um vetor para armazenar os vértices
            if (regions.find(bairro) == regions.end()) {
                regions[bairro] = vector<vertex>();
            }
            
            // Adicionar o vértice à lista da região correspondente
            if (find(regions[bairro].begin(), regions[bairro].end(), v) == regions[bairro].end()) {
                regions[bairro].push_back(v);
            }

            // Avançar para a próxima aresta
            edge = edge->next();
        }
    }

    // Converter o mapa para uma matriz (vector<vector<vertex>>)
    vector<vector<vertex>> regionsMatrix;
    
    // Adicionar os vértices de cada região à matriz
    for (const auto& region : regions) {
        regionsMatrix.push_back(region.second);
    }

    // Agora remove os vértices das regiões subsequentes nas regiões anteriores
    for (size_t i = 0; i < regionsMatrix.size(); ++i) {
        for (size_t j = i + 1; j < regionsMatrix.size(); ++j) {
            // Remover vértices da região i que aparecem na região j
            vector<vertex>& regionI = regionsMatrix[i];
            vector<vertex>& regionJ = regionsMatrix[j];

            for (vertex v : regionJ) {
                regionI.erase(remove(regionI.begin(), regionI.end(), v), regionI.end());
            }
        }
    }

    return regionsMatrix;
}

vertex findMedianCenter(const std::vector<vertex>& regionVertices) {
    // Copia os vértices para garantir que não alteramos a lista original
    std::vector<vertex> sortedVertices = regionVertices;

    // Ordena os vértices
    std::sort(sortedVertices.begin(), sortedVertices.end());

    // Retorna o vértice que está na posição mediana
    int medianIndex = std::floor(sortedVertices.size() / 2);
    // Pegando o lado menor se tivermos número par de vértices
    sortedVertices.size() % 2 == 0 ? medianIndex -= 1 : medianIndex = medianIndex; 
    return sortedVertices[medianIndex];
}

vector<vertex> findRegionCenters(GraphAdjList& graph, const vector<vector<vertex>>& regions) {
    vector<vertex> regionCenters;

    // Colocando o vértice na posição mediana de cada região
    for (int i = 0; i < regions.size(); i++){
        vertex medianVertex = findMedianCenter(regions[i]);
        regionCenters.push_back(medianVertex);
    }
    
    return regionCenters;
}

// vector<vertex> findRegionCenters(GraphAdjList& graph, const vector<vector<vertex>>& regions) {
//     vector<vertex> regionCenters;
//     int numVertices = graph.numVertices();
//     int parent[numVertices];
//     int distance[numVertices];

//     for (const auto& region : regions) {
//         int minDistanceSum = INT_MAX;
//         vertex center = -1;
        
//         // Para cada vértice da região
//         for (vertex v : region) {
//             // Calcular a soma das distâncias de 'v' para todos os outros vértices na região
//             // vector<int> distances = dijkstra(graph, v, region);
//             Dijkstra::compute(graph, v, parent, distance);
//             int distanceSum = 0;
            
//             for (vertex u : region) {
//                 distanceSum += distance[u];
//             }
            
//             if (distanceSum < minDistanceSum) {
//                 minDistanceSum = distanceSum;
//                 center = v;
//             }
//         }
        
//         regionCenters.push_back(center);
//     }
    
//     return regionCenters;
// }

// Função que recebe as estações e cria a linha de metrô.
GraphAdjList* createTreeMetro(GraphAdjList& graphStreets, const vector<vertex>& stations, int metroSpeed) {
    // Encontrando caminhos entre as estações com dijkstra;
    // Inicializando vetores de pais e distâncias
    int parent[graphStreets.numVertices()];
    int distance[graphStreets.numVertices()];
    // Vetor de origem
    int start = stations[0];

    // Rodando dijkstra para encontrar os caminhos que podem ser feitos
    // pelo metro.
    Dijkstra::compute(graphStreets, start, parent, distance);

    // Inicializando grafo de metrô
    GraphAdjList* graphMetro = new GraphAdjList(graphStreets.numVertices());

    // Pegando caminho
    IntList* caminho = pathVertices(start, stations[1], parent);

    // Criando arestas pra cada par de vértices no caminho
    IntNode* v = caminho->head;
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
                int* lotesTypes = new int[4];
                edge->getLotesType(lotesTypes);
                graphMetro->addEdge(v1, v2, edge->bairro(), edge->length(), edge->maxSpeed(), edge->isOneway(), edge->numLotes(), lotesTypes);
                break;
            }

            edge = edge->next();
        }
        
        v = v->next;
    }

    GraphAdjList* graphMetroBidirectional = graphMetro->createBidirectionalCopy();
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