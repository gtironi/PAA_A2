#include <iostream>
#include <cassert>

#include "graph.h"

// #include "graph.h"
#include "dijkstra.h"
#include "list.h"
// #include <iostream>
using namespace std;

float calculaTempoTaxi(GraphAdjList& graph, int origem, int destino, int* parentTaxi) {
    float tempo = 0.0f;
    int v_max;
    int cost;
    float modf_transito;
    int vertice = destino;

    while (vertice != parentTaxi[vertice]) {
        // Busca a aresta conectando parentTaxi[vertice] a vertice
        EdgeNode* edge = graph.getEdges(parentTaxi[vertice]);
        bool encontrou = false;

        while (edge) {
            int v2 = edge->otherVertex();
            if (v2 == vertice) {
                cost = edge->length();
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
                int cost = edge->length();
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

float rotaTaxi(GraphAdjList& graphCompleto, GraphAdjList& graphDirecionado, NodeList& rota, int origem, int destino, int custoMax){    
    // Distância máxima que da para andar de taxi com o custo
    int distMax = (int)(custoMax/4);//taxa taxi
    int vertice = destino;
    int parada = origem;
    Dijkstra dijkstra;
    float tempoTaxi =0;

    if (distMax >= 9/4 ){ // se dist_max<9/4 quer dizer que o usuário não tem dinheiro para andar de taxi, pois o valor mínimo de uma corrida é igual a 9
        // Inicializando vetores
        int distanceTaxi[graphDirecionado.numVertices()];
        int parentTaxi[graphDirecionado.numVertices()];
        // Dijkstra para achar o menor caminho de taxi até o endereço
        dijkstra.compute(graphDirecionado, origem, parentTaxi, distanceTaxi);
        
        int dist = 0;
        int finalRua;
        bool insertOnList = false;
        IntList caminhoTaxi;
        // Loop para achar o vértice mais longe que da pra ir de taxi
        while (vertice != parentTaxi[vertice]){
            if (!insertOnList){
                dist = distanceTaxi[vertice];
                if (dist < distMax){
                    caminhoTaxi.insert_front(vertice);
                    parada = vertice;
                    insertOnList = true;
                } // no momento que acha um vértice que da para chegar de taxi quebra
            } else{
                caminhoTaxi.insert_front(vertice);
            }
            vertice = parentTaxi[vertice];
        }
        caminhoTaxi.insert_front(vertice);

        
        // Se conseguir chegar no destino só de taxi acaba
        if (parada == destino){
            float tempo = calculaTempoTaxi(graphDirecionado, origem, destino, parentTaxi);
            rota.append(1, tempo, &caminhoTaxi);
            return tempo;
        }
        
        // Calcula tempo da viagem de taxi
        if (insertOnList){
            tempoTaxi  = calculaTempoTaxi(graphDirecionado, origem, parada, parentTaxi);
            rota.append(1, tempoTaxi, &caminhoTaxi); // adicionamos o caminho do taxi na rota
        }
    }
    // Dijkstra do vértice final do caminho do taxi até o dentino final
    int parentCaminhada[graphCompleto.numVertices()];
    int distanceCaminhada[graphCompleto.numVertices()];
    dijkstra.compute(graphCompleto, parada, parentCaminhada, distanceCaminhada);

    vertice = destino;
    IntList caminhoCaminhada;
    // Loop para achar o caminho que será feito a pé
    while (vertice != parentCaminhada[vertice]){
        caminhoCaminhada.insert_front(vertice);
        vertice = parentCaminhada[vertice];
    }
    caminhoCaminhada.insert_front(vertice);

    float tempoCaminhada  = calculaTempoCaminhada(graphCompleto, parada, destino, parentCaminhada);
    rota.append(0, tempoCaminhada, &caminhoCaminhada);

    return tempoTaxi + tempoCaminhada;
}

void melhorRota(GraphAdjList& graph, int origem, int destino, int custoMax) {
    NodeList rota;
    GraphAdjList* graphCompleto = graph.createBidirectionalCopy();
    GraphAdjList* graphDirecionado = graph.clone();
    
    cout << "COMEÇOU" << endl;
    cout << "===================================" << endl;
    // int distOrigem = enderecoToDist(enderecoOrigem);
    // int distDestino = enderecoToDist(enderecoDestino);
    
    // graphCompleto->splitEdge(enderecoOrigem.vi, enderecoOrigem.vj, distOrigem);
    // graphCompleto->splitEdge(enderecoDestino.vi, enderecoDestino.vj, distDestino);
    // graphDirecionado->splitEdge(enderecoOrigem.vi, enderecoOrigem.vj, distOrigem);
    // graphDirecionado->splitEdge(enderecoDestino.vi, enderecoDestino.vj, distDestino);

    // int origem = (graphCompleto->numVertices())-2;
    // int destino = (graphCompleto->numVertices())-1;

    float tempoTaxi = rotaTaxi(*graphCompleto, *graphDirecionado, rota, origem, destino, custoMax);
    rota.print();
    // float tempoMetro = rotaMetro(graph, rota, custoMax);
    // float tempoOnibus = rotaOnibus(graph, rota, custoMax);
    // float tempoMetroOnibus = rotaMetroOnibus(graph, rota, custoMax);
    cout << "===================================" << endl;
    cout << "FIM" << endl;
    return;
}

int main() {
    // Grafo de táxi pra teste
    GraphAdjList graphTaxi(13);

    int stdSizeEdge = 100;
    int stdMaxSpeed = 60;
    bool oneWay = true;
    int lotesType[4] = {1, 0, 0, 0};
    int numLotes = 100;

    // Adiciona arestas
    graphTaxi.addEdge(0, 1, "Centro", stdSizeEdge, stdMaxSpeed, !oneWay, numLotes, lotesType);
    graphTaxi.addEdge(0, 2, "Centro", stdSizeEdge, stdMaxSpeed, !oneWay, numLotes, lotesType);
    graphTaxi.addEdge(1, 3, "Centro", stdSizeEdge, stdMaxSpeed, !oneWay, numLotes, lotesType);
    graphTaxi.addEdge(3, 4, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graphTaxi.addEdge(3, 5, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graphTaxi.addEdge(3, 7, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graphTaxi.addEdge(4, 6, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graphTaxi.addEdge(5, 7, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graphTaxi.addEdge(6, 8, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graphTaxi.addEdge(6, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graphTaxi.addEdge(7, 8, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graphTaxi.addEdge(7, 9, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graphTaxi.addEdge(8, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graphTaxi.addEdge(9, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graphTaxi.addEdge(10, 12, "Centro", stdSizeEdge, stdMaxSpeed, !oneWay, numLotes, lotesType);
    graphTaxi.addEdge(11, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);

    // Grafo de metro pra teste
    GraphAdjList graphMetro(13);

    // Copiando aresta dos vértices 4 a 9
    graphMetro.addEdge(3, 4, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graphMetro.addEdge(3, 5, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graphMetro.addEdge(3, 7, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graphMetro.addEdge(4, 6, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graphMetro.addEdge(5, 7, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graphMetro.addEdge(6, 8, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graphMetro.addEdge(6, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graphMetro.addEdge(7, 8, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graphMetro.addEdge(7, 9, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graphMetro.addEdge(8, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);
    graphMetro.addEdge(9, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, numLotes, lotesType);

    GraphAdjList* graphMetroBidirectional = graphMetro.createBidirectionalCopy();

    // cout << "Printando grafo de metro: " << endl;
    // graphMetroBidirectional->print();   
    melhorRota(graphTaxi, 0, 3, 100000);

    return 0;
}