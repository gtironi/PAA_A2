#include "list.h"

// Implementação do construtor de IntNode
IntNode::IntNode(int value) : data(value), next(nullptr) {}

// Implementação do construtor de ListNode
ListNode::ListNode(int meio, float tempo) 
    : meioDeTransporte(meio), tempoCaminho(tempo), subList(nullptr), next(nullptr) {}

// Implementação do construtor de IntList
IntList::IntList() : head(nullptr) {}

// Implementação de append para IntList
void IntList::append(int value) {
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

// Implementação de insert_front para IntList
void IntList::insert_front(int value) {
    IntNode* newNode = new IntNode(value);
    newNode->next = head;
    head = newNode;
}

// Implementação de print para IntList
void IntList::print() {
    IntNode* temp = head;
    while (temp) {
        cout << temp->data << " ";
        temp = temp->next;
    }
    cout << endl;
}

// Implementação de clear para IntList
void IntList::clear() {
    while (head) {
        IntNode* temp = head;
        head = head->next;
        delete temp;
    }
}

// Implementação do construtor de NodeList
NodeList::NodeList() : head(nullptr) {}

// Implementação de append para NodeList
void NodeList::append(int meioDeTransporte, float tempoCaminho, IntList* subList) {
    ListNode* newNode = new ListNode(meioDeTransporte, tempoCaminho);
    newNode->subList = subList ? subList->head : nullptr;
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

// Implementação de print para NodeList
void NodeList::print() {
    ListNode* temp = head;
    while (temp) {
        cout << "Meio de Transporte: " << temp->meioDeTransporte
             << ", Tempo Caminho: " << temp->tempoCaminho << endl;

        cout << "Sub-lista: ";
        IntNode* subTemp = temp->subList;
        while (subTemp) {
            cout << subTemp->data << " ";
            subTemp = subTemp->next;
        }
        cout << endl;

        temp = temp->next;
    }
}

// Implementação de clear para NodeList
void NodeList::clear() {
    while (head) {
        ListNode* temp = head;
        head = head->next;

        // Limpa a sub-lista
        IntNode* subTemp = temp->subList;
        while (subTemp) {
            IntNode* toDelete = subTemp;
            subTemp = subTemp->next;
            delete toDelete;
        }

        delete temp;
    }
}