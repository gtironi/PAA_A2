#include "graph.h"

// Implementação da classe EdgeNode
EdgeNode::EdgeNode(vertex otherVertex, const string& bairro, int length, int maxSpeed, bool oneway, int numLotes, int lotesType[4], EdgeNode* next)
    : m_otherVertex(otherVertex), m_bairro(bairro), m_length(length), m_maxSpeed(maxSpeed), m_oneway(oneway), m_numLotes(numLotes), m_next(next) {
    // Copiar o tipo de lotes
    for (int i = 0; i < 4; ++i) {
        m_lotesType[i] = lotesType[i];
    }
}

vertex EdgeNode::otherVertex() {
    return m_otherVertex;
}

string EdgeNode::bairro() {
    return m_bairro;
}

int EdgeNode::length() {
    return m_length;
}

int EdgeNode::maxSpeed() {
    return m_maxSpeed;
}

bool EdgeNode::isOneway() {
    return m_oneway;
}

int EdgeNode::numLotes() {
    return m_numLotes;
}

void EdgeNode::getLotesType(int lotesType[4]) {
    for (int i = 0; i < 4; ++i) {
        lotesType[i] = m_lotesType[i];
    }
}

EdgeNode* EdgeNode::next() {
    return m_next;
}

void EdgeNode::setNext(EdgeNode* next) {
    m_next = next;
}

// Implementação da classe GraphAdjList
GraphAdjList::GraphAdjList(int numVertices)
    : m_numVertices(numVertices), m_numEdges(0), m_numBairros(0) {
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

void GraphAdjList::addEdge(vertex v1, vertex v2, const string& bairro, int length, int maxSpeed, bool oneway, int numLotes, int lotesType[4]) {
    // Adiciona o bairro ao conjunto de bairros
    if (m_bairrosSet.find(bairro) == m_bairrosSet.end()) {
        m_bairrosSet.insert(bairro);
        m_numBairros++;
    }

    // Adiciona a aresta de v1 para v2
    EdgeNode* edge = m_edges[v1];
    while (edge) {
        if (edge->otherVertex() == v2) {
            return; // Aresta já existe
        }
        edge = edge->next();
    }
    m_edges[v1] = new EdgeNode(v2, bairro, length, maxSpeed, oneway, numLotes, lotesType, m_edges[v1]);
    m_numEdges++;

    // Se não for oneway, adiciona a aresta de v2 para v1
    if (!oneway) {
        edge = m_edges[v2];
        while (edge) {
            if (edge->otherVertex() == v1) {
                return; // Já existe
            }
            edge = edge->next();
        }
        m_edges[v2] = new EdgeNode(v1, bairro, length, maxSpeed, oneway, numLotes, lotesType, m_edges[v2]);
        m_numEdges++;
    }
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
                 << ", Comprimento: " << edge->length()
                 << ", Velocidade Máxima: " << edge->maxSpeed()
                 << ", Oneway: " << (edge->isOneway() ? "Sim" : "Não")
                 << ", Lotes: " << edge->numLotes() << ") ";
            edge = edge->next();
        }
        cout << "\n";
    }
}
