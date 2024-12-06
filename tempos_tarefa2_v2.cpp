#include "graph_utils.h"
#include "bus_v2.h"

int main() {
    const std::string filename = "graphs_regions/grafo_berlim_adjacencia.csv";
    const std::string filename1 = "graphs_regions/grafo_nova_york_adjacencia.csv";
    const std::string filename2 = "graphs_regions/grafo_barcelona_adjacencia.csv";
    const std::string filename3 = "graphs_regions/grafo_florence_adjacencia.csv";

    for(const std::string& filename : {filename, filename1, filename2, filename3}) {
        vector<vertex> BestPath = loadAndProcessGraph(filename);
    }

    return 0;
}
