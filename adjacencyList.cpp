#include <iostream>
#include <string>
using namespace std;

using vertex = int;

class EdgeNode {
public:
    EdgeNode(vertex otherVertex, const string& neighborhood, int weight, EdgeNode* next)
        : m_otherVertex(otherVertex)
        , m_neighborhood(neighborhood)
        , m_weight(weight)
        , m_next(next) {}

    vertex otherVertex() {
        return m_otherVertex;
    }

    string neighborhood() {
        return m_neighborhood;
    }

    int weight() {
        return m_weight;
    }

    EdgeNode* next() {
        return m_next;
    }

    void setNext(EdgeNode* next) {
        m_next = next;
    }

private:
    vertex m_otherVertex;   // Vértice conectado
    string m_neighborhood;  // Bairro da conexão
    int m_weight;           // Peso da conexão
    EdgeNode* m_next;       // Próxima aresta na lista
};

// Classe para representar o grafo usando lista de adjacência
class GraphAdjList {
public:
    GraphAdjList(int numVertices)
        : m_numVertices(numVertices)
        , m_numEdges(0)
        , m_edges(nullptr) {
        m_edges = new EdgeNode * [numVertices];
        for (vertex i = 0; i < numVertices; i++) {
            m_edges[i] = nullptr;
        }
    }

    ~GraphAdjList() {
        for (vertex i = 0; i < m_numVertices; i++) {
            EdgeNode* edge = m_edges[i];
            while (edge) {
                EdgeNode* next = edge->next();
                delete edge;
                edge = next;
            }
        }
        delete[] m_edges;
    }

    void addEdge(vertex v1, vertex v2, const string& neighborhood, int weight) {
        // Verifica se a aresta já existe
        EdgeNode* edge = m_edges[v1];
        while (edge) {
            if (edge->otherVertex() == v2) {
                cout << "Aresta já existe entre " << v1 << " e " << v2 << ".\n";
                return;
            }
            edge = edge->next();
        }

        // Adiciona a nova aresta na lista de adjacência
        m_edges[v1] = new EdgeNode(v2, neighborhood, weight, m_edges[v1]);
        m_numEdges++;
    }

    void removeEdge(vertex v1, vertex v2) {
        EdgeNode* edge = m_edges[v1];
        EdgeNode* previousEdge = nullptr;
        while (edge) {
            if (edge->otherVertex() == v2) {
                if (previousEdge) {
                    previousEdge->setNext(edge->next());
                } else {
                    m_edges[v1] = edge->next();
                }
                delete edge;
                m_numEdges--;
                return;
            }
            previousEdge = edge;
            edge = edge->next();
        }
    }

private:
    int m_numVertices;      // Número de vértices
    int m_numEdges;         // Número de arestas
    EdgeNode** m_edges;     // Lista de adjacência
};
