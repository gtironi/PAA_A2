#include "graph.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>

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

    while (std::getline(file, line)) {
        if (isHeader) {
            isHeader = false;
            continue;
        }

        std::stringstream ss(line);
        std::string vOrigemStr, vDestinoStr, bairro, onewayStr, lengthStr, maxSpeedStr;

        std::getline(ss, vOrigemStr, ',');
        std::getline(ss, vDestinoStr, ',');
        std::getline(ss, bairro, ',');
        std::getline(ss, onewayStr, ',');
        std::getline(ss, lengthStr, ',');
        std::getline(ss, maxSpeedStr, ',');

        try {
            vertex vOrigem = std::stoi(vOrigemStr);
            vertex vDestino = std::stoi(vDestinoStr);
            bool oneway = (onewayStr == "True");
            int length = static_cast<int>(std::stof(lengthStr));
            int maxSpeed = static_cast<int>(std::stof(maxSpeedStr));

            // Valida se os vértices estão dentro dos limites do grafo
            if (vOrigem >= graph.numVertices() || vDestino >= graph.numVertices()) {
                std::cerr << "Vértices fora do limite: " << vOrigem << ", " << vDestino << std::endl;
                continue;
            }

            // Adiciona a aresta ao grafo
            graph.addEdge(vOrigem, vDestino, bairro, length, maxSpeed, oneway);
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

    // Exibe o grafo
    graph.print();

    return 0;
}
