#include "taxi.h"

float calculaTempoTaxi(GraphAdjList& graph, int origem, int destino, int filho, int distRestante, int* parentTaxi) {
    float tempo = 0.0f;
    int v_max;
    int cost;
    float modf_transito = 0.9; // Modificador do trânsito (fixo por enquanto)
    int vertice = destino;

    while (vertice != parentTaxi[vertice]) {

        // Busca a aresta conectando parentTaxi[vertice] a vertice
        EdgeNode* edge = graph.getEdges(parentTaxi[vertice]);
        bool encontrou = false;

        while (edge) {
            int v2 = edge->otherVertex();
            if (v2 == vertice) {
                cost = edge->cost();
                v_max = edge->maxSpeed();
                tempo += cost / (v_max * modf_transito);
                encontrou = true;
                break;
            }
            edge = edge->next(); // Próxima aresta
        }

        if (!encontrou) {
            cerr << "Erro: Não foi encontrada uma aresta entre " << parentTaxi[vertice] 
                 << " e " << vertice << endl;
            break;
        }

        // Atualiza o vértice para o pai
        vertice = parentTaxi[vertice];
    }

    // Caso haja uma distância restante a ser percorrida
    if (distRestante != 0) {
        EdgeNode* edge = graph.getEdges(destino);
        while (edge) {
            int v2 = edge->otherVertex();
            if (v2 == filho) {
                v_max = edge->maxSpeed();
                tempo += distRestante / (v_max * modf_transito);
                break;
            }
            edge = edge->next();
        }
    }

    return tempo;
}

float calculaTempoCaminhada(GraphAdjList& graph, int origem, int destino, int* parentCaminhada) {
    float tempo = 0.0f;
    int vertice = destino;

    // Percorrendo o caminho de destino até a origem utilizando parentCaminhada
    while (vertice != parentCaminhada[vertice]) {
        EdgeNode* edge = graph.getEdges(parentCaminhada[vertice]);
        bool encontrou = false;

        // Procura pela aresta que conecta parentCaminhada[vertice] a vertice
        while (edge) {
            int v2 = edge->otherVertex();
            if (v2 == vertice) {
                int cost = edge->cost();
                int v_max = edge->maxSpeed();
                float modf_transito = 0.9; // Modificador de trânsito (fixo por enquanto)
                tempo += cost / (v_max * modf_transito);
                encontrou = true;
                break;
            }
            edge = edge->next(); // Próxima aresta
        }

        if (!encontrou) {
            cerr << "Erro: Não foi encontrada uma aresta entre " << parentCaminhada[vertice] 
                 << " e " << vertice << endl;
            break;
        }

        // Atualiza o vértice para o pai
        vertice = parentCaminhada[vertice];
    }

    return tempo;
}

