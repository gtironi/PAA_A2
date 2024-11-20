#include "graph.h"

// Implementação da classe EdgeNode
EdgeNode::EdgeNode(vertex otherVertex, const string& bairro, int cost, EdgeNode* next)
    : m_otherVertex(otherVertex), m_bairro(bairro), m_cost(cost), m_next(next) {}

vertex EdgeNode::otherVertex() {
    return m_otherVertex;
}

string EdgeNode::bairro() {
    return m_bairro;
}

int EdgeNode::cost() {
    return m_cost;
}

EdgeNode* EdgeNode::next() {
    return m_next;
}

void EdgeNode::setNext(EdgeNode* next) {
    m_next = next;
}

// Implementação da classe GraphAdjList
GraphAdjList::GraphAdjList(int numVertices)
    : m_numVertices(numVertices), m_numEdges(0) {
    m_edges = new EdgeNode*[numVertices];
    for (vertex i = 0; i < numVertices; i++) {
        m_edges[i] = nullptr;
    }
}

GraphAdjList::~GraphAdjList() {
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

void GraphAdjList::addEdge(vertex v1, vertex v2, const string& bairro, int cost) {
    EdgeNode* edge = m_edges[v1];
    while (edge) {
        if (edge->otherVertex() == v2) {
            cout << "Aresta já existe entre " << v1 << " e " << v2 << ".\n";
            return;
        }
        edge = edge->next();
    }
    m_edges[v1] = new EdgeNode(v2, bairro, cost, m_edges[v1]);
    m_numEdges++;
}

void GraphAdjList::removeEdge(vertex v1, vertex v2) {
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

void GraphAdjList::print() {
    for (vertex i = 0; i < m_numVertices; i++) {
        cout << "Vértice " << i << ": ";
        EdgeNode* edge = m_edges[i];
        while (edge) {
            cout << "-> ("
                 << "Destino: " << edge->otherVertex()
                 << ", Bairro: " << edge->bairro()
                 << ", Peso: " << edge->cost() << ") ";
            edge = edge->next();
        }
        cout << "\n";
    }
}
