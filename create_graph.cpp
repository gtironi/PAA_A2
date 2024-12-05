#include "graph.h"
#include "trafficAPI.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <tuple>

// Função para determinar o maior índice de vértice no arquivo CSV
int getMaxVertex(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << filename << std::endl;
        return -1;
    }

    std::string line;
    bool isHeader = true;
    int maxVertex = 0;

    while (std::getline(file, line)) {
        if (isHeader) {
            isHeader = false;
            continue;
        }

        std::stringstream ss(line);
        std::string vOrigemStr, vDestinoStr;
        std::getline(ss, vOrigemStr, ',');
        std::getline(ss, vDestinoStr, ',');

        try {
            int vOrigem = std::stoi(vOrigemStr);
            int vDestino = std::stoi(vDestinoStr);
            maxVertex = std::max({maxVertex, vOrigem, vDestino});
        } catch (const std::exception& e) {
            std::cerr << "Erro ao processar linha: " << line << "\n"
                      << "Detalhes do erro: " << e.what() << std::endl;
        }
    }

    file.close();
    return maxVertex;
}

// Função para carregar o grafo a partir de um arquivo CSV
void loadGraphFromCSV(const std::string& filename, GraphAdjList& graph) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << filename << std::endl;
        return;
    }

    std::string line;
    bool isHeader = true;

    int lotes_type[4];

    while (std::getline(file, line)) {
        if (isHeader) {
            isHeader = false;
            continue;
        }

        std::stringstream ss(line);
        std::string vOrigemStr, vDestinoStr, bairro, onewayStr, lengthStr, maxSpeedStr, numLotesStr, casasStr, industriasStr, atracoesStr, comerciosStr;

        std::getline(ss, vOrigemStr, ',');
        std::getline(ss, vDestinoStr, ',');
        std::getline(ss, bairro, ',');
        std::getline(ss, onewayStr, ',');
        std::getline(ss, lengthStr, ',');
        std::getline(ss, maxSpeedStr, ',');
        std::getline(ss, numLotesStr, ',');
        std::getline(ss, casasStr, ',');
        std::getline(ss, industriasStr, ',');
        std::getline(ss, atracoesStr, ',');
        std::getline(ss, comerciosStr, ',');

        try {
            vertex vOrigem = std::stoi(vOrigemStr);
            vertex vDestino = std::stoi(vDestinoStr);
            bool oneway = (onewayStr == "True");
            int length = static_cast<int>(std::stof(lengthStr));
            int maxSpeed = static_cast<int>(std::stof(maxSpeedStr));
            int numLotes = static_cast<int>(std::stof(numLotesStr));
            int casas = std::stoi(casasStr);
            int industrias = std::stoi(industriasStr);
            int atracoes = std::stoi(atracoesStr);
            int comercios = std::stoi(comerciosStr);

            // Valida se os vértices estão dentro dos limites do grafo
            if (vOrigem >= graph.numVertices() || vDestino >= graph.numVertices()) {
                std::cerr << "Vértices fora do limite: " << vOrigem << ", " << vDestino << std::endl;
                continue;
            }

            // Define o tipo de lote
            lotes_type[0] = casas;
            lotes_type[1] = industrias;
            lotes_type[2] = atracoes;
            lotes_type[3] = comercios;

            // Adiciona a aresta ao grafo
            graph.addEdge(vOrigem, vDestino, bairro, length, maxSpeed, oneway, numLotes, lotes_type);
        } catch (const std::exception& e) {
            std::cerr << "Erro ao processar linha: " << line << "\n"
                      << "Detalhes do erro: " << e.what() << std::endl;
        }
    }

    file.close();
}

int main() {
    const std::string filename = "data.csv";

    // Determina o maior índice de vértice no arquivo CSV
    int maxVertex = getMaxVertex(filename);
    if (maxVertex == -1) {
        return 1; // Erro ao abrir o arquivo
    }

    GraphAdjList graph(maxVertex + 1); // Cria o grafo com base no maior índice de vértice

    // Carrega o grafo a partir do arquivo CSV
    loadGraphFromCSV(filename, graph);

    TrafficAPI trafficAPI;

    // Atualiza os multiplicadores de tráfego
    trafficAPI.updateTraffic(graph);

    // Exibe o grafo
    graph.print();

    return 0;
}
