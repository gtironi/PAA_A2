#ifndef BUS_H
#define BUS_H

#include <unordered_map>
#include <vector>
#include <string>
#include "graph.h"
#include "list.h"

std::unordered_map<std::string, std::pair<vertex, vertex>> selecionarArestasMinimasPorBairro(GraphAdjList& grafo);


std::vector<vertex> reconstruirCaminho(vertex origem, vertex destino, const std::vector<vertex>& parent);

std::vector<vertex> encontrarCicloArestasMinimas(GraphAdjList& subgrafo, const std::unordered_map<std::string, std::pair<vertex, vertex>>& arestasMinimas);

void testarDesempenho();
float rotaOnibus(GraphAdjList& graphCompleto, GraphAdjList& graphDirecionado, std::vector<vertex> linhaOnibus, NodeList& rota, int origem, int destino, float custoMax);

#endif // BUS_H