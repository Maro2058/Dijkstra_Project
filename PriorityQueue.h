#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

template<typename T, typename Compare = std::greater<T>>

class PriorityQueue {
    std::vector<T> data;
    Compare comp;

public:
    bool empty() const {
        return data.empty();
    }

    void push(const T& value) {
        data.push_back(value);
        std::push_heap(data.begin(), data.end(), comp);
    }

    void pop() {
        std::pop_heap(data.begin(), data.end(), comp);
        data.pop_back();
    }

    T top() const {
        return data.front();
    }
};

#include "PriorityQueue.cpp"

#endif // PRIORITY_QUEUE_H
