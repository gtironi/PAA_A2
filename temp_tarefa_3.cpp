void melhorRota(GraphAdjList& graph, int origem, int destino, int custoMax) {
    int parentPath[graph.m_numVertices];
    for (vertex v=0; v < graph.m_numVertices; v++) {
        parentPath[v] = -1;
    } 

    tempoTaxi = rotaTaxi(graph, parentPath, custoMax)
    tempoMetro = rotaMetro(graph, parentPath, custoMax)
    tempoOnibus = rotaOnibus(graph, parentPath, custoMax)
    tempoMetroOnibus = rotaMetroOnibus(graph, parentPath, custoMax)

}

