#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

using vertex = int;

// Forward declaration
class EdgeNode;

// Class to store vertex information
class VertexInfo {
public:
    VertexInfo(const string& bairro = "");

    string bairro() const;
    void setBairro(const string& bairro);

private:
    string m_bairro;  // Neighborhood of the vertex
};

// Class to store the edges of the graph
class EdgeNode {
public:
    EdgeNode(vertex otherVertex, const string& bairro, int cost, int maxSpeed, EdgeNode* next);

    vertex otherVertex();
    string bairro();
    int cost();
    int maxSpeed();
    EdgeNode* next();
    void setNext(EdgeNode* next);

private:
    vertex m_otherVertex;   // Vertex
    string m_bairro;        // Neighborhood
    int m_cost;             // Cost (excavation cost)
    int m_maxSpeed;         // Maximum speed
    EdgeNode* m_next;       // Next edge in the list
};

// Class to represent the graph using adjacency lists
class GraphAdjList {
public:
    GraphAdjList(int numVertices);
    ~GraphAdjList();

    void addEdge(vertex v1, vertex v2, const string& bairro, int cost, int maxSpeed, bool oneway);
    void removeEdge(vertex v1, vertex v2);
    void print();

    int numVertices() const {
        return m_numVertices;
    }

    EdgeNode* getEdges(vertex v) const {
        return m_edges[v];
    }

    // New method to set the neighborhood of a vertex
    void setVertexBairro(vertex v, const string& bairro);

    // New method to get the neighborhood of a vertex
    string getVertexBairro(vertex v) const;

private:
    int m_numVertices;      // Number of vertices
    int m_numEdges;         // Number of edges
    EdgeNode** m_edges;     // Adjacency list

    // Array to store vertex information
    VertexInfo* m_vertexInfo;
};

#endif // GRAPH_H
