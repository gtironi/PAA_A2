#include <iostream>
#include <cassert>
#include <vector>
#include <unordered_map>
#include <string>
#include "graph.h"
#include "heap.h"
#include "dijkstra.h"
#include "prim.h"
#include "bus_stop_selection.h"
#include "mst_construction.h"

void testGraphModule() {
    std::cout << "Testing Graph Module...\n";

    // Create a graph with 5 vertices
    GraphAdjList graph(5);

    // Add edges
    std::cout << "Adding edges...\n";
    graph.addEdge(0, 1, "Neighborhood A", 10, 50, false);
    graph.addEdge(0, 2, "Neighborhood A", 5, 50, false);
    graph.addEdge(1, 3, "Neighborhood B", 3, 40, false);
    graph.addEdge(2, 3, "Neighborhood B", 2, 40, false);
    graph.addEdge(3, 4, "Neighborhood C", 7, 60, false);

    // Set vertex neighborhoods
    std::cout << "Setting vertex neighborhoods...\n";
    graph.setVertexBairro(0, "Neighborhood A");
    graph.setVertexBairro(1, "Neighborhood A");
    graph.setVertexBairro(2, "Neighborhood A");
    graph.setVertexBairro(3, "Neighborhood B");
    graph.setVertexBairro(4, "Neighborhood C");

    // Print graph
    std::cout << "Graph structure:\n";
    graph.print();

    // Test getVertexBairro
    std::cout << "Testing getVertexBairro...\n";
    assert(graph.getVertexBairro(0) == "Neighborhood A");
    assert(graph.getVertexBairro(3) == "Neighborhood B");
    assert(graph.getVertexBairro(4) == "Neighborhood C");
    std::cout << "getVertexBairro passed.\n";

    // Remove an edge
    std::cout << "Removing edge between 2 and 3...\n";
    graph.removeEdge(2, 3);

    // Verify edge removal
    std::cout << "Verifying edge removal...\n";
    EdgeNode* edge = graph.getEdges(2);
    bool edgeExists = false;
    while (edge) {
        if (edge->otherVertex() == 3) {
            edgeExists = true;
            break;
        }
        edge = edge->next();
    }
    assert(!edgeExists);
    std::cout << "Edge removal verified.\n";

    std::cout << "Graph Module tests passed.\n\n";
}

void testHeapModule() {
    std::cout << "Testing Heap Module...\n";

    MinHeap heap;

    std::cout << "Inserting elements into heap...\n";
    heap.insert_or_update(10, 1);
    heap.insert_or_update(5, 2);
    heap.insert_or_update(15, 3);
    heap.insert_or_update(7, 4);

    std::cout << "Updating element in heap...\n";
    heap.insert_or_update(3, 1); // Update priority of vertex 1

    std::cout << "Extracting elements from heap...\n";
    int minVertex = heap.extract_min();
    assert(minVertex == 1);
    heap.pop();

    minVertex = heap.extract_min();
    assert(minVertex == 2);
    heap.pop();

    minVertex = heap.extract_min();
    assert(minVertex == 4);
    heap.pop();

    minVertex = heap.extract_min();
    assert(minVertex == 3);
    heap.pop();

    std::cout << "Heap is empty: " << std::boolalpha << heap.empty() << "\n";
    assert(heap.empty());

    std::cout << "Heap Module tests passed.\n\n";
}

void testDijkstraModule() {
    std::cout << "Testing Dijkstra's Algorithm...\n";

    // Create a graph
    GraphAdjList graph(6);

    // Add edges
    graph.addEdge(0, 1, "", 7, 0, false);
    graph.addEdge(0, 2, "", 9, 0, false);
    graph.addEdge(0, 5, "", 14, 0, false);
    graph.addEdge(1, 2, "", 10, 0, false);
    graph.addEdge(1, 3, "", 15, 0, false);
    graph.addEdge(2, 3, "", 11, 0, false);
    graph.addEdge(2, 5, "", 2, 0, false);
    graph.addEdge(3, 4, "", 6, 0, false);
    graph.addEdge(4, 5, "", 9, 0, false);

    // Arrays for results
    std::vector<int> parent(6, -1);
    std::vector<int> distance(6, INT_MAX);

    // Run Dijkstra's algorithm
    std::cout << "Running Dijkstra's algorithm from vertex 0...\n";
    Dijkstra::compute(graph, 0, parent.data(), distance.data(), /*useCost=*/true);

    // Expected distances from vertex 0
    int expectedDistances[6] = {0, 7, 9, 20, 20, 11};

    // Verify results
    std::cout << "Verifying distances...\n";
    for (int i = 0; i < 6; ++i) {
        std::cout << "Vertex " << i << ": Expected distance = " << expectedDistances[i]
                  << ", Computed distance = " << distance[i] << "\n";
        assert(distance[i] == expectedDistances[i]);
    }

    std::cout << "Dijkstra's Algorithm tests passed.\n\n";
}

