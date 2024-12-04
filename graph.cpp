#include "graph.h"

// Implementação da classe EdgeNode
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

void GraphAdjList::addEdge(vertex v1, vertex v2, const string& bairro, int cost, int maxSpeed, bool oneway) {
    // Adiciona a aresta de v1 para v2
    EdgeNode* edge = m_edges[v1];
    while (edge) {
        if (edge->otherVertex() == v2) {
            cout << "Aresta já existe entre " << v1 << " e " << v2 << ".\n";
            return;
        }
        edge = edge->next();
    }
    m_edges[v1] = new EdgeNode(v2, bairro, cost, maxSpeed, m_edges[v1]);
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
        cout << "Vértice " << i << ": ";
        EdgeNode* edge = m_edges[i];
        while (edge) {
            cout << "-> ("
                 << "Destino: " << edge->otherVertex()
                 << ", Bairro: " << edge->bairro()
                 << ", Peso: " << edge->cost()
                 << ", Velocidade Máxima: " << edge->maxSpeed() << ") ";
            edge = edge->next();
        }
        cout << "\n";
    }
}

void GraphAdjList::splitEdge(int vi, int vj, int distRestante) {
    // Verifica se vi e vj são válidos
    if (vi >= m_numVertices || vj >= m_numVertices || vi < 0 || vj < 0) {
        cerr << "Erro: Vértices fora do limite.\n";
        return;
    }

    // Localiza a aresta (vi, vj)
    EdgeNode* prev = nullptr;
    EdgeNode* edge = m_edges[vi];
    int cost = -1;
    int bairro = -1;
    int maxSpeed = -1;

    while (edge != nullptr) {
        if (edge->otherVertex() == vj) {
            cost = edge->cost();
            bairro = edge->bairro();
            maxSpeed = edge->maxSpeed();
            break;
        }
        prev = edge;
        edge = edge->next();
    }

    // Se não encontrou a aresta (vi, vj), retorna
    if (cost == -1) {
        cerr << "Erro: Aresta (" << vi << ", " << vj << ") não encontrada.\n";
        return;
    }

    // Remove a aresta (vi, vj)
    if (prev == nullptr) {
        m_edges[vi] = edge->next();
    } else {
        prev->setNext(edge->next());
    }
    delete edge;

    // Localiza e remove a aresta (vj, vi) de maneira similar
    prev = nullptr;
    edge = m_edges[vj];
    while (edge != nullptr) {
        if (edge->otherVertex() == vi) {
            break;
        }
        prev = edge;
        edge = edge->next();
    }
    if (edge != nullptr) {
        if (prev == nullptr) {
            m_edges[vj] = edge->next();
        } else {
            prev->setNext(edge->next());
        }
        delete edge;
    }

    // Cria um novo vértice vk
    int vk = m_numVertices++;
    EdgeNode** newEdges = new EdgeNode*[m_numVertices];
    for (int i = 0; i < m_numVertices - 1; ++i) {
        newEdges[i] = m_edges[i];
    }
    newEdges[m_numVertices - 1] = nullptr;
    delete[] m_edges;
    m_edges = newEdges;

    // Adiciona as novas arestas (vi, vk), (vk, vi), (vj, vk) e (vk, vj)
    addEdge(vi, vk, bairro, distRestante, maxSpeed, false);
    // addEdge(vk, vi, bairro, distRestante, maxSpeed, false);
    addEdge(vj, vk, bairro, cost - distRestante, maxSpeed, false);
    // addEdge(vk, vj, bairro, cost - distRestante, maxSpeed, false);
}

GraphAdjList::GraphAdjList(const GraphAdjList& other) {
    m_numVertices = other.m_numVertices;
    m_numEdges = other.m_numEdges;
    m_edges = new EdgeNode*[m_numVertices];

    for (int i = 0; i < m_numVertices; ++i) {
        EdgeNode* current = other.m_edges[i];
        EdgeNode* previous = nullptr;

        while (current) {
            EdgeNode* newEdge = new EdgeNode(current->otherVertex(), current->bairro(),
                                             current->cost(), current->maxSpeed(), nullptr);

            if (!previous) {
                m_edges[i] = newEdge; // Primeira aresta da lista
            } else {
                previous->setNext(newEdge); // Conecta à última aresta copiada
            }

            previous = newEdge;
            current = current->next();
        }

        if (!previous) {
            m_edges[i] = nullptr; // Nenhuma aresta para este vértice
        }
    }
}

GraphAdjList* createBidirectionalCopy(const GraphAdjList& original) {
    // Cria uma nova instância do grafo com o mesmo número de vértices
    GraphAdjList* bidirectionalGraph = new GraphAdjList(original.numVertices());

    // Percorre cada vértice do grafo original
    for (int i = 0; i < original.numVertices(); ++i) {
        EdgeNode* edge = original.getEdges(i);

        // Percorre todas as arestas do vértice atual
        while (edge != nullptr) {
            vertex j = edge->otherVertex();
            string bairro = edge->bairro();
            int cost = edge->cost();
            int maxSpeed = edge->maxSpeed();

            // Adiciona as arestas (i, j) e (j, i) no grafo bidirecional
            bidirectionalGraph->addEdge(i, j, bairro, cost, maxSpeed, false);
            // bidirectionalGraph->addEdge(j, i, bairro, cost, maxSpeed, false);

            edge = edge->next();
        }
    }

    return bidirectionalGraph;
}