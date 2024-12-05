#ifndef HEAP_H
#define HEAP_H

#include <unordered_map>
#include <vector>
#include <climits>  // Para usar INT_MAX

class MinHeap {
private:
    std::vector<std::pair<int, int>> heap;
    std::unordered_map<int, int> position; // vertex -> index no heap

    void heapify_up(int idx);
    void heapify_down(int idx);

public:
    void insert_or_update(int priority, int vertex);
    int extract_min();
    void pop();
    bool empty() const;
};

#endif
