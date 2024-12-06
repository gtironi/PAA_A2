#include "graph_utils.h"
#include "bus_v2.h"

int main() {
    const std::string filename = "graphs_regions/grafo_berlim_adjacencia.csv";

    vector<vertex> BestPath = loadAndProcessGraph(filename);

    cout << "Caminho total: ";
    for (vertex v : BestPath) {
        cout << v << " ";
    }
    cout << endl;

    return 0;
}
