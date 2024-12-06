#include <climits> // Para INT_MAX
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <limits>     // Para numeric_limits
#include <queue>
#include <set>
#include <chrono>
#include <fstream>

using namespace std;
using namespace chrono;

#include "graph.h"
#include "dijkstra.h"
#include "prim.h"
#include "list.h"
#include "taxi.h"
#include "bus.h"
#include "utills.h"

unordered_map<std::string, std::pair<vertex, vertex>> selecionarArestasMinimasPorBairro(GraphAdjList& grafo) {
    std::unordered_map<std::string, std::pair<vertex, vertex>> arestasMinimasPorBairro;
    std::unordered_map<std::string, int> custosMinimosPorBairro;
    // Itera sobre todos os vértices do grafo
    for (vertex u = 0; u < grafo.numVertices(); u++) {
        EdgeNode* aresta = grafo.getEdges(u);
        // Itera sobre todas as arestas de cada vértice
        while (aresta) {
            vertex v = aresta->otherVertex();
            std::string bairro = aresta->bairro();
            int custo = aresta->cost();
            // Se o bairro ainda não foi encontrado ou a aresta atual tem custo menor
            if (arestasMinimasPorBairro.find(bairro) == arestasMinimasPorBairro.end() || custo < custosMinimosPorBairro[bairro]) {
                // Atualiza a aresta e o custo mínimo
                arestasMinimasPorBairro[bairro] = {u, v};
                custosMinimosPorBairro[bairro] = custo;
            }
            aresta = aresta->next();
        }
    }
    return arestasMinimasPorBairro;
}


vector<vertex> reconstruirCaminho(vertex origem, vertex destino, const vector<vertex>& parent) {
    vector<vertex> caminho;
    // Itera sobre todos os vértices, do fim para o inicio
    for (vertex v = destino; v != origem; v = parent[v]) {
        caminho.push_back(v);
    }
    caminho.push_back(origem);
    reverse(caminho.begin(), caminho.end());
    return caminho;
}

std::vector<vertex> encontrarCicloArestasMinimas(GraphAdjList& subgrafo, const std::unordered_map<std::string, std::pair<vertex, vertex>>& arestasMinimas) {
    std::vector<vertex> ciclo;
    std::set<std::pair<vertex, vertex>> arestasAdicionadas;
    // Adiciona os vértices das arestas mínimas ao ciclo
    for (const auto& bairro : arestasMinimas) {
        vertex u = bairro.second.first;
        vertex v = bairro.second.second;
        if (std::find(ciclo.begin(), ciclo.end(), u) == ciclo.end()) {
            ciclo.push_back(u);
        }
        if (std::find(ciclo.begin(), ciclo.end(), v) == ciclo.end()) {
            ciclo.push_back(v);
        }
        arestasAdicionadas.insert({std::min(u, v), std::max(u, v)});
    }
    // Conecta os vértives que foram adicionados ao ciclo
    for (size_t i = 0; i < ciclo.size() - 1; ++i) {
        vertex u = ciclo[i];
        vertex v = ciclo[i + 1];
        // Se a aresta ainda nao foi adicionada
        if (arestasAdicionadas.find({std::min(u, v), std::max(u, v)}) == arestasAdicionadas.end()) {
            std::vector<vertex> caminho;
            int* parent = new int[subgrafo.numVertices()];
            int* distance = new int[subgrafo.numVertices()];
            // Encontra o menor caminho entre os vertices
            Dijkstra::compute(subgrafo, u, parent, distance);
            caminho = reconstruirCaminho(u, v, std::vector<vertex>(parent, parent + subgrafo.numVertices()));
            // Adiciona o caminho ao ciclo
            for (size_t j = 1; j < caminho.size(); ++j) {
                if (std::find(ciclo.begin(), ciclo.end(), caminho[j]) == ciclo.end()) {
                    ciclo.insert(ciclo.begin() + i + 1, caminho[j]);
                    ++i;
                }
            }
            delete[] parent;
            delete[] distance;
        }
    }
    // Conecta o último vértice ao primeiro
    vertex u = ciclo.back();
    vertex v = ciclo.front();
    // Se a aresta ainda nao foi adicionada
    if (arestasAdicionadas.find({std::min(u, v), std::max(u, v)}) == arestasAdicionadas.end()) {
        std::vector<vertex> caminho;
        int* parent = new int[subgrafo.numVertices()];
        int* distance = new int[subgrafo.numVertices()];
        // Encontra o menor caminho
        Dijkstra::compute(subgrafo, u, parent, distance);
        caminho = reconstruirCaminho(u, v, std::vector<vertex>(parent, parent + subgrafo.numVertices()));
        for (size_t j = 1; j < caminho.size(); ++j) {
            if (std::find(ciclo.begin(), ciclo.end(), caminho[j]) == ciclo.end()) {
                ciclo.push_back(caminho[j]);
            }
        }
        delete[] parent;
        delete[] distance;
    }
    // Adiciona o primeiro vertice pra fechar o ciclo
    ciclo.push_back(v);
    return ciclo;
}

