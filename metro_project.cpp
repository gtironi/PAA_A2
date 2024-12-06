#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <set>
#include <string>
#include <algorithm>
#include <chrono> // Incluído para medir o tempo de execução

using namespace std;
using namespace std::chrono;

const int INF = numeric_limits<int>::max();

// Estrutura para representar uma aresta
struct Edge {
    int from;             // Vértice de origem
    int to;               // Vértice de destino
    string neighborhood;  // Bairro
    int length;           // Comprimento do segmento (em metros)
    int maxSpeed;         // Velocidade máxima permitida (em km/h)
    bool oneway;          // Indica se é mão única
    int numLotes;         // Número de lotes
    int lotesType[4];     // Tipos de lotes: [casas, indústrias, atrações, comércios]

    Edge(int from, int to, const string& neighborhood, int length, int maxSpeed, bool oneway, int numLotes, const int lotesType[4]) 
        : from(from), to(to), neighborhood(neighborhood), length(length), maxSpeed(maxSpeed), oneway(oneway), numLotes(numLotes) {
        for (int i = 0; i < 4; ++i) {
            this->lotesType[i] = lotesType[i];
        }
    }
};

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        auto hash1 = std::hash<T1>{}(pair.first);
        auto hash2 = std::hash<T2>{}(pair.second);
        return hash1 ^ (hash2 << 1); // Combina os hashes
    }
};


// Estrutura para representar o grafo
class Graph {
private:
    unordered_map<int, vector<Edge>> adjList; // Lista de adjacências

public:
    // Adiciona uma aresta ao grafo
    void addEdge(int from, int to, const string& neighborhood, int length, int maxSpeed, bool oneway, int numLotes, const int lotesType[4]) {
        adjList[from].emplace_back(from, to, neighborhood, length, maxSpeed, oneway, numLotes, lotesType);

        if (!oneway) {
            adjList[to].emplace_back(to, from, neighborhood, length, maxSpeed, oneway, numLotes, lotesType);
        }
    }

    const vector<Edge>& getEdges(int vertex) const {
        static const vector<Edge> empty;
        auto it = adjList.find(vertex);
        return it != adjList.end() ? it->second : empty;
    }

    int numVertices() const {
        return adjList.size();
    }

    void print() const {
        cout << "Representação do Grafo:" << endl;
        for (const auto& [vertex, edges] : adjList) {
            cout << "Vértice " << vertex << ":\n";
            for (const auto& edge : edges) {
                cout << "  -> " << edge.to << " (Bairro: " << edge.neighborhood
                     << ", Comprimento: " << edge.length << "m"
                     << ", Velocidade Máxima: " << edge.maxSpeed << "km/h"
                     << ", Sentido Único: " << (edge.oneway ? "Sim" : "Não")
                     << ", Número de Lotes: " << edge.numLotes
                     << ", Tipos de Lotes: [" << edge.lotesType[0] << ", " << edge.lotesType[1]
                     << ", " << edge.lotesType[2] << ", " << edge.lotesType[3] << "])\n";
            }
        }
    }
};

