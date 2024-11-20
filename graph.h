#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
using namespace std;

using vertex = int;

// Classe para armazenar as arestas do grafo
class EdgeNode {
public:
	EdgeNode(vertex otherVertex, const string& bairro, int cost, EdgeNode* next);

	vertex otherVertex();
	string bairro();
	int cost();
	EdgeNode* next();
	void setNext(EdgeNode* next);

private:
	vertex m_otherVertex;   // Vertice
	string m_bairro;  // Bairro
	int m_cost;           // Peso
	EdgeNode* m_next;       // Proxima aresta na lista
};

// Classe para representar o grafo usando lista de adjacencia
class GraphAdjList {
public:
	GraphAdjList(int numVertices);
	~GraphAdjList();

	void addEdge(vertex v1, vertex v2, const string& bairro, int cost);
	void removeEdge(vertex v1, vertex v2);
	void print();

	int numVertices() const {
		return m_numVertices;
	}

	EdgeNode* getEdges(vertex v) const {
		return m_edges[v];
	}

private:
	int m_numVertices;      // # vertices
	int m_numEdges;         // # arestas
	EdgeNode** m_edges;     
};

#endif // GRAPH_H
