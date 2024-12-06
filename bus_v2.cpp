#include "bus_v2.h"
#include "graph_utils.h"
#include <iostream>
#include <algorithm>
#include <cassert>

using namespace std;

// Função para achar o vértice com menor coeficiente de lotes para cada bairro
void findMinCoefficientVertices(GraphAdjList& grafo, vector<int>& minLotesVertices, vector<int>& vertexIndices) {
    map<string, int> bairroIndex;
    int bairroCounter = 0;
    int numVertices = grafo.numVertices();

    for (int i = 0; i < numVertices; ++i) {
        EdgeNode* edge = grafo.getEdges(i);
        while (edge) {
            const string& bairro = edge->bairro();
            if (bairroIndex.find(bairro) == bairroIndex.end()) {
                bairroIndex[bairro] = bairroCounter++;
                minLotesVertices.push_back(0);
                vertexIndices.push_back(-1);
            }
            int bairroIdx = bairroIndex[bairro];
            if (minLotesVertices[bairroIdx] > edge->coefficient_lotes() || minLotesVertices[bairroIdx] == 0) {
                minLotesVertices[bairroIdx] = edge->coefficient_lotes();
                vertexIndices[bairroIdx] = i;
            }
            edge = edge->next();
        }
    }
}

// Função para calcular o custo total de um ciclo
int calculateCycleCost(const vector<vertex>& permutation, const GraphAdjList& grafo) {
    int totalCost = 0;
    int numVertices = grafo.numVertices();

    for (size_t i = 0; i < permutation.size(); ++i) {
        int u = permutation[i];
        int v = permutation[(i + 1) % numVertices];
        EdgeNode* edge = grafo.getEdges(u);
        while (edge) {
            if (edge->otherVertex() == v) {
                totalCost += edge->coefficient_lotes();
                break;
            }
            edge = edge->next();
        }
    }
    return totalCost;
}

// Função para resolver o TSP
int solveTSP(const GraphAdjList& grafo, vector<vertex>& bestPath) {
    int numVertices = grafo.numVertices();
    vector<vertex> vertices(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        vertices[i] = i;
    }

    int minCost = INT_MAX;
    vector<vertex> bestPermutation;

    do {
        int currentCost = calculateCycleCost(vertices, grafo);
        if (currentCost < minCost) {
            minCost = currentCost;
            bestPermutation = vertices;
        }
    } while (next_permutation(vertices.begin(), vertices.end()));

    bestPath = bestPermutation;
    bestPath.push_back(bestPath[0]);

    return minCost;
}

// Reconstrói o caminho entre dois vértices
vector<vertex> reconstructPath(const vector<int>& parent, int start, int end) {
    vector<vertex> path;
    for (int v = end; v != start; v = parent[v]) {
        path.push_back(v);
    }
    path.push_back(start);
    reverse(path.begin(), path.end());
    return path;
}

// Função para encontrar o caminho de ônibus com mais atrações e comércios
vector<vertex> findBusPath(GraphAdjList& grafo, const vector<int>& vertexIndices) {
    int numBairros = vertexIndices.size();
    GraphAdjList subgraph(numBairros);

    for (int i = 0; i < numBairros; ++i) {
        int v1 = vertexIndices[i];
        auto [parent, distance] = Dijkstra3::compute(grafo, v1);

        for (int j = 0; j < numBairros; ++j) {
            if (i == j) continue;

            int v2 = vertexIndices[j];
            if (distance[v2] != INT_MAX) {
                int lotesType[4] = {distance[v2], 0, 1, 0};
                subgraph.addEdge(i, j, "SubgraphEdge", 0, 0, true, 1, lotesType);
            }
        }
    }

    vector<vertex> bestPath;
    solveTSP(subgraph, bestPath);

    vector<vertex> allPaths;
    for (size_t i = 0; i < bestPath.size() - 1; ++i) {
        vector<vertex> path = reconstructPath(Dijkstra3::compute(grafo, vertexIndices[bestPath[i]]).first,
                                              vertexIndices[bestPath[i]], vertexIndices[bestPath[i + 1]]);
        if (!allPaths.empty() && allPaths.back() == path.front()) {
            path.erase(path.begin());
        }
        allPaths.insert(allPaths.end(), path.begin(), path.end());
    }

    return allPaths;
}

// Função para carregar e processar o grafo
vector<vertex> loadAndProcessGraph(const string& filename) {
    int maxVertex = getMaxVertex(filename);
    if (maxVertex == -1) return {};

    GraphAdjList grafo(maxVertex + 1);
    loadGraphFromCSV(filename, grafo);

    vector<int> minLotesVertices, vertexIndices;
    findMinCoefficientVertices(grafo, minLotesVertices, vertexIndices);

    vector<vertex> BestPath = findBusPath(grafo, vertexIndices);

    return BestPath;
}
