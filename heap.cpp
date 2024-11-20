#include "heap.h"

void MinHeap::heapify_up(int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap[idx].first >= heap[parent].first) break;
        std::swap(heap[idx], heap[parent]);
        position[heap[idx].second] = idx;
        position[heap[parent].second] = parent;
        idx = parent;
    }
}

void MinHeap::heapify_down(int idx) {
    int n = heap.size();
    while (true) {
        int smallest = idx;
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;

        if (left < n && heap[left].first < heap[smallest].first)
            smallest = left;
        if (right < n && heap[right].first < heap[smallest].first)
            smallest = right;

        if (smallest == idx) break;

        std::swap(heap[idx], heap[smallest]);
        position[heap[idx].second] = idx;
        position[heap[smallest].second] = smallest;
        idx = smallest;
    }
}

void MinHeap::insert_or_update(int priority, int vertex) {
    if (position.find(vertex) != position.end()) {
        int idx = position[vertex];
        heap[idx].first = priority;
        heapify_up(idx);
        heapify_down(idx);
    } else {
        heap.emplace_back(priority, vertex);
        int idx = heap.size() - 1;
        position[vertex] = idx;
        heapify_up(idx);
    }
}

int MinHeap::extract_min() {
    if (heap.empty()) {
        return -1;  // Retorna um valor inválido se a heap estiver vazia
    }
    return heap[0].second;  // Retorna apenas o vertex do mínimo
}

void MinHeap::pop() {
    if (heap.empty()) {
        return;  // Se a heap estiver vazia, não faz nada
    }
    std::swap(heap[0], heap.back());
    position[heap[0].second] = 0;
    position.erase(heap.back().second);  // Remove o último elemento do mapa
    heap.pop_back();
    heapify_down(0);
}

bool MinHeap::empty() const {
    return heap.empty();
}
