#include "tarefa_1.h"
#include "dijkstra.h"
#include <limits>
#include <iostream>
#include <algorithm>

// Estruturas auxiliares para o algoritmo de Kruskal
struct Subconjunto {
    int pai;
    int rank;
};

// Funções auxiliares para o algoritmo de Kruskal
int encontrar(Subconjunto subconjuntos[], int i) {
    if (subconjuntos[i].pai != i)
        subconjuntos[i].pai = encontrar(subconjuntos, subconjuntos[i].pai);
    return subconjuntos[i].pai;
}

void unir(Subconjunto subconjuntos[], int x, int y) {
    int raizX = encontrar(subconjuntos, x);
    int raizY = encontrar(subconjuntos, y);

    if (subconjuntos[raizX].rank < subconjuntos[raizY].rank) {
        subconjuntos[raizX].pai = raizY;
    } else if (subconjuntos[raizX].rank > subconjuntos[raizY].rank) {
        subconjuntos[raizY].pai = raizX;
    } else {
        subconjuntos[raizY].pai = raizX;
        subconjuntos[raizX].rank++;
    }
}

// Função principal para executar a Tarefa 1
void Tarefa1::executar(GraphAdjList& graph, const std::vector<Regiao>& regioesInput) {
    // Copiar as regiões para uma versão modificável
    std::vector<Regiao> regioes = regioesInput;

    // Etapa 1: Encontrar as estações de metrô (centros das regiões)
    encontrarEstacoes(graph, regioes);

    // Obter a lista de estações
    std::vector<vertex> estacoes;
    for (const auto& regiao : regioes) {
        estacoes.push_back(regiao.estacao);
    }

    // Etapa 2: Construir o grafo completo das estações
    std::vector<std::tuple<vertex, vertex, int>> arestasEstacoes;
    construirGrafoEstacoes(graph, estacoes, arestasEstacoes);

    // Etapa 3: Executar Kruskal para encontrar a MST das estações
    std::vector<std::tuple<vertex, vertex, int>> mst;
    kruskalMST(estacoes.size(), arestasEstacoes, mst);

    // Imprimir o resultado
    imprimirResultado(mst, regioes);
}

// Função para encontrar o centro (estação) de cada região
void Tarefa1::encontrarEstacoes(GraphAdjList& graph, std::vector<Regiao>& regioes) {
    for (auto& regiao : regioes) {
        vertex melhorVertice = -1;
        int menorDistanciaMaxima = std::numeric_limits<int>::max();

        for (vertex v : regiao.vertices) {
            // Executar Dijkstra a partir do vértice v para todos os outros na região
            int numVertices = graph.numVertices();
            int* parent = new int[numVertices];
            int* distance = new int[numVertices];
            Dijkstra::compute(graph, v, parent, distance);

            // Encontrar a distância máxima de v para outros vértices da região
            int distanciaMaxima = 0;
            for (vertex u : regiao.vertices) {
                if (distance[u] > distanciaMaxima) {
                    distanciaMaxima = distance[u];
                }
            }

            // Verificar se este vértice tem a menor distância máxima
            if (distanciaMaxima < menorDistanciaMaxima) {
                menorDistanciaMaxima = distanciaMaxima;
                melhorVertice = v;
            }

            delete[] parent;
            delete[] distance;
        }

        // Definir o melhor vértice como a estação da região
        regiao.estacao = melhorVertice;
    }
}

// Função para construir o grafo completo das estações
void Tarefa1::construirGrafoEstacoes(GraphAdjList& graph, const std::vector<vertex>& estacoes,
                                     std::vector<std::tuple<vertex, vertex, int>>& arestasEstacoes) {
    int numVertices = graph.numVertices();

    for (size_t i = 0; i < estacoes.size(); ++i) {
        for (size_t j = i + 1; j < estacoes.size(); ++j) {
            vertex v1 = estacoes[i];
            vertex v2 = estacoes[j];

            // Executar Dijkstra entre v1 e v2 para encontrar o custo mínimo
            int* parent = new int[numVertices];
            int* distance = new int[numVertices];
            Dijkstra::compute(graph, v1, parent, distance);

            int custo = distance[v2];

            // Adicionar aresta entre as estações com o custo do caminho mínimo
            arestasEstacoes.push_back(std::make_tuple(v1, v2, custo));

            delete[] parent;
            delete[] distance;
        }
    }
}

// Função para executar o algoritmo de Kruskal e construir a MST das estações
void Tarefa1::kruskalMST(int numEstacoes, const std::vector<std::tuple<vertex, vertex, int>>& arestasEstacoes,
                         std::vector<std::tuple<vertex, vertex, int>>& mst) {
    // Ordenar as arestas em ordem crescente de peso
    std::vector<std::tuple<vertex, vertex, int>> arestasOrdenadas = arestasEstacoes;
    std::sort(arestasOrdenadas.begin(), arestasOrdenadas.end(),
              [](const std::tuple<vertex, vertex, int>& a, const std::tuple<vertex, vertex, int>& b) {
                  return std::get<2>(a) < std::get<2>(b);
              });

    // Inicializar os subconjuntos para a união
    Subconjunto* subconjuntos = new Subconjunto[numEstacoes];
    for (int i = 0; i < numEstacoes; ++i) {
        subconjuntos[i].pai = i;
        subconjuntos[i].rank = 0;
    }

    // Mapeamento de estação para índice
    std::unordered_map<vertex, int> estacaoParaIndice;
    for (int i = 0; i < numEstacoes; ++i) {
        estacaoParaIndice[i] = i;
    }

    // Iterar sobre as arestas ordenadas
    for (const auto& aresta : arestasOrdenadas) {
        vertex u = std::get<0>(aresta);
        vertex v = std::get<1>(aresta);
        int peso = std::get<2>(aresta);

        int indiceU = estacaoParaIndice[u];
        int indiceV = estacaoParaIndice[v];

        int raizU = encontrar(subconjuntos, indiceU);
        int raizV = encontrar(subconjuntos, indiceV);

        // Se incluir esta aresta não forma um ciclo
        if (raizU != raizV) {
            mst.push_back(aresta);
            unir(subconjuntos, raizU, raizV);
        }
    }

    delete[] subconjuntos;
}

// Função para imprimir o resultado
void Tarefa1::imprimirResultado(const std::vector<std::tuple<vertex, vertex, int>>& mst,
                                const std::vector<Regiao>& regioes) {
    std::cout << "Estações de Metrô por Região:\n";
    for (const auto& regiao : regioes) {
        std::cout << "Região CEP " << regiao.cep << " - Estação no vértice " << regiao.estacao << "\n";
    }

    std::cout << "\nArestas da MST conectando as estações:\n";
    for (const auto& aresta : mst) {
        vertex u = std::get<0>(aresta);
        vertex v = std::get<1>(aresta);
        int peso = std::get<2>(aresta);
        std::cout << "Aresta entre " << u << " e " << v << " com custo " << peso << "\n";
    }
}
