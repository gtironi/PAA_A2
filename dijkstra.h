#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"
#include "heap.h"
#include <climits> // Para INT_MAX

class Dijkstra {
public:
    static void compute(GraphAdjList& graph, int start, int* parent, int* distance);
};

class Dijkstra2 {
public:
    static void compute(GraphAdjList& graph, int start, int* parent, int* distance);
};

class Dijkstra3 {
public:
    static std::pair<std::vector<int>, std::vector<int>> compute(const GraphAdjList& graph, int start);
};

#endif // DIJKSTRA_H
