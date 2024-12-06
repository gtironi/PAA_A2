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
#include <ctime>

#include "graph.h"
#include "dijkstra.h"
#include "prim.h"
#include "list.h"
#include "bus.h"
#include "taxi.h"
#include "trafficAPI.h"
#include "utills.h"
#include "metro.h"
#include "graph_utils.h"

using namespace std;


int main() {
    const std::string filename = "graphs_regions/grafo_berlim_adjacencia.csv";

    cout << "Grafo baseado na ciade de berlim!" << endl;

    // Determina o maior índice de vértice no arquivo CSV
    int maxVertex = getMaxVertex(filename);
    if (maxVertex == -1) {
        return 1; // Erro ao abrir o arquivo
    }

    GraphAdjList graph(maxVertex + 1); // Cria o grafo com base no maior índice de vértice

    // Carrega o grafo a partir do arquivo CSV
    loadGraphFromCSV(filename, graph);

    // Defina aqui os vértices de origem e destino e o custo máximo, respectivamente
    melhorRota(graph, 30, 1000, 20); // a função irá printar o melhor caminho e o tempo gasto
    //                /\  /\   /\\
    //         Origem ||  ||   ||
    //            Destino ||   ||
    //                Dinheiro || 

    return 0;
}