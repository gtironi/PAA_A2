void melhorRota(GraphAdjList& graph, int origem, int destino, int custoMax) {
    int parentPath[graph.m_numVertices];
    for (int v=0; v < graph.m_numVertices; v++) {
        parentPath[v] = -1;
    }



    tempoTaxi = rotaTaxi(graph, parentPath, origem, destino, custoMax)
    tempoMetro = rotaMetro(graph, parentPath, custoMax)
    tempoOnibus = rotaOnibus(graph, parentPath, custoMax)
    tempoMetroOnibus = rotaMetroOnibus(graph, parentPath, custoMax)

    return;
}

float rotaTaxi(GraphAdjList& graphCompleto, GraphAdjList& graphDirecionado, int** path, int origem, int destino, int custoMax){
    // custo/taxa = distância maxima que consigo andar de taxi
    // dijkstra até eu chegar no vértice do destino
    // pego o parent e vejo até onde deu para chegar de taxi
    //      aqui tem aquele problema de andar até o meio da aresta
    // faço um dijkstra, agora considerando o grafo completo, saindo do vértice que deu pra chegar de taxi
    // pego os dois vetores parent e vejo o tempo que demorou para cada caminho
    // combino os dois vetores parent para achar o path
    int distMax = (int)(custoMax/TAXA);
    int distanceTaxi[graphDirecionado.numVertices()];
    int parentTaxi[graphDirecionado.numVertices()];

    dijkstra(graphDirecionado, origem, parentTaxi, distanceTaxi);
    int dist = 0;
    int vertice = destino;
    int filho = 0; 
    while vertice != parentTaxi[vertice]{
        dist = distanceTaxi[vertice];
        if dist < distMax: break;
        filho = vertice;
        vertice = parentTaxi[vertice];
    }

    if vertice == destino:
        float tempo = calculaTempoTaxi(graphDirecionado, origem, destino, 0, 0, parentTaxi);
        path[0][0] = origem;
        path[0][1] = destino;
        path[0][2] = 1; // número refente ao taxi 
        return tempo;

    int distRestante = distMax - dist; 
    graphCompleto.splitEdge(vertice, filho, distRestante);

    float tempoTaxi  = calculaTempoTaxi(graphDirecionado, origem, vertice, filho, distRestante, parentTaxi);
    path[0][0] = graphCompleto.endereco(origem) ;
    path[0][1] = graphCompleto.endereco(graphCompleto.numVertices()); 
    path[0][2] = 1; // número refente ao taxi 

    int parentCaminhada[graphCompleto.numVertices()];
    int distanceCaminhada[graphCompleto.numVertices()];
    dijkstra(graphCompleto, graphCompleto.numVertices(), parentCaminhada, distanceCaminhada);

    vertice = destino;
    while vertice != parentCaminhada[vertice]{
        vertice = parentCaminhada[vertice];
    }

    float tempoCaminhada  = calculaTempoCaminhada(graphCompleto, graphCompleto.numVertices(), destino, parentCaminhada);
    path[1][0] = graphCompleto.endereco(graphCompleto.numVertices()) ;
    path[1][1] = graphCompleto.endereco(destino); 
    path[1][2] = 1; // número refente ao taxi 

    return tempoTaxi + tempoCaminhada;
}

float calculaTempoTaxi(GraphAdjList& graph, int origem, int destino, int filho, int distRestante, int* parentTaxi){
    float tempo;

    int vertice = destino;
    while vertice != parentTaxi[vertice]{
        EdgeNode* edge = graph.getEdges(parentTaxi[vertice]);
        while (edge) {
            int v2 = edge->otherVertex();
            if (v2==vertice) {
                int cost = edge->cost();
                int v_max = edge->maxSpeed();
                float modf_transito = get_transito(); // usa a api de tansito para buscar o modificar da velocidade maxima da via
                tempo = tempo + cost/(v_max*modf_transito);
            }
        vertice = parentTaxi[vertice];
        }
    }

    if ta

    return tempo;
}
    


