#include <iostream>
#include <cassert>
#include <vector>

#include "graph.h"
#include "metro.h"
#include "taxi.h"
#include "task1.h"

void testMetro(GraphAdjList& graphStreets, GraphAdjList& graphMetroBidirectional, const vector<vertex>& stations) {
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
        assert(currentVertex->data == expectedCurrentVertex->data);
        expectedCurrentVertex = expectedCurrentVertex->next;
        currentVertex = currentVertex->next;
    }
    
    cout << "Teste do caminho passou!" << endl;

    // Vértices esperado no grafo de metrô
    vertex expectedStation1 = 1;
    vertex expectedStation2 = 10;
    vertex expectedNotStation = 6;

    // Verificando resultados;
    assert(isStation(expectedStation1, stations));
    assert(isStation(expectedStation2, stations));
    assert(!isStation(expectedNotStation, stations));

    cout << "Teste de verificação de estação passou!" << endl;

    // Testes da função de estação mais próxima.
    // Vértices esperados
    // Vértice antes da primeiro estação
    vertex expectedClosestStation1 = 1;
    // Vértice "dentro" do metrô mais próximo da primeira estação
    vertex expectedClosestStation2 = 1;
    // vértice mais próximo da segunda estação
    vertex expectedClosestStation3 = 10;

    // Rodando função
    vertex closestStation1 = closestMetroStation(2, graphStreets, stations);
    vertex closestStation2 = closestMetroStation(3, graphStreets, stations);
    vertex closestStation3 = closestMetroStation(9, graphStreets, stations);

    // Verificando resultados
    assert(expectedClosestStation1 == closestStation1);
    assert(expectedClosestStation2 == closestStation2);
    assert(expectedClosestStation3 == closestStation3);

    cout << "Teste da estação mais próxima passou!" << endl;

    // Teste da função de tempo gasto
    const int METRO_SPEED = 70;
    
    float timeTaken = timeMetro(graphMetroBidirectional, *pathMetro, METRO_SPEED);
    float expectedTimeTaken = 380.0/70.0;

    assert(expectedTimeTaken == timeTaken);

    cout << "Teste do tempo passou!" << endl;
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
    // Colocando algumas arestas um pouco menores para garantir o caminho de metro do teste
    graphStreets.addEdge(3, 7, "Centro", stdSizeEdge - 10, stdMaxSpeed, oneWay, stdNumLots, lotsType);
    graphStreets.addEdge(4, 6, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, stdNumLots, lotsType);
    graphStreets.addEdge(5, 7, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, stdNumLots, lotsType);
    graphStreets.addEdge(6, 8, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, stdNumLots, lotsType);
    graphStreets.addEdge(6, 10, "Centro", stdSizeEdge, stdMaxSpeed, oneWay, stdNumLots, lotsType);
    graphStreets.addEdge(7, 8, "Centro", stdSizeEdge - 10, stdMaxSpeed, oneWay, stdNumLots, lotsType);
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
    GraphAdjList graphMetroBidirectional = *createTreeMetro(*graphStreetsBidirectional, stations);

    cout << "Printando grafo de metro:" << endl;
    graphMetroBidirectional.print();

    cout << "Rodando testes do metro" << endl;
    testMetro(graphStreets, graphMetroBidirectional, stations);
    
    return 0;
}