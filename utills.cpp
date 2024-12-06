#include <iostream>
#include <ctime>
#include <string>

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

    // Verificando como está o trânsito em toda a cidade
    TrafficAPI trafficAPI;
    trafficAPI.updateTraffic(graph);
    // Criando grafos para andar de taxi ou a pé (direcionado ou não)
    GraphAdjList* graphCompleto = graph.createBidirectionalCopy();
    GraphAdjList* graphDirecionado = graph.clone();
    

    // Selecione as arestas mínimas por bairro
    auto arestasMinimas = selecionarArestasMinimasPorBairro(graph);
    // Encontra o ciclo para a rota de onibus
    auto ciclo = encontrarCicloArestasMinimas(graph, arestasMinimas);

    float tempoTaxi = rotaTaxi(*graphCompleto, *graphDirecionado, rota1, origem, destino, custoMax, resto);

    float tempoOnibus = rotaOnibus(*graphCompleto, *graphDirecionado, ciclo, rota2, origem, destino, custoMax, horaAtual);

    cout << "========= MELHOR ROTA ENCONTRADA =========" << endl;
    if (tempoTaxi<tempoOnibus){
        printRota(rota1);
    } else{
        printRota(rota2);
    }
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

void printRota(NodeList& rota){
    ListNode* temp = rota.head;
    string transporte;
    float tempoTotal = 0;
    int nCaminhos =0;
    while (temp) {
        nCaminhos++;
        cout << "---------------------------" << endl;
        int meio = temp->meioDeTransporte;
        tempoTotal+=temp->tempoCaminho;
        if (meio == 0){
            transporte = "a pé";
        } else if (meio == 1){
            transporte  = "de taxi";
        } else if (meio == 2) {
            transporte = "de ônibus";
        } else{
            transporte = "de metrô";
        }

        cout << "Tempo do caminho (minutos): " << (temp->tempoCaminho)*60 << endl;
        cout << "Faça o seguinte caminho " << transporte << ": ";

        IntNode* subTemp = temp->subList;
        while (subTemp) {
            cout << subTemp->data << " ";
            subTemp = subTemp->next;
        }
        cout << endl;

        temp = temp->next;
    }
    cout << "---------------------------" << endl;
    if (nCaminhos == 1){
        cout << "Fazendo esse caminho você chegará ao seu destino em " << tempoTotal*60 << " minutos." << endl;
    } else{
        cout << "Fazendo esses " << nCaminhos << " caminhos, você chegará ao seu destino em " << tempoTotal*60 << " minutos." << endl;
    }
    
}