void testarDesempenho() {
    vector<int> tamanhos = {1000, 2000, 4000, 40000}; // Tamanhos dos grafos para teste
    vector<double> tempos; // Armazena os tempos de execução
    int lotesType[4] = {1, 0, 0, 0};

    for (int tamanho : tamanhos) {
        // Criação do grafo de teste
        GraphAdjList grafo(tamanho);
        for (int i = 0; i < tamanho - 1; ++i) {
            grafo.addEdge(i, i + 1, "BairroA", rand() % 50 + 1, 50, false, 1, lotesType);
            if (i % 2 == 0) {
                grafo.addEdge(i, (i + 2) % tamanho, "BairroB", rand() % 50 + 1, 50, false, 1, lotesType);
            }
        }

        // Selecione as arestas mínimas por bairro
        auto arestasMinimas = selecionarArestasMinimasPorBairro(grafo);

        // Medir tempo de execução
        auto inicio = high_resolution_clock::now();
        auto ciclo = encontrarCicloArestasMinimas(grafo, arestasMinimas);
        auto fim = high_resolution_clock::now();

        // Calcula o tempo em milissegundos
        double duracao = duration_cast<milliseconds>(fim - inicio).count();
        tempos.push_back(duracao);

        cout << "Tamanho do grafo: " << tamanho << " | Tempo: " << duracao << " ms\n";
    }

    // Salvar dados para plotar gráfico
    ofstream dataFile("tempo_execucao.txt");
    for (size_t i = 0; i < tamanhos.size(); ++i) {
        dataFile << tamanhos[i] << " " << tempos[i] << endl;
    }
    dataFile.close();
}

float calculaTempoOnibus(GraphAdjList& graph, IntList& caminhoOnibus) {
    float tempo = 0.0f;

    // Obtém o primeiro nó da lista
    IntNode* current = caminhoOnibus.head;

    // Itera pelos pares consecutivos de nós
    while (current && current->next) {
        int u = current->data;
        int v = current->next->data;

        // Busca a aresta conectando u a v
        EdgeNode* edge = graph.getEdges(u);
        bool encontrou = false;

        while (edge) {
            int v2 = edge->otherVertex();
            if (v2 == v) {
                float cost = (float)(edge->length())/1000;
                int v_max = edge->maxSpeed();
                float modf_transito = edge->traffic_multiplier;
                tempo += cost / (v_max * modf_transito);
                encontrou = true;
                break;
            }
            edge = edge->next();
        }

        if (!encontrou) {
            cerr << "Erro: Não foi encontrada uma aresta entre " << u << " e " << v << endl;
            break;
        }

        // Avança para o próximo nó
        current = current->next;
    }

    return tempo;
}
float rotaOnibus(GraphAdjList& graphCompleto, GraphAdjList& graphDirecionado, std::vector<vertex> linhaOnibus, NodeList& rota, int origem, int destino, float custoMax, float horaAtual) {
    // Custo do ônibus
    const float CUSTO_ONIBUS = 4.50;

    // Verificar se o custo disponível é suficiente para pegar um ônibus
    if (custoMax < CUSTO_ONIBUS) {
        return std::numeric_limits<float>::max();
    }

    // Reduzir o custo máximo pelo valor do ônibus
    custoMax -= CUSTO_ONIBUS;

    // Encontrar vértices mais próximos na linha de ônibus
    Dijkstra dijkstra;
    int parentOrigem[graphCompleto.numVertices()];
    int distanceOrigem[graphCompleto.numVertices()];
    dijkstra.compute(graphCompleto, origem, parentOrigem, distanceOrigem);

    int parentDestino[graphCompleto.numVertices()];
    int distanceDestino[graphCompleto.numVertices()];
    dijkstra.compute(graphCompleto, destino, parentDestino, distanceDestino);

    vertex embarque = -1;
    vertex desembarque = -1;
    int menorDistOrigem = std::numeric_limits<int>::max();
    int menorDistDestino = std::numeric_limits<int>::max();

    // Determinar o vértice mais próximo na linha de ônibus para embarcar e desembarcar
    int idx_embarque;
    int idx_desembarque;
    for (int i=0; i<linhaOnibus.size();i++) {
        int v = linhaOnibus[i];
        if (distanceOrigem[v] < menorDistOrigem) {
            menorDistOrigem = distanceOrigem[v];
            embarque = v;
            idx_embarque = i;
        }
        if (distanceDestino[v] < menorDistDestino) {
            menorDistDestino = distanceDestino[v];
            desembarque = v;
            idx_desembarque = i;
        }
    }

    // Se não encontrou vértices viáveis na linha de ônibus
    if (embarque == -1 || desembarque == -1) {
        return std::numeric_limits<float>::max();
    }

    IntList caminhoOnibus;
    int i=idx_embarque;
    while (i!=idx_desembarque) {
        caminhoOnibus.append(linhaOnibus[i]);
        i++;
        if (i==linhaOnibus.size() - 1){i=0;}
    }
    caminhoOnibus.append(linhaOnibus[i]);


    float tempoOnibus = 0;
    float tempoSaida = 0;
    float tempoIda = 0;
    float horario = horaAtual;
    
    if (embarque != desembarque){
        tempoIda = rotaTaxi(graphCompleto, graphDirecionado, rota, origem, embarque, custoMax, custoMax);
        // if (embarque != destino){
        horario += tempoIda;
        tempoOnibus = calculaTempoOnibus(graphDirecionado, caminhoOnibus);
        tempoOnibus += calcularFaltante(horario);
        rota.append(BUS_CODE, tempoOnibus, &caminhoOnibus);
        // }
        tempoSaida = rotaTaxi(graphCompleto, graphDirecionado, rota, desembarque, destino, custoMax, custoMax);
    } else{
        tempoSaida = rotaTaxi(graphCompleto, graphDirecionado, rota, origem, destino, custoMax, custoMax);
    }

    return tempoIda + tempoOnibus + tempoSaida;
}

