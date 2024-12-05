#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <set>
#include <climits>
#include <cstdlib>
using namespace std;

using vertex = int;

// Classe para armazenar as arestas do grafo
class EdgeNode {
public:
    EdgeNode(vertex otherVertex, const string& bairro, int length, int maxSpeed, bool oneway, int numLotes, int lotesType[4], EdgeNode* next);

    vertex otherVertex();
    string bairro();
    int length();
    int cost();
    int maxSpeed();
    bool isOneway();
    int numLotes();
    float coefficient_lotes();
    void getLotesType(int lotesType[4]);
    EdgeNode* next();
    void setNext(EdgeNode* next);

private:
    vertex m_otherVertex;   // Vértice
    string m_bairro;        // Bairro
    int m_length;           // Comprimento
    int m_cost;             // Custo para construir o metro
    int m_maxSpeed;         // Velocidade máxima
    bool m_oneway;          // Direção da aresta (se for unidirecional)
    int m_numLotes;         // Número de lotes
    int m_lotesType[4];     // Tipos de lotes (casas, indústrias, atrações, comércios)
    float m_coefficient_lotes;  // Coeficiente de atratividade
    EdgeNode* m_next;       // Próxima aresta na lista
};

// Classe para representar o grafo usando lista de adjacência
class GraphAdjList {
public:
    GraphAdjList(int numVertices);
    ~GraphAdjList();

    void addEdge(vertex v1, vertex v2, const string& bairro, int length, int maxSpeed, bool oneway, int numLotes, int lotesType[4]);
    void removeEdge(vertex v1, vertex v2);
    void print();
    void splitEdge(int vi, int vj, int distRestante);
    GraphAdjList* createBidirectionalCopy() const;
    GraphAdjList* clone() const;

    int numBairros() const {
        return m_numBairros;
    }

    int numVertices() const {
        return m_numVertices;
    }

    int numEdges() const {
        return m_numEdges;
    }

    EdgeNode* getEdges(vertex v) const {
        return m_edges[v];
    }

private:
    int m_numVertices;      // Número de vértices
    int m_numEdges;         // Número de arestas
    int m_numBairros;       // Número de bairros
    std::set<string> m_bairrosSet;  // Conjunto de bairros
    EdgeNode** m_edges;     // Lista de adjacência
};

#endif // GRAPH_H
