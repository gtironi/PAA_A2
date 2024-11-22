#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
using namespace std;

using vertex = int;

// Classe para armazenar as arestas do grafo
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
    vertex m_otherVertex;   // Vértice
    string m_bairro;        // Bairro
    int m_cost;             // Peso
    int m_maxSpeed;         // Velocidade máxima
    EdgeNode* m_next;       // Próxima aresta na lista
};

// Classe para representar o grafo usando lista de adjacência
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

private:
    int m_numVertices;      // Número de vértices
    int m_numEdges;         // Número de arestas
    EdgeNode** m_edges;     // Lista de adjacência
};

#endif // GRAPH_H
