#include <iostream>
#include <ctime>

#include "graph.h"
#include "dijkstra.h"
#include "prim.h"
#include "list.h"
#include "bus.h"
#include "taxi.h"
#include "trafficAPI.h"
#include "utills.h"

using namespace std;


void melhorRota(GraphAdjList& graph, int origem, int destino, int custoMax) {
    NodeList rota1;
    NodeList rota2;
    float resto;
    float horaAtual = getHoraAtual();

    cout << "ANTES API" << endl;
    // Verificando como está o trânsito em toda a cidade
    TrafficAPI trafficAPI;
    trafficAPI.updateTraffic(graph);
    cout << "DEPOIS API" << endl;
    // Criando grafos para andar de taxi ou a pé (direcionado ou não)
    GraphAdjList* graphCompleto = graph.createBidirectionalCopy();
    GraphAdjList* graphDirecionado = graph.clone();
    

    cout << "ANTES CRIAR ONIBUS" << endl;
    // Selecione as arestas mínimas por bairro
    auto arestasMinimas = selecionarArestasMinimasPorBairro(graph);
    cout << "Arestas mínimas por bairro (apenas vértices):\n";
    for (const auto& bairro : arestasMinimas) {
        vertex u = bairro.second.first;
        vertex v = bairro.second.second;
        cout << "Origem: " << u << " | Destino: " << v << endl;
    }
    // Encontra o ciclo para a rota de onibus
    auto ciclo = encontrarCicloArestasMinimas(graph, arestasMinimas);
    cout << "Ciclo passando pelas arestas mínimas:\n";
    for (vertex v : ciclo) {
        cout << v << " ";
    }
    cout << endl;
    cout << "DEPOIS CRIAR ONIBUS" << endl;
    float tempoTaxi = rotaTaxi(*graphCompleto, *graphDirecionado, rota1, origem, destino, custoMax, resto);
    cout << "ROTA SOMENTE COM TAXI:" << endl;
    rota1.print();
    float tempoOnibus = rotaOnibus(*graphCompleto, *graphDirecionado, ciclo, rota2, origem, destino, custoMax, horaAtual);
    cout << "ROTA COM ÔNIBUS:" << endl;
    rota2.print();
    return;
}

float getHoraAtual(){
    // Obtém o tempo atual
    time_t now = std::time(nullptr);

    // Converte para uma estrutura de tempo local
    std::tm* localTime = std::localtime(&now);

    // Obtém a hora, minutos e segundos
    int hours = localTime->tm_hour;
    int minutes = localTime->tm_min;
    int seconds = localTime->tm_sec;

    // Calcula o tempo em horas decimais
    float decimalHours = hours + (minutes / 60.0) + (seconds / 3600.0);

    return decimalHours;

}

float calcularFaltante(float numero) {
    // Extrai a parte decimal do número
    float parteDecimal = numero - static_cast<int>(numero);
    
    // Multiplica por 100 para trabalhar com valores inteiros
    int parteDecimalInt = static_cast<int>(parteDecimal * 100);
    
    // Verifica o valor da parte decimal e calcula a diferença para 00, 25, 50 ou 75
    int diferenca = 0;
    if (parteDecimalInt < 25) {
        diferenca = 25 - parteDecimalInt;
    } else if (parteDecimalInt < 50) {
        diferenca = 50 - parteDecimalInt;
    } else if (parteDecimalInt < 75) {
        diferenca = 75 - parteDecimalInt;
    } else {
        diferenca = 100 - parteDecimalInt;
    }
    
    // Retorna a diferença calculada
    return diferenca / 100.0; // Converte de volta para a parte decimal
}