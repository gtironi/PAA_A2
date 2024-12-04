#include "graph.h"

// Implementation of VertexInfo class
VertexInfo::VertexInfo(const string& bairro)
    : m_bairro(bairro) {}

string VertexInfo::bairro() const {
    return m_bairro;
}

void VertexInfo::setBairro(const string& bairro) {
    m_bairro = bairro;
}

// Implementation of EdgeNode class
EdgeNode::EdgeNode(vertex otherVertex, const string& bairro, int cost, int maxSpeed, EdgeNode* next)
    : m_otherVertex(otherVertex), m_bairro(bairro), m_cost(cost), m_maxSpeed(maxSpeed), m_next(next) {}

vertex EdgeNode::otherVertex() {
    return m_otherVertex;
}

string EdgeNode::bairro() {
    return m_bairro;
}

int EdgeNode::cost() {
    return m_cost;
}

int EdgeNode::maxSpeed() {
    return m_maxSpeed;
}

EdgeNode* EdgeNode::next() {
    return m_next;
}

void EdgeNode::setNext(EdgeNode* next) {
    m_next = next;
}

// Implementation of GraphAdjList class
GraphAdjList::GraphAdjList(int numVertices)
    : m_numVertices(numVertices), m_numEdges(0) {
    m_edges = new EdgeNode*[numVertices];
    m_vertexInfo = new VertexInfo[numVertices];
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
    delete[] m_vertexInfo;
}

void GraphAdjList::addEdge(vertex v1, vertex v2, const string& bairro, int cost, int maxSpeed, bool oneway) {
    // Add edge from v1 to v2
    EdgeNode* edge = m_edges[v1];
    while (edge) {
        if (edge->otherVertex() == v2) {
            // Edge already exists
            return;
        }
        edge = edge->next();
    }
    m_edges[v1] = new EdgeNode(v2, bairro, cost, maxSpeed, m_edges[v1]);
    m_numEdges++;

    // If not oneway, add edge from v2 to v1
    if (!oneway) {
        edge = m_edges[v2];
        while (edge) {
            if (edge->otherVertex() == v1) {
                // Edge already exists
                return;
            }
            edge = edge->next();
        }
        m_edges[v2] = new EdgeNode(v1, bairro, cost, maxSpeed, m_edges[v2]);
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
        cout << "Vertex " << i << " (Neighborhood: " << m_vertexInfo[i].bairro() << "): ";
        EdgeNode* edge = m_edges[i];
        while (edge) {
            cout << "-> ("
                 << "Destination: " << edge->otherVertex()
                 << ", Neighborhood: " << edge->bairro()
                 << ", Cost: " << edge->cost()
                 << ", Max Speed: " << edge->maxSpeed() << ") ";
            edge = edge->next();
        }
        cout << "\n";
    }
}

void GraphAdjList::setVertexBairro(vertex v, const string& bairro) {
    if (v >= 0 && v < m_numVertices) {
        m_vertexInfo[v].setBairro(bairro);
    }
}

string GraphAdjList::getVertexBairro(vertex v) const {
    if (v >= 0 && v < m_numVertices) {
        return m_vertexInfo[v].bairro();
    }
    return "";
}