// Função de Dijkstra que retorna as distâncias de um vértice inicial
vector<int> dijkstra(const Graph& graph, int start) {
    auto startTime = high_resolution_clock::now(); // Início da medição do tempo

    int n = graph.numVertices();
    vector<int> dist(n, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

    dist[start] = 0;
    pq.push({0, start});

    cout << "Iniciando Dijkstra a partir do vértice " << start << "...\n";

    while (!pq.empty()) {
        auto [currentDist, currentNode] = pq.top();
        pq.pop();

        if (currentDist > dist[currentNode]) continue;

        for (const Edge& edge : graph.getEdges(currentNode)) {
            int newDist = currentDist + edge.length;

            if (newDist < dist[edge.to]) {
                dist[edge.to] = newDist;
                pq.push({newDist, edge.to});
                cout << "  Atualizando distância para o nó " << edge.to << ": " << newDist << "m\n";
            }
        }
    }

    auto endTime = high_resolution_clock::now(); // Fim da medição do tempo
    auto duration = duration_cast<milliseconds>(endTime - startTime).count();
    cout << "Tempo de execução do Dijkstra para o vértice " << start << ": " << duration << "ms\n";

    return dist;
}


unordered_map<pair<int, int>, vector<int>, pair_hash> findShortestPaths(const Graph& graph, const vector<int>& metroStations) {
    auto startTime = high_resolution_clock::now(); // Início da medição do tempo

    unordered_map<pair<int, int>, vector<int>, pair_hash> paths;

    for (size_t i = 0; i < metroStations.size(); ++i) {
        for (size_t j = i + 1; j < metroStations.size(); ++j) {
            int start = metroStations[i];
            int end = metroStations[j];

            cout << "\nEncontrando caminho mais curto entre " << start << " e " << end << "...\n";

            vector<int> dist(graph.numVertices(), INF);
            vector<int> parent(graph.numVertices(), -1);
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

            dist[start] = 0;
            pq.push({0, start});

            while (!pq.empty()) {
                auto [currentDist, currentNode] = pq.top();
                pq.pop();

                if (currentDist > dist[currentNode]) continue;

                for (const Edge& edge : graph.getEdges(currentNode)) {
                    int newDist = currentDist + edge.length;
                    if (newDist < dist[edge.to]) {
                        dist[edge.to] = newDist;
                        parent[edge.to] = currentNode;
                        pq.push({newDist, edge.to});
                    }
                }
            }

            // Reconstruindo o caminho
            vector<int> path;
            int currentNode = end;
            while (currentNode != -1) {
                path.push_back(currentNode);
                currentNode = parent[currentNode];
            }
            reverse(path.begin(), path.end());

            if (!path.empty() && path.front() == start && path.back() == end) {
                paths[{start, end}] = path;
                cout << "  Caminho encontrado: ";
                for (int node : path) cout << node << " ";
                cout << "\n";
            }
        }
    }

    auto endTime = high_resolution_clock::now(); // Fim da medição do tempo
    auto duration = duration_cast<milliseconds>(endTime - startTime).count();
    cout << "Tempo de execução para encontrar todos os caminhos mais curtos: " << duration << "ms\n";

    return paths;
}



// Função para encontrar o vértice mais central de cada bairro
vector<int> findMetroStations(Graph& graph) {
    // Mapear os bairros e seus respectivos vértices
    unordered_map<string, set<int>> neighborhoods;
    for (int vertex = 0; vertex < graph.numVertices(); ++vertex) {
        for (const Edge& edge : graph.getEdges(vertex)) {
            neighborhoods[edge.neighborhood].insert(edge.from);
        }
    }

    vector<int> metroStations;

    // Iterar sobre cada bairro para encontrar o vértice mais central
    for (const auto& [neighborhood, vertices] : neighborhoods) {
        cout << "\nAnalisando bairro: " << neighborhood << endl;
        vector<int> vertexList(vertices.begin(), vertices.end());

        int minMaxDist = INF;
        int centralVertex = -1;

        // Calcular a matriz de distâncias no subgrafo do bairro
        for (int vertex : vertexList) {
            cout << "  Executando Dijkstra para o vértice " << vertex << "...\n";
            vector<int> distances = dijkstra(graph, vertex);

            // Considerar apenas as distâncias para vértices do mesmo bairro
            int maxDist = 0;
            for (int otherVertex : vertexList) {
                if (distances[otherVertex] < INF) {
                    maxDist = max(maxDist, distances[otherVertex]);
                }
            }

            cout << "  Distância máxima do vértice " << vertex << " para outros no bairro: " << maxDist << "m\n";

            // Atualizar o vértice mais central
            if (maxDist < minMaxDist) {
                minMaxDist = maxDist;
                centralVertex = vertex;
            }
        }

        cout << "Vértice mais central para o bairro " << neighborhood << ": " << centralVertex 
             << " com distância máxima " << minMaxDist << "m\n";

        // Adicionar o vértice como estação de metrô
        metroStations.push_back(centralVertex);
    }

    return metroStations;
}

Graph buildConnectedSubgraph(const Graph& graph, const unordered_map<pair<int, int>, vector<int>, pair_hash>& shortestPaths) {
    auto startTime = high_resolution_clock::now(); // Início da medição do tempo

    Graph subgraph;
    set<int> includedVertices; // Conjunto de vértices incluídos
    set<pair<int, int>> addedEdges; // Para evitar duplicação de arestas

    for (const auto& [pair, path] : shortestPaths) {
        for (size_t i = 1; i < path.size(); ++i) {
            int from = path[i - 1];
            int to = path[i];

            // Adicionar vértices ao conjunto
            includedVertices.insert(from);
            includedVertices.insert(to);

            // Evitar duplicação de arestas
            if (addedEdges.count({from, to}) || addedEdges.count({to, from})) {
                continue;
            }

            // Recuperar a aresta original do grafo
            for (const Edge& edge : graph.getEdges(from)) {
                if (edge.to == to) {
                    subgraph.addEdge(from, to, edge.neighborhood, edge.length, edge.maxSpeed, edge.oneway, edge.numLotes, edge.lotesType);
                    addedEdges.insert({from, to});
                    break;
                }
            }
        }
    }

    auto endTime = high_resolution_clock::now(); // Fim da medição do tempo
    auto duration = duration_cast<milliseconds>(endTime - startTime).count();
    cout << "Tempo de execução para construir o subgrafo conectado: " << duration << "ms\n";

    return subgraph;
}



Graph primMST(const Graph& graph) {
    auto startTime = high_resolution_clock::now(); // Início da medição do tempo
    Graph mst;
    set<int> included; // Conjunto de vértices já incluídos na MST
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<>> pq;

    if (graph.numVertices() == 0) {
        cout << "O subgrafo está vazio. MST não pode ser gerada.\n";
        return mst;
    }

    // Escolha do vértice inicial
    int start = -1;
    for (int i = 0; i < graph.numVertices(); ++i) {
        if (!graph.getEdges(i).empty()) {
            start = i;
            cout << "Vértice inicial escolhido para a MST: " << start << "\n";
            break;
        }
    }
    if (start == -1) {
        cout << "Não há vértices conectados no grafo.\n";
        return mst;
    }

    included.insert(start);

    // Adicione todas as arestas conectadas ao vértice inicial na fila de prioridade
    for (const Edge& edge : graph.getEdges(start)) {
        pq.push({edge.length, {start, edge.to}});
    }

    while (!pq.empty()) {
        auto [weight, vertices] = pq.top();
        pq.pop();
        int from = vertices.first, to = vertices.second;

        // Ignore arestas que levam a vértices já incluídos
        if (included.find(to) != included.end()) {
            continue;
        }

        if (included.find(to) == included.end()) {
            // Inclua o vértice 'to' na MST
            included.insert(to);

            // Adicione a aresta ao MST
            for (const Edge& edge : graph.getEdges(from)) {
                if (edge.to == to) {
                    mst.addEdge(from, to, edge.neighborhood, edge.length, edge.maxSpeed, edge.oneway, edge.numLotes, edge.lotesType);
                    break;
                }
            }

            // Adicione as arestas conectadas ao novo vértice à fila de prioridade
            for (const Edge& edge : graph.getEdges(to)) {
                if (included.find(edge.to) == included.end()) {
                    pq.push({edge.length, {to, edge.to}});
                }
            }
        }
    }

    cout << "\nÁrvore Geradora Mínima (MST) gerada:\n";
    mst.print();

    auto endTime = high_resolution_clock::now(); // Fim da medição do tempo
    auto duration = duration_cast<milliseconds>(endTime - startTime).count();
    cout << "Tempo de execução para gerar MST: " << duration << "ms\n";

    return mst;
}

void validateGraph(const Graph& graph) {
    cout << "\nValidando o grafo:\n";
    graph.print();

    for (int i = 0; i < graph.numVertices(); ++i) {
        if (graph.getEdges(i).empty()) {
            cout << "  Vertice " << i << " nao possui arestas conectadas.\n";
        }
    }
    cout << "Fim da validacao do grafo.\n";
}

int main() {
    // Criação do grafo
    Graph graph;

    // Definição dos tipos de lotes
    int lotesType[4] = {0, 0, 0, 0};

    // Adicionando arestas ao grafo
    graph.addEdge(0, 1, "Centro", 10, 50, false, 1, lotesType);
    graph.addEdge(1, 2, "Centro", 15, 50, false, 1, lotesType);
    graph.addEdge(2, 3, "Zona Sul", 12, 50, false, 1, lotesType);
    graph.addEdge(3, 4, "Zona Sul", 8, 50, false, 1, lotesType);
    graph.addEdge(3, 5, "Zona Sul", 20, 50, false, 1, lotesType);
    graph.addEdge(4, 5, "Zona Sul", 8, 50, false, 1, lotesType);
    graph.addEdge(5, 6, "Zona Sul", 22, 50, false, 1, lotesType);

    // Exibindo o grafo
    graph.print();

    // Encontrando as estações de metrô
    cout << "\nSelecionando estações de metrô...\n";
    vector<int> metroStations = findMetroStations(graph);

    // Encontrando caminhos mais curtos entre estações
    cout << "\nCalculando caminhos mais curtos entre estações...\n";
    auto shortestPaths = findShortestPaths(graph, metroStations);

    // Construir subgrafo conectado
    cout << "\nConstruindo subgrafo conectado...\n";
    Graph connectedSubgraph = buildConnectedSubgraph(graph, shortestPaths);
    cout << "\nSubgrafo conectado construído:\n";
    connectedSubgraph.print();

    // Encontrar a MST
    cout << "\nGerando MST do subgrafo conectado...\n";
    Graph mst = primMST(connectedSubgraph);

    validateGraph(mst);

    return 0;
}