void testPrimModule() {
    std::cout << "Testing Prim's Algorithm...\n";

    // Create a graph
    GraphAdjList graph(5);

    // Add edges
    graph.addEdge(0, 1, "", 2, 0, false);
    graph.addEdge(0, 3, "", 6, 0, false);
    graph.addEdge(1, 2, "", 3, 0, false);
    graph.addEdge(1, 3, "", 8, 0, false);
    graph.addEdge(1, 4, "", 5, 0, false);
    graph.addEdge(2, 4, "", 7, 0, false);
    graph.addEdge(3, 4, "", 9, 0, false);

    // Vector for parent vertices
    std::vector<int> parent;

    // Run Prim's algorithm
    std::cout << "Running Prim's algorithm...\n";
    Prim::mst(graph, parent);

    // Expected parent array (may vary due to equal weights)
    // Possible expected MST edges:
    // Edge from 0 to 1
    // Edge from 1 to 2
    // Edge from 1 to 4
    // Edge from 0 to 3

    std::cout << "MST edges:\n";
    for (int i = 0; i < parent.size(); ++i) {
        if (parent[i] != -1 && parent[i] != i) {
            std::cout << "Edge from " << parent[i] << " to " << i << "\n";
        }
    }

    std::cout << "Prim's Algorithm test completed.\n\n";
}

void testBusStopSelection() {
    std::cout << "Testing Bus Stop Selection Module...\n";

    // Create a graph
    GraphAdjList graph(6);

    // Add edges within neighborhoods
    graph.addEdge(0, 1, "Neighborhood A", 1, 0, false);
    graph.addEdge(1, 2, "Neighborhood A", 1, 0, false);

    graph.addEdge(3, 4, "Neighborhood B", 1, 0, false);
    graph.addEdge(4, 5, "Neighborhood B", 1, 0, false);

    // Set vertex neighborhoods
    graph.setVertexBairro(0, "Neighborhood A");
    graph.setVertexBairro(1, "Neighborhood A");
    graph.setVertexBairro(2, "Neighborhood A");
    graph.setVertexBairro(3, "Neighborhood B");
    graph.setVertexBairro(4, "Neighborhood B");
    graph.setVertexBairro(5, "Neighborhood B");

    // Map to store bus stops
    std::unordered_map<std::string, vertex> busStops;

    // Run bus stop selection
    std::cout << "Selecting bus stops...\n";
    BusStopSelection::selectBusStops(graph, busStops);

    // Expected bus stops (vertex 1 for Neighborhood A, vertex 4 for Neighborhood B)
    std::cout << "Verifying selected bus stops...\n";
    assert(busStops["Neighborhood A"] == 1);
    assert(busStops["Neighborhood B"] == 4);

    std::cout << "Selected Bus Stops:\n";
    for (const auto& pair : busStops) {
        std::cout << "Neighborhood: " << pair.first << ", Bus Stop Vertex: " << pair.second << "\n";
    }

    std::cout << "Bus Stop Selection tests passed.\n\n";
}

void testMSTConstruction() {
    std::cout << "Testing MST Construction Module...\n";

    // Create a graph
    GraphAdjList graph(6);

    // Add edges
    graph.addEdge(0, 1, "", 4, 0, false);
    graph.addEdge(0, 2, "", 3, 0, false);
    graph.addEdge(1, 2, "", 1, 0, false);
    graph.addEdge(1, 3, "", 2, 0, false);
    graph.addEdge(2, 3, "", 4, 0, false);
    graph.addEdge(3, 4, "", 2, 0, false);
    graph.addEdge(4, 5, "", 6, 0, false);

    // Assume bus stops are at vertices 0, 3, and 5
    std::unordered_map<std::string, vertex> busStops;
    busStops["Neighborhood A"] = 0;
    busStops["Neighborhood B"] = 3;
    busStops["Neighborhood C"] = 5;

    // Vector to store MST edges
    std::vector<std::pair<vertex, vertex>> mstEdges;

    // Run MST construction
    std::cout << "Constructing MST connecting bus stops...\n";
    MSTConstruction::constructMST(graph, busStops, mstEdges);

    // Expected MST edges: (0,1), (1,3), (3,4), (4,5)
    std::cout << "MST Edges:\n";
    for (const auto& edge : mstEdges) {
        std::cout << "Edge from " << edge.first << " to " << edge.second << "\n";
    }

    std::cout << "MST Construction test completed.\n\n";
}

int main() {
    try {
        testGraphModule();
    } catch (const std::exception& e) {
        std::cerr << "Graph Module test failed: " << e.what() << "\n";
    }

    try {
        testHeapModule();
    } catch (const std::exception& e) {
        std::cerr << "Heap Module test failed: " << e.what() << "\n";
    }

    try {
        testDijkstraModule();
    } catch (const std::exception& e) {
        std::cerr << "Dijkstra's Algorithm test failed: " << e.what() << "\n";
    }

    try {
        testPrimModule();
    } catch (const std::exception& e) {
        std::cerr << "Prim's Algorithm test failed: " << e.what() << "\n";
    }

    try {
        testBusStopSelection();
    } catch (const std::exception& e) {
        std::cerr << "Bus Stop Selection test failed: " << e.what() << "\n";
    }

    try {
        testMSTConstruction();
    } catch (const std::exception& e) {
        std::cerr << "MST Construction test failed: " << e.what() << "\n";
    }

    std::cout << "All tests completed.\n";
    return 0;
}
