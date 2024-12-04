#include <iostream>
#include "list.h"
using namespace std;

// Estrutura para a lista de inteiros
struct IntNode {
    int data;
    IntNode* next;

    IntNode(int value) : data(value), next(nullptr) {}
};

// Estrutura para a lista de nós com sub-lista
struct ListNode {
    int meioDeTransporte;  // Inteiro representando o meio de transporte
    int tempoCaminho;      // Inteiro representando o tempo do caminho
    IntNode* subList;      // Ponteiro para uma sub-lista de inteiros
    ListNode* next;        // Ponteiro para o próximo nó na lista principal

    ListNode(int meio, int tempo)
        : meioDeTransporte(meio), tempoCaminho(tempo), subList(nullptr), next(nullptr) {}
};

// Funções para manipular a lista de inteiros
struct IntList {
    IntNode* head = nullptr;

    IntList::IntList() : head(nullptr) {}

    void insert_front(int value) {
        // Cria um novo nó com o valor fornecido
        IntNode* newNode = new IntNode(value);
        
        // Faz o novo nó apontar para o nó atual da cabeça
        newNode->next = head;
        
        // Atualiza a cabeça da lista para o novo nó
        head = newNode;
    }

    void append(int value) {
        IntNode* newNode = new IntNode(value);
        if (!head) {
            head = newNode;
        } else {
            IntNode* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    void print() {
        IntNode* temp = head;
        while (temp) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }

    void clear() {
        while (head) {
            IntNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// Funções para manipular a lista de nós com sub-listas
struct NodeList {
    ListNode* head = nullptr;

    NodeList::NodeList() : head(nullptr) {}

    void append(int meioDeTransporte, int tempoCaminho, IntList* subList = nullptr) {
        ListNode* newNode = new ListNode(meioDeTransporte, tempoCaminho);
        if (subList) {
            newNode->subList = subList->head; // Copia a sub-lista para o nó
        }
        if (!head) {
            head = newNode;
        } else {
            ListNode* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    void print() {
        ListNode* temp = head;
        while (temp) {
            cout << "Meio de Transporte: " << temp->meioDeTransporte
                 << ", Tempo do Caminho: " << temp->tempoCaminho
                 << ", Sub-list: ";
            IntNode* subListTemp = temp->subList;
            while (subListTemp) {
                cout << subListTemp->data << " ";
                subListTemp = subListTemp->next;
            }
            cout << endl;
            temp = temp->next;
        }
    }

    void clear() {
        while (head) {
            ListNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main() {
    // Criando uma lista de inteiros
    IntList list1;
    list1.append(1);
    list1.append(2);
    list1.append(3);

    cout << "Lista de inteiros:" << endl;
    list1.print();

    // Criando uma lista de nós com sub-listas
    NodeList nodeList;
    nodeList.append(1, 10, &list1); // Nó com meio de transporte 1, tempo 10 e sub-lista list1

    // Outra sub-lista
    IntList list2;
    list2.append(4);
    list2.append(5);

    nodeList.append(2, 20, &list2); // Nó com meio de transporte 2, tempo 20 e sub-lista list2
    nodeList.append(3, 30);        // Nó com meio de transporte 3, tempo 30 e sem sub-lista

    cout << "\nLista de nós:" << endl;
    nodeList.print();

    // Limpar memória
    list1.clear();
    list2.clear();
    nodeList.clear();

    return 0;
}
