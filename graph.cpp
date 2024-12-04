#include "graph.h"

// Implementação da classe EdgeNode ------------------------------------------
EdgeNode::EdgeNode(vertex otherVertex, const string& bairro, int length, int maxSpeed, bool oneway, int numLotes, int lotesType[4], EdgeNode* next)
    : m_otherVertex(otherVertex), m_bairro(bairro), m_length(length), m_maxSpeed(maxSpeed), traffic_multiplier(1.0f), m_oneway(oneway), m_numLotes(numLotes), m_next(next) {
    // Copiar o tipo de lotes
    for (int i = 0; i < 4; ++i) {
        m_lotesType[i] = lotesType[i];
    }

    // Calculo do coeficiente de atratividade
    int numCasas = m_lotesType[0];      // Casas
    int numIndustria = m_lotesType[1];  // Indústrias
    int numAtracoes = m_lotesType[2];   // Atrações
    int numComercios = m_lotesType[3];  // Comércios

    if (numAtracoes + numComercios > 0) {
        m_coefficient_lotes = (numCasas + numIndustria) / float(numAtracoes + numComercios);
    } else {
        m_coefficient_lotes = INT_MAX;  // Caso não haja atrações ou comércios, o coeficiente é inf
    }

    // Calcular o custo para construir o metrô
    m_cost = (rand() % 10) * m_length * 100;
}

// Funções para acessar os atributos
vertex EdgeNode::otherVertex() {
    return m_otherVertex;
}

string EdgeNode::bairro() {
    return m_bairro;
}

int EdgeNode::length() {
    return m_length;
}

int EdgeNode::cost() {
    return m_cost;
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

float EdgeNode::coefficient_lotes() {
    return m_coefficient_lotes;
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

// Implementação da classe GraphAdjList ------------------------------------------
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
                 << ", Transito: " << edge-> traffic_multiplier
                 << ", Comprimento: " << edge->length()
                 << ", Custo: " << edge->cost()
                 << ", Velocidade Máxima: " << edge->maxSpeed()
                 << ", Coeficiente: " << edge->coefficient_lotes() << ") ";
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