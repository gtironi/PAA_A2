#include "taxi.h"

float calculaTempoTaxi(GraphAdjList& graph, int origem, int destino, int* parentTaxi) {
    float tempo = 0.0f;
    int v_max;
    float cost;
    float modf_transito;
    int vertice = destino;

    while (vertice != parentTaxi[vertice]) {
        // Busca a aresta conectando parentTaxi[vertice] a vertice
        EdgeNode* edge = graph.getEdges(parentTaxi[vertice]);
        bool encontrou = false;

        while (edge) {
            int v2 = edge->otherVertex();
            if (v2 == vertice) {
                cost = (float)(edge->length())/1000;
                v_max = edge->maxSpeed();
                modf_transito = edge->traffic_multiplier;
                tempo += cost / (v_max * modf_transito);
                encontrou = true;
                break;
            }
            edge = edge->next();
        }

        if (!encontrou) {
            cerr << "Erro: Não foi encontrada uma aresta entre " << parentTaxi[vertice] 
                 << " e " << vertice << endl;
            break;
        }

        // Atualiza o vértice para o pai
        vertice = parentTaxi[vertice];
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
                float cost = (float)(edge->length())/1000;
                tempo += cost / 5;
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

float rotaTaxi(GraphAdjList& graphCompleto, GraphAdjList& graphDirecionado, NodeList& rota, int origem, int destino, int custoMax, float& dinheiroRestante) {    
    // Distância máxima que da para andar de taxi com o custo
    float distMax = (float)((custoMax) / TAXA_TAXI);  // taxa taxi
    int vertice = destino;
    int parada = origem;
    Dijkstra dijkstra;
    float tempoTaxi = 0;
    dinheiroRestante = 0;
    
    // Se dist_max < CUSTO_MIN_TAXI/TAXA_TAXI quer dizer que o usuário não tem dinheiro para andar de taxi
    if (distMax >= CUSTO_MIN_TAXI / TAXA_TAXI) {
        // Inicializando vetores
        int distanceTaxi[graphDirecionado.numVertices()];
        int parentTaxi[graphDirecionado.numVertices()];
        
        // Dijkstra para achar o menor caminho de taxi até o endereço
        dijkstra.compute(graphDirecionado, origem, parentTaxi, distanceTaxi);
        
        float dist = 0;
        bool insertOnList = false;
        IntList caminhoTaxi;
        
        // Verifica se o Dijkstra encontrou um caminho
        if (parentTaxi[destino] == -1) {
            std::cerr << "Erro: Dijkstra não encontrou caminho até o destino." << std::endl;
            return -1;
        }

        // Loop para achar o vértice mais longe que da pra ir de taxi
        while (vertice != parentTaxi[vertice]) {
            if (!insertOnList) {
                dist = (distanceTaxi[vertice])/1000;
                if (dist < distMax) {
                    caminhoTaxi.insert_front(vertice);
                    parada = vertice;
                    insertOnList = true;
                }
            } else {
                caminhoTaxi.insert_front(vertice);
            }
            vertice = parentTaxi[vertice];

            // Verifica se vertice não está fora do limite
            if (vertice < 0 || vertice >= graphDirecionado.numVertices()) {
                std::cerr << "Erro: vertice fora dos limites ao acessar parentTaxi." << std::endl;
                return -1;
            }
        }
        
        caminhoTaxi.insert_front(vertice);
        dinheiroRestante = (distMax - (distanceTaxi[parada]/1000)) * TAXA_TAXI;
        
        // Se conseguir chegar no destino só de taxi, acaba
        if (parada == destino) {
            float tempo = calculaTempoTaxi(graphDirecionado, origem, destino, parentTaxi);
            rota.append(TAXI_CODE, tempo, &caminhoTaxi);
            return tempo;
        }
        
        // Calcula tempo da viagem de taxi
        if (insertOnList) {
            tempoTaxi = calculaTempoTaxi(graphDirecionado, origem, parada, parentTaxi);
            rota.append(TAXI_CODE, tempoTaxi, &caminhoTaxi); // adicionamos o caminho do taxi na rota
        }
    }

    // Dijkstra do vértice final do caminho do taxi até o destino final
    int parentCaminhada[graphCompleto.numVertices()];
    int distanceCaminhada[graphCompleto.numVertices()];
    dijkstra.compute(graphCompleto, parada, parentCaminhada, distanceCaminhada);

    vertice = destino;
    IntList caminhoCaminhada;

    // Verifica se o Dijkstra encontrou um caminho
    if (parentCaminhada[destino] == -1) {
        std::cerr << "Erro: Dijkstra não encontrou caminho até o destino a pé." << std::endl;
        return -1;
    }

    // Loop para achar o caminho que será feito a pé
    while (vertice != parentCaminhada[vertice]) {
        caminhoCaminhada.insert_front(vertice);
        vertice = parentCaminhada[vertice];

        // Verifica se vertice não está fora do limite
        if (vertice < 0 || vertice >= graphCompleto.numVertices()) {
            std::cerr << "Erro: vertice fora dos limites ao acessar parentCaminhada." << std::endl;
            return -1;
        }
    }
    
    caminhoCaminhada.insert_front(vertice);

    float tempoCaminhada = calculaTempoCaminhada(graphCompleto, parada, destino, parentCaminhada);
    rota.append(A_PE_CODE, tempoCaminhada, &caminhoCaminhada);

    return tempoTaxi + tempoCaminhada;
}


