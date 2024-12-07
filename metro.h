#ifndef METRO_H
#define METRO_H

#include <iostream>
#include <vector>

#include "graph.h"
#include "dijkstra.h"
#include "list.h"
#include "utils.h"

using namespace std;

GraphAdjList* createTreeMetro(GraphAdjList& graphStreets, const vector<vertex>& stations);

vector<vector<vertex>> getRegionsAsMatrix(const GraphAdjList& graph);

vector<vertex> findRegionCenters(GraphAdjList& graph, const vector<vector<vertex>>& regions);

IntList* createPathMetro(GraphAdjList& graphMetro, int start, int end);

float timeMetro(GraphAdjList& graphMetro, IntList& path, const int SPEED_METRO);

bool isStation(vertex v, const vector<vertex>& stations);

vertex closestMetroStation(vertex v, GraphAdjList& graphStreets, const vector<vertex>& stations);


#endif // METRO_H