#ifndef UTILS_H
#define UTILS_H

#include "graph.h"
#include "list.h"

// Retorna a hora atual em formato decimal (float)
float getHoraAtual();

// Calcula a melhor rota entre dois pontos considerando restrições e atualizações de tráfego
void melhorRota(GraphAdjList& graph, int origem, int destino, int custoMax);
float calcularFaltante(float numero);
void printRota(NodeList& rota);

#endif // UTILS_H