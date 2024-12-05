#ifndef METRO_H
#define METRO_H

#include <iostream>
#include <string>

#include "graph.h"
#include "dijkstra.h"
#include "list.h"

using namespace std;

// ======================================================================
// Funções de caminho
IntList* pathVertices(int start, int end, int* parent);

// ======================================================================
// Funções auxiliares
void printVector(const vector<int>& vector);

// ======================================================================
// Funções de metro
IntList* createPathMetro(GraphAdjList& graphMetro, int start, int end);

float timeMetro(GraphAdjList& graphMetro, IntList& path, const int SPEED_METRO);

bool vertexInGraphMetro(vertex v, GraphAdjList& graphMetro);

vertex closestMetroStation(vertex start, GraphAdjList& graphStreets, GraphAdjList& graphMetro);

#endif // METRO_H