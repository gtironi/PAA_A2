#ifndef METRO_H
#define METRO_H

#include <iostream>

#include "graph.h"
#include "dijkstra.h"
#include "list.h"
#include "utills.h"

using namespace std;


IntList* createPathMetro(GraphAdjList& graphMetro, int start, int end);

float timeMetro(GraphAdjList& graphMetro, IntList& path, const int SPEED_METRO);

bool vertexInGraphMetro(vertex v, GraphAdjList& graphMetro);

vertex closestMetroStation(vertex start, GraphAdjList& graphStreets, GraphAdjList& graphMetro);


#endif // METRO_H