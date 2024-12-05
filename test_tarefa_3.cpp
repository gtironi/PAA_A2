#include <iostream>
#include <cassert>
#include <vector>

#include "graph.h"
#include "metro.h"
#include "taxi.h"

void testMetro(GraphAdjList& graphStreets, GraphAdjList& graphMetroBidirectional) {
    //======================================================================
    // Testando funções de metrô

    // Resultados esperados
    IntList expectedPathMetro;
    expectedPathMetro.insert_front(10);
    expectedPathMetro.insert_front(8);
    expectedPathMetro.insert_front(6);
    expectedPathMetro.insert_front(4);
    expectedPathMetro.insert_front(3);
    
    // Criação do caminho no formato de lista
    IntList* pathMetro = createPathMetro(graphMetroBidirectional, 3, 10);

    // Começo das duas listas
    IntNode* currentVertex = pathMetro->head;
    IntNode* expectedCurrentVertex = expectedPathMetro.head;


    // Verificando resultados
    while (currentVertex != nullptr && expectedCurrentVertex != nullptr){
        assert(currentVertex->data == expectedCurrentVertex->data);
        expectedCurrentVertex = expectedCurrentVertex->next;
        currentVertex = currentVertex->next;
    }
    
    cout << "Teste do caminho passou!" << endl;

    // Teste da função de tempo do metrô.
    // Velocidade do metro
    int metroSpeed = 70;
    // Valor esperado para o tempo
    float expectedValueTime = float(40.0/7.0);

    // Calculando tempo gasto no caminho
    float timeTakenMetro = timeMetro(graphMetroBidirectional, *pathMetro, metroSpeed);

    // Verificando resultado
    assert(timeTakenMetro == expectedValueTime);

    cout << "Teste do tempo passou!" << endl;

    // Vértices esperado no grafo de metrô
    vertex expectedVertexInGraph1 = 7;
    vertex expectedVertexInGraph2 = 10;
    vertex expectedVertexInGraph3 = 3;
    vertex expectedVertexNotInGraph = 0;

    // Verificando resultados;
    assert(vertexInGraphMetro(expectedVertexInGraph1, graphMetroBidirectional));
    assert(vertexInGraphMetro(expectedVertexInGraph2, graphMetroBidirectional));
    assert(vertexInGraphMetro(expectedVertexInGraph3, graphMetroBidirectional));
    assert(!vertexInGraphMetro(expectedVertexNotInGraph, graphMetroBidirectional));

    cout << "Teste do pertencimento passou!" << endl;

    // Testes da função de estação mais próxima.
    // Primeiro teste: estação inicial
    // Vértice esperado
    vertex expectedStation = 3;

    // Rodando função
    vertex closestStation = closestMetroStation(1, graphStreets, graphMetroBidirectional);

    // Verificando resultado
    assert(expectedStation == closestStation);

    // Segundo teste: estação final
    // Vértice esperado
    expectedStation = 10;

    // Rodando função
    closestStation = closestMetroStation(11, graphStreets, graphMetroBidirectional);

    // Verificando resultado
    assert(expectedStation == closestStation);

    cout << "Teste da estação mais próxima passou!" << endl;
}

int main() {
    int stdSizeEdge = 100;
    int stdMaxSpeed = 60;
    bool oneWay = true;
    
    // Grafo de táxi pra teste
    GraphAdjList graphStreets(13);
    // Adiciona arestas
    graphStreets.addEdge(0, 1, "Centro", stdSizeEdge, stdMaxSpeed, !oneWay);
    graphStreets.addEdge(0, 2, "Centro", stdSizeEdge, stdMaxSpeed, !oneWay);
    graphStreets.addEdge(1, 3, "Centro", stdSizeEdge, stdMaxSpeed, !oneWay);
    graphStreets.addEdge(3, 4, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphStreets.addEdge(3, 5, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphStreets.addEdge(3, 7, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphStreets.addEdge(4, 6, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphStreets.addEdge(5, 7, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphStreets.addEdge(6, 8, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphStreets.addEdge(6, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphStreets.addEdge(7, 8, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphStreets.addEdge(7, 9, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphStreets.addEdge(8, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphStreets.addEdge(9, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphStreets.addEdge(10, 12, "Centro", stdSizeEdge, stdMaxSpeed, !oneWay);
    graphStreets.addEdge(11, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);

    // Grafo de metro pra teste
    GraphAdjList graphMetro(13);
    // Copiando aresta dos vértices 4 a 9
    graphMetro.addEdge(3, 4, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(3, 5, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(3, 7, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(4, 6, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(6, 8, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(7, 9, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(8, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);
    graphMetro.addEdge(7, 9, "Centro", stdSizeEdge, stdMaxSpeed, oneWay);

    GraphAdjList* graphStreetsBidirectional = graphStreets.createBidirectionalCopy();
    GraphAdjList* graphMetroBidirectional = graphMetro.createBidirectionalCopy();
    
    testMetro(graphStreets, *graphMetroBidirectional);

    // Testes de táxi
    // Origem
    vertex start = 0;
    // Estação que queremos chegar
    vertex closestStation = closestMetroStation(0, graphStreets, graphMetro);
    // Orçamento
    int budget = 100;

    NodeList rota;
    rotaTaxi(*graphStreetsBidirectional, graphStreets, rota, start, closestStation, budget);

    cout << "Rota até a estação: " << endl;
    rota.print();

    return 0;
}