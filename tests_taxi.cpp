#include <iostream>
#include <cassert>

#include "graph.h"

// #include "graph.h"
#include "dijkstra.h"
#include "list.h"
// #include <iostream>
using namespace std;

int enderecoToDist(int);

// float calculaTempoTaxi(GraphAdjList& graph, int origem, int destino, int filho, int distRestante, int* parentTaxi){
//     float tempo;
//     int v_max;
//     int cost;
//     float modf_transito;
//     int vertice = destino;
//     cout << "inicio calculaTempoTaxi" << endl;
//     cout << vertice << "--" << parentTaxi[vertice] << endl;
//     cout << "origem: " << origem << " - destino: " << destino << endl;
//     cout << graph.getEdges(destino) << endl;
//     while (vertice != parentTaxi[vertice]){
//         cout << parentTaxi[vertice] << endl;
//         EdgeNode* edge = graph.getEdges(parentTaxi[vertice]);
//         while (edge) {
//             int v2 = edge->otherVertex();
//             cout << v2 << endl;
//             if (v2==vertice) {
//                 cost = edge->cost();
//                 v_max = edge->maxSpeed();
//                 modf_transito = 0.9; // usa a api de tansito para buscar o modificar da velocidade maxima da via
//                 tempo = tempo + cost/(v_max*modf_transito);
//                 break;
//             }
//         vertice = parentTaxi[vertice];
//         }
//     }

//     if (distRestante!=0) {
//         EdgeNode* edge = graph.getEdges(destino);
//         while (edge) {
//             int v2 = edge->otherVertex();
//             if (v2==filho) {
//                 v_max = edge->maxSpeed();
//                 modf_transito = 0.9; // usa a api de tansito para buscar o modificar da velocidade maxima da via
//                 break;
//             }
//         vertice = parentTaxi[vertice];
//         }
//         tempo = tempo + distRestante/(v_max*modf_transito);
//     }

//     cout << "fim calculaTempoTaxi" << endl;
//     return tempo;
// }

float calculaTempoTaxi(GraphAdjList& graph, int origem, int destino, int filho, int distRestante, int* parentTaxi) {
    float tempo = 0.0f;
    int v_max;
    int cost;
    float modf_transito = 0.9; // Modificador do trânsito (fixo por enquanto)
    int vertice = destino;

    cout << "Inicio calculaTempoTaxi" << endl;
    cout << "Origem: " << origem << ", Destino: " << destino << endl;

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


// float calculaTempoCaminhada(GraphAdjList& graph, int origem, int destino, int* parentCaminhada){
//     float tempo;

//     int vertice = destino;
//     while (vertice != parentCaminhada[vertice]){
//         EdgeNode* edge = graph.getEdges(parentCaminhada[vertice]);
//         while (edge) {
//             int v2 = edge->otherVertex();
//             if (v2==vertice) {
//                 int cost = edge->cost();
//                 int v_max = edge->maxSpeed();
//                 float modf_transito = 0.9; // usa a api de tansito para buscar o modificar da velocidade maxima da via
//                 tempo = tempo + cost/(v_max*modf_transito);
//                 break;
//             }
//         vertice = parentCaminhada[vertice];
//         }
//     }
//     return tempo;
// }
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

    // Inicializando vetores
    int distanceTaxi[graphDirecionado.numVertices()];
    int parentTaxi[graphDirecionado.numVertices()];
    // Dijkstra para achar o menor caminho de taxi até o endereço
    Dijkstra dijkstra;
    dijkstra.compute(graphDirecionado, origem, parentTaxi, distanceTaxi);
    
    int dist = 0;
    int vertice = destino;
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

    if (dist >=1){ // 1km é a distância mínima que da pra andar de taxi, se dist<1 quer dizer que o usuário não tem dinheiro para andar de taxi
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
        float tempoTaxi=0;
        if (vertice != origem){
            tempoTaxi  = calculaTempoTaxi(graphDirecionado, origem, vertice, finalRua, distRestante, parentTaxi);
            rota.append(1, tempoTaxi, &caminhoTaxi); // adicionamos o caminho do taxi na rota
        }
    }
    // Dijkstra do vértice final do caminho do taxi até o dentino final
    int parentCaminhada[graphCompleto.numVertices()];
    int distanceCaminhada[graphCompleto.numVertices()];
    dijkstra.compute(graphCompleto, graphCompleto.numVertices()-1, parentCaminhada, distanceCaminhada);

    vertice = destino;
    IntList caminhoCaminhada;
    // Loop para achar o caminho que será feito a pé
    while (vertice != parentCaminhada[vertice]){
        caminhoCaminhada.insert_front(vertice);
        vertice = parentCaminhada[vertice];
    }
    caminhoCaminhada.insert_front(vertice);

    float tempoCaminhada  = calculaTempoCaminhada(graphCompleto, graphCompleto.numVertices()-1, destino, parentCaminhada);
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
    // float tempoMetro = rotaMetro(graph, rota, custoMax);
    // float tempoOnibus = rotaOnibus(graph, rota, custoMax);
    // float tempoMetroOnibus = rotaMetroOnibus(graph, rota, custoMax);
    cout << tempoTaxi << endl;
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

    // Adiciona arestas
    graphTaxi.addEdge(0, 1, "Centro", stdSizeEdge, stdMaxSpeed, !oneWay);
    graphTaxi.addEdge(0, 2, "Centro", stdSizeEdge, stdMaxSpeed, !oneWay);
    graphTaxi.addEdge(1, 3, "Centro", stdSizeEdge, stdMaxSpeed, !oneWay);
    graphTaxi.addEdge(3, 4, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphTaxi.addEdge(3, 5, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphTaxi.addEdge(3, 7, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphTaxi.addEdge(4, 6, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphTaxi.addEdge(5, 7, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphTaxi.addEdge(6, 8, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphTaxi.addEdge(6, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphTaxi.addEdge(7, 8, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphTaxi.addEdge(7, 9, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphTaxi.addEdge(8, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphTaxi.addEdge(9, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphTaxi.addEdge(10, 12, "Centro", stdSizeEdge, stdMaxSpeed, !oneWay);
    graphTaxi.addEdge(11, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);

    // Grafo de metro pra teste
    GraphAdjList graphMetro(13);

    // Copiando aresta dos vértices 4 a 9
    graphMetro.addEdge(3, 4, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(3, 5, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(3, 7, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(4, 6, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(5, 7, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(6, 8, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(6, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(7, 8, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(7, 9, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(8, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(9, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);

    GraphAdjList* graphMetroBidirectional = graphMetro.createBidirectionalCopy();

    // cout << "Printando grafo de metro: " << endl;
    // graphMetroBidirectional->print();   

    melhorRota(graphTaxi, 0, 10, 10);

    return 0;
}