float rotaTaxi(GraphAdjList& graphCompleto, GraphAdjList& graphDirecionado, NodeList& rota, int origem, int destino, int custoMax){
    // custo/taxa = distância maxima que consigo andar de taxi
    // dijkstra até eu chegar no vértice do destino
    // pego o parent e vejo até onde deu para chegar de taxi
    //      aqui tem aquele problema de andar até o meio da aresta
    // faço um dijkstra, agora considerando o grafo completo, saindo do vértice que deu pra chegar de taxi
    // pego os dois vetores parent e vejo o tempo que demorou para cada caminho
    // combino os dois vetores parent para achar a rota
    
    // Distância máxima que da para andar de taxi com o custo
    int distMax = (int)(custoMax/10);//taxa taxi
    int vertice = destino;
    Dijkstra dijkstra;
    float tempoTaxi =0;
    if (distMax >=1){ // se dist_max<1 quer dizer que o usuário não tem dinheiro para andar de taxi, pois o valor mínimo de uma corrida é igual a TAXA
        // Inicializando vetores
        int distanceTaxi[graphDirecionado.numVertices()];
        int parentTaxi[graphDirecionado.numVertices()];
        // Dijkstra para achar o menor caminho de taxi até o endereço
        dijkstra.compute(graphDirecionado, origem, parentTaxi, distanceTaxi);
        
        int dist = 0;
        int finalRua;
        IntList caminhoTaxi;
        // Loop para achar o vértice mais longe que da pra ir de taxi
        while (vertice != parentTaxi[vertice]){
            caminhoTaxi.insert_front(vertice);
            dist = distanceTaxi[vertice];
            if (dist < distMax){ break;} // no momento que acha um vértice que da para chegar de taxi quebra
            finalRua = vertice;
            vertice = parentTaxi[vertice];
        }
        caminhoTaxi.insert_front(vertice);

        
        
        // Se conseguir chegar no destino só de taxi acaba
        if (vertice == destino){
            float tempo = calculaTempoTaxi(graphDirecionado, origem, destino, 0, 0, parentTaxi);
            rota.append(1, tempo, &caminhoTaxi);
            return tempo;
        }

        // Calcula distancia restante que da para andar na dentro da rua seguinte ao último vértice que deu pra chegar de taxi
        // Vamos andar essa distância de taxi
        int distRestante = distMax - dist;
        // Cria vértice no lugar em que ele parou de taxi na rua, dessa forma podemos fazer dijkstra a partir dele
        graphCompleto.splitEdge(vertice, finalRua, distRestante);

        
        // Calcula tempo da viagem de taxi
        if (vertice != origem){
            tempoTaxi  = calculaTempoTaxi(graphDirecionado, origem, vertice, finalRua, distRestante, parentTaxi);
            rota.append(1, tempoTaxi, &caminhoTaxi); // adicionamos o caminho do taxi na rota
        }
    }
    // Dijkstra do vértice final do caminho do taxi até o dentino final
    int parentCaminhada[graphCompleto.numVertices()];
    int distanceCaminhada[graphCompleto.numVertices()];
    dijkstra.compute(graphCompleto, graphCompleto.numVertices()-1, parentCaminhada, distanceCaminhada);

    // vertice = destino;
    cout << "Calculando caminho" << endl;
    IntList* caminhoCaminhada = pathVertices(origem, destino, parentCaminhada);

    // Loop para achar o caminho que será feito a pé
    // while (vertice != parentCaminhada[vertice]){
    //     caminhoCaminhada.insert_front(vertice);
    //     vertice = parentCaminhada[vertice];
    // }
    // caminhoCaminhada.insert_front(vertice);

    float tempoCaminhada  = calculaTempoCaminhada(graphCompleto, graphCompleto.numVertices()-1, destino, parentCaminhada);
    rota.append(0, tempoCaminhada, caminhoCaminhada);

    return tempoTaxi + tempoCaminhada;
}

void melhorRota(GraphAdjList& graph, int origem, int destino, int custoMax) {
    NodeList rota;

    GraphAdjList* graphCompleto = graph.createBidirectionalCopy();
    GraphAdjList* graphDirecionado = graph.clone();

    // int distOrigem = enderecoToDist(enderecoOrigem);
    // int distDestino = enderecoToDist(enderecoDestino);
    
    // graphCompleto->splitEdge(enderecoOrigem.vi, enderecoOrigem.vj, distOrigem);
    // graphCompleto->splitEdge(enderecoDestino.vi, enderecoDestino.vj, distDestino);
    // graphDirecionado->splitEdge(enderecoOrigem.vi, enderecoOrigem.vj, distOrigem);
    // graphDirecionado->splitEdge(enderecoDestino.vi, enderecoDestino.vj, distDestino);

    // int origem = (graphCompleto->numVertices())-2;
    // int destino = (graphCompleto->numVertices())-1;

    float tempoTaxi = rotaTaxi(*graphCompleto, *graphDirecionado, rota, origem, destino, custoMax);
    // float tempoMetro = rotaMetro(graph, rota, custoMax);
    // float tempoOnibus = rotaOnibus(graph, rota, custoMax);
    // float tempoMetroOnibus = rotaMetroOnibus(graph, rota, custoMax);
    cout << tempoTaxi << endl;
    cout << "===================================" << endl;
    return;
}

