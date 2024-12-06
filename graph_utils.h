#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include <string>
#include "graph.h" // Certifique-se de incluir o cabeçalho adequado

// Declarações das funções
int getMaxVertex(const std::string& filename);
void loadGraphFromCSV(const std::string& filename, GraphAdjList& graph);
GraphAdjList LoadGraph(const std::string& filename);

#endif // GRAPH_UTILS_H