// int main() {

//     int lotesType[4] = {1, 0, 0, 0};

//     // Criação do grafo de teste com 10 vértices
//     GraphAdjList grafo(10);
//     grafo.addEdge(0, 1, "BairroA", 10, 50, false, 1, lotesType);
//     grafo.addEdge(1, 2, "BairroA", 20, 40, false, 1, lotesType);
//     grafo.addEdge(2, 3, "BairroA", 15, 45, false, 1, lotesType);
//     grafo.addEdge(3, 4, "BairroA", 20, 35, false, 1, lotesType);
//     grafo.addEdge(1, 3, "BairroA", 5, 35, false, 1, lotesType);
//     grafo.addEdge(0, 4, "BairroA", 10, 35, false, 1, lotesType);
//     grafo.addEdge(4, 5, "BairroA", 30, 40, false, 1, lotesType);
//     grafo.addEdge(9, 0, "BairroB", 25, 45, false, 1, lotesType);
//     grafo.addEdge(5, 6, "BairroB", 10, 60, false, 1, lotesType);
//     grafo.addEdge(6, 7, "BairroB", 25, 55, false, 1, lotesType);
//     grafo.addEdge(6, 9, "BairroB", 25, 55, false, 1, lotesType);
//     grafo.addEdge(7, 8, "BairroB", 35, 50, false, 1, lotesType);
//     grafo.addEdge(8, 9, "BairroB", 40, 45, false, 1, lotesType);

//     // // Selecione as arestas mínimas por bairro
//     // auto arestasMinimas = selecionarArestasMinimasPorBairro(grafo);
//     // cout << "Arestas mínimas por bairro (apenas vértices):\n";
//     // for (const auto& bairro : arestasMinimas) {
//     //     vertex u = bairro.second.first;
//     //     vertex v = bairro.second.second;
//     //     cout << "Origem: " << u << " | Destino: " << v << endl;
//     // }
//     // // Encontra o ciclo para a rota de onibus
//     // auto ciclo = encontrarCicloArestasMinimas(grafo, arestasMinimas);
//     // cout << "Ciclo passando pelas arestas mínimas:\n";
//     // for (vertex v : ciclo) {
//     //     cout << v << " ";
//     // }
//     // cout << endl;

//     testarDesempenho();
//     return 0;
// }
