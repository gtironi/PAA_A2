#include "graph.h"
#include "bus_stop_selection.h"
#include "mst_construction.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <vector>

// Function to determine the maximum vertex index in the CSV file
int getMaxVertex(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening the file " << filename << std::endl;
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
            std::cerr << "Error processing line: " << line << "\n"
                      << "Error details: " << e.what() << std::endl;
        }
    }

    file.close();
    return maxVertex;
}

// Function to load the graph from a CSV file
void loadGraphFromCSV(const std::string& filename, GraphAdjList& graph) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening the file " << filename << std::endl;
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

            if (vOrigem >= graph.numVertices() || vDestino >= graph.numVertices()) {
                std::cerr << "Vertices out of bounds: " << vOrigem << ", " << vDestino << std::endl;
                continue;
            }

            graph.addEdge(vOrigem, vDestino, bairro, length, maxSpeed, oneway);

            // Set the neighborhood of the vertices
            graph.setVertexBairro(vOrigem, bairro);
            graph.setVertexBairro(vDestino, bairro);

        } catch (const std::exception& e) {
            std::cerr << "Error processing line: " << line << "\n"
                      << "Error details: " << e.what() << std::endl;
        }
    }

    file.close();
}

int main() {
    const std::string filename = "data.csv";

    int maxVertex = getMaxVertex(filename);
    if (maxVertex == -1) {
        return 1; // Error opening the file
    }

    GraphAdjList graph(maxVertex + 1);

    loadGraphFromCSV(filename, graph);

    // Select bus stops
    std::unordered_map<std::string, vertex> busStops;
    BusStopSelection::selectBusStops(graph, busStops);

    // Connect bus stops with MST
    std::vector<std::pair<vertex, vertex>> mstEdges;
    MSTConstruction::constructMST(graph, busStops, mstEdges);

    // Display the results
    std::cout << "Selected Bus Stops:\n";
    for (const auto& pair : busStops) {
        std::cout << "Neighborhood: " << pair.first << ", Vertex: " << pair.second << "\n";
    }

    std::cout << "\nMST Edges:\n";
    for (const auto& edge : mstEdges) {
        std::cout << "From " << edge.first << " to " << edge.second << "\n";
    }

    return 0;
}
