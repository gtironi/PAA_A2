#include "graph.h"
#include "dijkstra.h"
#include <iostream>
using namespace std;


void melhorRota(GraphAdjList& graph, int enderecoOrigem, int enderecoDestino, int custoMax) {
    // int** rota[2][];
    int** rota = new int*[2];

    // Para cada linha, alocar memória para as colunas
    for (int i = 0; i < 2; ++i) {
        rota[i] = new int[3];
    }


    GraphAdjList* graphCompleto = graph.createBidirectionalCopy();
    GraphAdjList* graphDirecionado = graph.clone();


    float tempoTaxi = rotaTaxi(*graphCompleto, *graphDirecionado, rota, origem, destino, custoMax);
    // float tempoMetro = rotaMetro(graph, rota, custoMax);
    // float tempoOnibus = rotaOnibus(graph, rota, custoMax);
    // float tempoMetroOnibus = rotaMetroOnibus(graph, rota, custoMax);
    cout << 
    return;
}

float rotaTaxi(GraphAdjList& graphCompleto, GraphAdjList& graphDirecionado, int** rota, int origem, int destino, int custoMax){
    // custo/taxa = distância maxima que consigo andar de taxi
    // dijkstra até eu chegar no vértice do destino
    // pego o parent e vejo até onde deu para chegar de taxi
    //      aqui tem aquele problema de andar até o meio da aresta
    // faço um dijkstra, agora considerando o grafo completo, saindo do vértice que deu pra chegar de taxi
    // pego os dois vetores parent e vejo o tempo que demorou para cada caminho
    // combino os dois vetores parent para achar a rota
    
    // Distância máxima que da para andar de taxi com o custo
    int distMax = (int)(custoMax/TAXA_TAXI);

    // Inicializando vetores
    int distanceTaxi[graphDirecionado.numVertices()];
    int parentTaxi[graphDirecionado.numVertices()];
    
    // Dijkstra para achar o menor caminho de taxi até o endereço
    dijkstra(graphDirecionado, origem, parentTaxi, distanceTaxi);


    int dist = 0;
    int vertice = destino;
    int filho;
    // Loop para achar o vértice mais longe que da pra ir de taxi
    while (vertice != parentTaxi[vertice]){
        dist = distanceTaxi[vertice];
        if (dist < distMax){ break;}
        filho = vertice;
        vertice = parentTaxi[vertice];
    }

    // Se conseguir chegar no destino só de taxi retona
    if (vertice == destino){
        float tempo = calculaTempoTaxi(graphDirecionado, origem, destino, 0, 0, parentTaxi);
        rota[0][0] = origem;
        rota[0][1] = destino;
        rota[0][2] = 1; // número refente ao taxi 
        return tempo;
    }
    int distRestante = distMax - dist;

    graphCompleto.splitEdge(vertice, filho, distRestante);

    float tempoTaxi  = calculaTempoTaxi(graphDirecionado, origem, vertice, filho, distRestante, parentTaxi);
    rota[0][0] = origem;
    rota[0][1] = graphCompleto.numVertices(); 
    rota[0][2] = 1; // número refente ao taxi 

    int parentCaminhada[graphCompleto.numVertices()];
    int distanceCaminhada[graphCompleto.numVertices()];
    dijkstra(graphCompleto, graphCompleto.numVertices(), parentCaminhada, distanceCaminhada);

    vertice = destino;
    while vertice != parentCaminhada[vertice]{
        vertice = parentCaminhada[vertice];
    }

    float tempoCaminhada  = calculaTempoCaminhada(graphCompleto, graphCompleto.numVertices(), destino, parentCaminhada);
    rota[1][0] = graphCompleto.numVertices();
    rota[1][1] = destino; 
    rota[1][2] = 1; // número refente ao taxi 

    return tempoTaxi + tempoCaminhada;
}

float calculaTempoTaxi(GraphAdjList& graph, int origem, int destino, int filho, int distRestante, int* parentTaxi){
    float tempo;

    int vertice = destino;
    while (vertice != parentTaxi[vertice]){
        EdgeNode* edge = graph.getEdges(parentTaxi[vertice]);
        while (edge) {
            int v2 = edge->otherVertex();
            if (v2==vertice) {
                int cost = edge->cost();
                int v_max = edge->maxSpeed();
                float modf_transito = get_transito(); // usa a api de tansito para buscar o modificar da velocidade maxima da via
                tempo = tempo + cost/(v_max*modf_transito);
                break;
            }
        vertice = parentTaxi[vertice];
        }
    }

    if (distRestante!=0) {
        EdgeNode* edge = graph.getEdges(destino);
        while (edge) {
            int v2 = edge->otherVertex();
            if (v2==filho) {
                v_max = edge->maxSpeed();
                modf_transito = get_transito(); // usa a api de tansito para buscar o modificar da velocidade maxima da via
                break;
            }
        vertice = parentTaxi[vertice];
        }
        tempo = tempo + distRestante/(v_max*modf_transito);
    }

    return tempo;
}


float calculaTempoCaminhada(GraphAdjList& graph, int origem, int destino, int* parentCaminhada){
    float tempo;

    int vertice = destino;
    while (vertice != parentCaminhada[vertice]){
        EdgeNode* edge = graph.getEdges(parentCaminhada[vertice]);
        while (edge) {
            int v2 = edge->otherVertex();
            if (v2==vertice) {
                int cost = edge->cost();
                int v_max = edge->maxSpeed();
                float modf_transito = get_transito(); // usa a api de tansito para buscar o modificar da velocidade maxima da via
                tempo = tempo + cost/(v_max*modf_transito);
                break;
            }
        vertice = parentTaxi[vertice];
        }
    }
    return tempo
}


