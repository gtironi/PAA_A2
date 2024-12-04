#include "metro.h"

// ======================================================================
// Funções de caminho

// Recebe vértices de origem, destino, vetor de país gerado por uma função
// de caminho e o comprimento do caminho e retorna caminho no formato de 
// lista
IntList* pathVertices(int start, int end, int* parent) {
    // Lista para armazenar o caminho
    IntList * path = new IntList;

    // Começamos no destinho
    vertex currentVertex = end;

    // E vamos voltando
    while (currentVertex != start)
    {
        cout << "vertice atual:  " << currentVertex << endl;
        path->insert_front(currentVertex);
        currentVertex = parent[currentVertex];
    }

    // Primeira posição do caminho é a origem
    cout << "último vértice: ";
    cout << "vertice atual:  " << currentVertex << endl;
    path->insert_front(currentVertex);

    return path;
}


// ======================================================================
// Funções auxiliares
void printVector(const vector<int>& vector){
    for (int i = 0; i < vector.size(); i++){
        cout << vector[i] << " ";
    }
}

// ======================================================================
// Funções de metro


// Recebe grafo do metrô, vértices de origem e destino e retorna caminho no 
// formato de lista
IntList* createPathMetro(GraphAdjList& graphMetro, int start, int end){
    // Inicializando vetores de pais e distâncias
    int parent[graphMetro.numVertices()];
    int distance[graphMetro.numVertices()];
    // Rodando dijkstra para encontrar os caminhos que podem ser feitos
    // pelo metro.
    Dijkstra::compute(graphMetro, start, parent, distance);

    // Retornando caminho a partir do parent retornado pelo dijkstra
    return pathVertices(start, end, parent);
}

// Recebe o grafo do metro e o caminho que vai ser percorrido e retorna 
// o tempo total gasto nesse caminho
float timeMetro(GraphAdjList& graphMetro, IntList& path, const int SPEED_METRO){
    
    float tempoTotal = 0;

    // Percorrendo caminho
    IntNode* currentVertex = path.head;

    while (currentVertex != nullptr){
        vertex v_1 = currentVertex->data;

        // Pegando arestas do vértice v_1
        EdgeNode * edge = graphMetro.getEdges(v_1);

        while (edge){
            vertex v_2 = edge->otherVertex();

            // Se não tivermos encontrado a aresta que queremos do caminho
            if (currentVertex->next && v_2 == currentVertex->next->data){
                // Tamanho da aresta
                int custo = edge->cost();
                // Calculando tempo dividino pela velocidade do metro
                tempoTotal += custo;
            }

            edge = edge->next();
        }

        currentVertex = currentVertex->next;
    }
    
    return tempoTotal/SPEED_METRO;
}