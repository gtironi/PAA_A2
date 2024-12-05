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
vector<vector<vertex>> generateRegions(const GraphAdjList& graph) {
    unordered_map<string, vector<vertex>> regionMap;

    for (vertex v = 0; v < graph.numVertices(); v++) {
        EdgeNode* edge = graph.getEdges(v);

        while (edge) {
            string bairro = edge->bairro();

            // Adicionar o vértice à região correspondente
            if (regionMap.find(bairro) == regionMap.end()) {
                regionMap[bairro] = vector<vertex>();
            }
            regionMap[bairro].push_back(v);

            edge = edge->next();
        }
    }

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

    return stations;
}

//Função para calcular o custo mínimo para conectar todas as estações
int minimumCostToConnectStations(GraphAdjList& graph, const vector<vertex>& stations) {
    int n = stations.size();
    vector<bool> inMST(n, false);
    priority_queue<pair<int, vertex>, vector<pair<int, vertex>>, greater<>> pq;

    int totalCost = 0;
    pq.push({0, stations[0]});

    while (!pq.empty()) {
        int cost = pq.top().first;
        vertex u = pq.top().second;
        pq.pop();

        if (inMST[u]) continue;

        inMST[u] = true;
        totalCost += cost;

        EdgeNode* edge = graph.getEdges(u);
        while (edge) {
            vertex v = edge->otherVertex();
            if (!inMST[v]) {
                pq.push({edge->cost(), v});
            }
            edge = edge->next();
        }
    }

    return totalCost;
}