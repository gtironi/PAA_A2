#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>

#include "graph.h"
#include "dijkstra.h"
#include "prim.h"

using namespace std;

// Função para gerar a lista de regiões
// vector<vector<vertex>> generateRegions(const GraphAdjList& graph) {
//     unordered_map<string, vector<vertex>> regionMap;

//     for (vertex v = 0; v < graph.numVertices(); v++) {
//         EdgeNode* edge = graph.getEdges(v);

//         while (edge) {
//             string bairro = edge->bairro();

//             // Adicionar o vértice à região correspondente
//             if (regionMap.find(bairro) == regionMap.end()) {
//                 regionMap[bairro] = vector<vertex>();
//             }
//             regionMap[bairro].push_back(v);

//             edge = edge->next();
//         }
//     }

//     vector<vector<vertex>> regions;
//     for (const auto& pair : regionMap) {
//         regions.push_back(pair.second);
//     }

//     return regions;
// }

vector<vector<vertex>> generateRegions(const GraphAdjList& graph) {
    unordered_map<string, vector<vertex>> regionMap;

    // Adiciona os vértices manualmente às suas regiões
    for (vertex v = 0; v < graph.numVertices(); v++) {
        // Verifica em qual região o vértice se encaixa (0-7 para região 1, 8-12 para região 2)
        string bairro;
        if (v >= 0 && v <= 7) {
            bairro = "Região 1";  // Região 1: vértices 0 a 7
        } else if (v >= 8 && v <= 12) {
            bairro = "Região 2";  // Região 2: vértices 8 a 12
        }

        // Adiciona o vértice à região correspondente
        if (regionMap.find(bairro) == regionMap.end()) {
            regionMap[bairro] = vector<vertex>();
        }
        regionMap[bairro].push_back(v);
    }

    // Converte o mapa de regiões para o formato de vetor
    vector<vector<vertex>> regions;
    for (const auto& pair : regionMap) {
        regions.push_back(pair.second);
    }

    return regions;
}

// Função para encontrar a estação de metrô ideal para cada região
vector<vertex> determineStations(GraphAdjList& graph, const vector<vector<vertex>>& regions) {
    vector<vertex> stations;
    int numVertices = graph.numVertices();
    int parent[numVertices];
    int distance[numVertices];

    for (const auto& region : regions) {
        int minMaxDist = INT_MAX;
        vertex bestStation = -1;

        for (vertex v : region) {
            Dijkstra::compute(graph, v, parent, distance);

            // Determinar a distância máxima dentro da região
            int maxDist = 0;
            for (vertex u : region) {
                if (distance[u] < INT_MAX) {
                    maxDist = max(maxDist, distance[u]);
                }
            }

            if (maxDist < minMaxDist) {
                minMaxDist = maxDist;
                bestStation = v;
            }
        }

        stations.push_back(bestStation);
    }

    // for (const auto& region : regions) {
    //     int bestStation = -1;
    //     int minTotalDist = INT_MAX; // Usar soma das distâncias em vez da distância máxima

    //     // Para cada vértice na região
    //     for (vertex v : region) {
    //         Dijkstra::compute(graph, v, parent, distance);

    //         // Calcular a soma das distâncias para todos os vértices dentro da região
    //         int totalDist = 0;
    //         for (vertex u : region) {
    //             if (distance[u] < INT_MAX) {  // Ignorar vértices desconectados
    //                 totalDist += distance[u];
    //             }
    //         }

    //         // Escolher o vértice com a menor soma de distâncias
    //         if (totalDist < minTotalDist) {
    //             minTotalDist = totalDist;
    //             bestStation = v;
    //         }
    //     }

    //     stations.push_back(bestStation);
    // }

    return stations;
}

// Função para construir o subgrafo com base nos caminhos mínimos entre as estações
GraphAdjList buildStationSubgraph(GraphAdjList& graph, const vector<vertex>& stations) {
    int numStations = stations.size();
    GraphAdjList subgraph(numStations);

    // Mapear cada estação para o índice no subgrafo
    unordered_map<vertex, int> stationIndex;
    for (int i = 0; i < numStations; i++) {
        stationIndex[stations[i]] = i;
    }

    // Calcular os menores caminhos entre cada par de estações
    int parent[graph.numVertices()];
    int distance[graph.numVertices()];

    int lotesType[4] = {};

    for (int i = 0; i < numStations; i++) { 
        cout << "Iteração: " << i << endl;
        Dijkstra::compute(graph, stations[i], parent, distance);

        for (int j = i + 1; j < numStations; j++) {
            cout << "Station: " << j << " is " << stations[j] << endl;
            if (stations[j] >= graph.numVertices()) {
                continue;
            }

            int cost = distance[stations[j]];
            cout << "The cost is: " << cost << endl;
            if (cost < INT_MAX) {
                subgraph.addEdge(i, j, "Subgraph", cost, 0, false, 0, lotesType);
            }
        }
    }

    return subgraph;
}

// Função para calcular o custo mínimo para conectar as estações
int minimumCostToConnectStations(GraphAdjList& graph, const vector<vertex>& stations) {
    GraphAdjList subgraph = buildStationSubgraph(graph, stations);

    // Usar o algoritmo de Prim para calcular a MST no subgrafo
    int numStations = stations.size();
    int parent[numStations];
    Prim::mst(subgraph, parent);

    // PRINT MST
    //cout << "Minimum Spanning Tree:" << endl;
    //subgraph.print();

    // Calcular o custo total da MST
    int totalCost = 0;
    for (int v = 1; v < numStations; v++) {
        EdgeNode* edge = subgraph.getEdges(v);
        while (edge) {
            if (edge->otherVertex() == parent[v]) {
                totalCost += edge->cost();
                break;
            }
            edge = edge->next();
        }
    }

    return totalCost;
}