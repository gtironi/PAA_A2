#ifndef GRAPH_PROCESSING_H
#define GRAPH_PROCESSING_H

#include <vector>
#include <map>
#include "graph.h"
#include "graph_utils.h"
#include "dijkstra.h"

// Definição de tipos para simplificar
using vertex = int;

// Declaração de funções
void findMinCoefficientVertices(GraphAdjList& grafo, std::vector<int>& minLotesVertices, std::vector<int>& vertexIndices);
int calculateCycleCost(const std::vector<vertex>& permutation, const GraphAdjList& grafo);
int solveTSP(const GraphAdjList& grafo, std::vector<vertex>& bestPath);
std::vector<vertex> reconstructPath(const std::vector<int>& parent, int start, int end);
std::vector<vertex> findBusPath(GraphAdjList& grafo, const std::vector<int>& vertexIndices);
vector<vertex> loadAndProcessGraph(const std::string& filename);

#endif // GRAPH_PROCESSING_H
