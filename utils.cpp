#include "graph.h";
#include "utils.h";

// Recebe vértices de origem, destino, vetor de país gerado por uma função
// de caminho e o comprimento do caminho e retorna caminho no formato de 
// lista
IntList* pathVertices(int start, int end, int* parent) {
    // Lista para armazenar o caminho
    IntList * path = new IntList;

    // Começamos no destinho
    vertex currentVertex = end;

    // E vamos voltando
    while (currentVertex != start){
        path->insert_front(currentVertex);
        currentVertex = parent[currentVertex];
    }

    // Primeira posição do caminho é a origem
    path->insert_front(currentVertex);

    return path;
}