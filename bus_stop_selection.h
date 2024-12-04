#ifndef BUS_STOP_SELECTION_H
#define BUS_STOP_SELECTION_H

#include "graph.h"
#include <unordered_map>
#include <string>
#include <vector>

class BusStopSelection {
public:
    // Method to select optimal bus stops for each neighborhood
    static void selectBusStops(GraphAdjList& graph,
                               std::unordered_map<std::string, vertex>& busStops);
};

#endif // BUS_STOP_SELECTION_H
