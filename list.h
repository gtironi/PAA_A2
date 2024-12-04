#ifndef LIST_H
#define LIST_H

#include <iostream>
using namespace std;

// Estrutura para a lista de inteiros
struct IntNode {
    int data;         // Valor inteiro armazenado no nó
    IntNode* next;    // Ponteiro para o próximo nó na lista

    IntNode(int value); // Construtor
};

// Estrutura para a lista de nós com sub-listas
struct ListNode {
    int meioDeTransporte;  // Inteiro representando o meio de transporte
    int tempoCaminho;      // Inteiro representando o tempo do caminho
    IntNode* subList;      // Ponteiro para uma sub-lista de inteiros
    ListNode* next;        // Ponteiro para o próximo nó na lista principal

    ListNode(int meio, int tempo); // Construtor
};

// Estrutura para manipular a lista de inteiros
struct IntList {
    IntNode* head;   // Ponteiro para o início da lista

    IntList();             // Construtor
    void insert_front(int value);
    void append(int value); // Adiciona um valor à lista
    void print();           // Imprime os elementos da lista
    void clear();           // Limpa a memória da lista
};

// Estrutura para manipular a lista de nós com sub-listas
struct NodeList {
    ListNode* head;  // Ponteiro para o início da lista

    NodeList();                                   // Construtor
    void append(int meioDeTransporte, int tempoCaminho, IntList* subList = nullptr); 
                                                 // Adiciona um nó à lista
    void print();                                // Imprime os nós e suas sub-listas
    void clear();                                // Limpa a memória da lista
};

#endif // LIST_H