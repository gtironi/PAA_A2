#include <iostream>
#include <cassert>
#include <vector>

#include "graph.h"
#include "metro.h"
#include "taxi.h"
#include "task1.h"

void testMetro(GraphAdjList& graphStreets, GraphAdjList& graphMetroBidirectional) {
    //======================================================================
    // Testando funções de metrô

    // Resultados esperados
    IntList expectedPathMetro;
    expectedPathMetro.insert_front(10);
    expectedPathMetro.insert_front(8);
    expectedPathMetro.insert_front(7);
    expectedPathMetro.insert_front(3);
    expectedPathMetro.insert_front(1);
    
    // Criação do caminho no formato de lista
    IntList* pathMetro = createPathMetro(graphMetroBidirectional, 1, 10);


    // Começo das duas listas
    IntNode* currentVertex = pathMetro->head;
    IntNode* expectedCurrentVertex = expectedPathMetro.head;

    // Verificando resultados
    while (currentVertex != nullptr && expectedCurrentVertex != nullptr){
        cout << currentVertex->data << endl;
        cout << expectedCurrentVertex->data << endl;
        assert(currentVertex->data == expectedCurrentVertex->data);
        expectedCurrentVertex = expectedCurrentVertex->next;
        currentVertex = currentVertex->next;
    }
    
    cout << "Teste do caminho passou!" << endl;

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
    vertex expectedStation = 1;

    // Rodando função
    vertex closestStation = closestMetroStation(2, graphStreets, graphMetroBidirectional);

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
    // Valores padrão para criação do teste
    int stdSizeEdge = 100;
    int stdMaxSpeed = 60;
    bool oneWay = true;
    int stdNumLots = 1;
    int lotsType[4] = {1, 0, 0, 0};

    // Criar o grafo (exemplo)
    GraphAdjList graphStreets(13);

    int lotesType[4] = {1, 0, 0, 0};

    // Primeira região: vértices de 0 a 7
    graphStreets.addEdge(0, 1, "Centro", stdSizeEdge, stdMaxSpeed, !oneWay, stdNumLots, lotsType);
    graphStreets.addEdge(0, 2, "Centro", stdSizeEdge, stdMaxSpeed, !oneWay, stdNumLots, lotsType);
    graphStreets.addEdge(1, 3, "Centro", stdSizeEdge, stdMaxSpeed, !oneWay, stdNumLots, lotsType);
    graphStreets.addEdge(3, 4, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, stdNumLots, lotsType);
    graphStreets.addEdge(3, 5, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, stdNumLots, lotsType);
    graphStreets.addEdge(3, 7, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, stdNumLots, lotsType);
    graphStreets.addEdge(4, 6, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, stdNumLots, lotsType);
    graphStreets.addEdge(5, 7, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, stdNumLots, lotsType);
    graphStreets.addEdge(6, 8, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, stdNumLots, lotsType);
    graphStreets.addEdge(6, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, stdNumLots, lotsType);
    graphStreets.addEdge(7, 8, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, stdNumLots, lotsType);
    graphStreets.addEdge(7, 9, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, stdNumLots, lotsType);

    // Segunda região: vértices de 8 a 12
    graphStreets.addEdge(8, 10, "Zona Sul", stdSizeEdge, stdMaxSpeed, oneWay, stdNumLots, lotsType);
    graphStreets.addEdge(9, 10, "Zona Sul", stdSizeEdge, stdMaxSpeed, oneWay, stdNumLots, lotsType);
    graphStreets.addEdge(10, 12, "Zona Sul", stdSizeEdge, stdMaxSpeed, !oneWay, stdNumLots, lotsType);
    graphStreets.addEdge(12, 10, "Zona Sul", stdSizeEdge, stdMaxSpeed, !oneWay, stdNumLots, lotsType);
    graphStreets.addEdge(11, 10, "Zona Sul", stdSizeEdge, stdMaxSpeed, oneWay, stdNumLots, lotsType);

    // Grafo completo
    GraphAdjList* graphStreetsBidirectional = graphStreets.createBidirectionalCopy();

    cout << "Grafo normal: " << endl;
    graphStreets.print();

    // Gerar regiões
    cout << "Rodando função para gerar regiões" << endl;
    vector<vector<vertex>> regions = generateRegions(*graphStreetsBidirectional);
    cout << "Regiões: " << endl;
    for (int i = 0; i < regions.size(); i++){
        cout << "Linha " << i << ": ";
        for (int j = 0; j < regions[i].size(); j++){
            cout << regions[i][j] << " ";
        }
        cout << endl;
    }

    // Determinar estações de metrô
    cout << "Rodando função para determinar estações" << endl;
    vector<vertex> stations = determineStations(*graphStreetsBidirectional, regions);

    cout << "Estações: " << endl;
    for (int i = 0; i < stations.size(); i++){
        cout << stations[i] << " ";
    }
    cout << endl;

    cout << "Rodando função para criar subgrafo de estações" << endl;
    GraphAdjList graphMetroBidirectional = *createTreeMetro(*graphStreetsBidirectional, stations, 70);

    cout << "Printando grafo de metro:" << endl;
    graphMetroBidirectional.print();

    cout << "Rodando testes do metro" << endl;
    testMetro(graphStreets, graphMetroBidirectional);
    
    return 0;
}