#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h" 
#include "heap.h"  

class Dijkstra {
public:
    static void compute(GraphAdjList& graph, int start, int* parent, int* distance);
};

#endif // DIJKSTRA_H
