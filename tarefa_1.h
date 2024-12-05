#ifndef TAREFA_1_H
#define TAREFA_1_H

#include "graph.h"
#include <vector>
#include <unordered_map>

// Estrutura para representar uma região
struct Regiao {
    int cep;
    std::vector<vertex> vertices; // Vértices pertencentes à região
    vertex estacao;               // Vértice escolhido como estação de metrô
};

class Tarefa1 {
public:
    // Função principal para executar a Tarefa 1
    static void executar(GraphAdjList& graph, const std::vector<Regiao>& regioes);

private:
    // Função para encontrar o centro de cada região
    static void encontrarEstacoes(GraphAdjList& graph, std::vector<Regiao>& regioes);

    // Função para construir o grafo completo das estações
    static void construirGrafoEstacoes(GraphAdjList& graph, const std::vector<vertex>& estacoes,
                                       std::vector<std::tuple<vertex, vertex, int>>& arestasEstacoes);

    // Função para executar o algoritmo de Kruskal e construir a MST das estações
    static void kruskalMST(int numEstacoes, const std::vector<std::tuple<vertex, vertex, int>>& arestasEstacoes,
                           std::vector<std::tuple<vertex, vertex, int>>& mst);

    // Função para imprimir o resultado
    static void imprimirResultado(const std::vector<std::tuple<vertex, vertex, int>>& mst,
                                  const std::vector<Regiao>& regioes);
};

#endif // TAREFA_1_H
