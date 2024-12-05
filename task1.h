#ifndef TASK1_H
#define TASK1_H

#include <vector>
#include "graph.h"  
#include "dijkstra.h" 
#include "prim.h" 

using namespace std;

vector<vector<vertex>> generateRegions(const GraphAdjList& graph);

vector<vertex> determineStations(GraphAdjList& graph, const vector<vector<vertex>>& regions);

<<<<<<< HEAD
=======
// Função para construir o subgrafo com base nos caminhos mínimos entre as estações
GraphAdjList buildStationSubgraph(GraphAdjList& graph, const vector<vertex>& stations);

>>>>>>> juntando-branches
int minimumCostToConnectStations(GraphAdjList& graph, const vector<vertex>& stations);

#endif // TASK1_H
