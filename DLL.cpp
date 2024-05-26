#include "DLL.h"
#include <iostream>

template <typename T>
DLL<T>::DLL() : head(nullptr), tail(nullptr) {}

template <typename T>
DLL<T>::~DLL() {
    while (!isEmpty()) {
        deleteFromHead();
    }
}

template <typename T>
bool DLL<T>::isEmpty() const {
    return head == nullptr;
}

template <typename T>
DLLNode<T>* DLL<T>::begin() const {
    return head;
}

template <typename T>
DLLNode<T>* DLL<T>::end() const {
    return nullptr;
}
template<typename T>
DLLNode<T> & DLLNode<T>::operator=(const DLLNode &other) {
    info = other.info;
    next = other->next;
    prev = other->prev;
    return *this;
}

template <typename T>
void DLL<T>::addToHead(T el) {
    head = new DLLNode<T>(el, head);
    if (head->next) {
        head->next->prev = head;
    }
    if (!tail) {
        tail = head;
    }
}

template <typename T>
void DLL<T>::push_back(T el) {
    if (tail) {
        tail->next = new DLLNode<T>(el, nullptr, tail);
        tail = tail->next;
    } else {
        head = tail = new DLLNode<T>(el);
    }
}

template <typename T>
T DLL<T>::deleteFromHead() {
    if (isEmpty()) throw std::underflow_error("List is empty");

    T el = head->info;
    DLLNode<T>* tmp = head;
    if (head == tail) {
        head = tail = nullptr;
    } else {
        head = head->next;
        head->prev = nullptr;
    }
    delete tmp;
    return el;
}

template <typename T>
T DLL<T>::deleteFromTail() {
    if (isEmpty()) throw std::underflow_error("List is empty");

    T el = tail->info;
    DLLNode<T>* tmp = tail;
    if (head == tail) {
        head = tail = nullptr;
    } else {
        tail = tail->prev;
        tail->next = nullptr;
    }
    delete tmp;
    return el;
}

template <typename T>
void DLL<T>::deleteNode(T el) {
    DLLNode<T>* tmp = head;
    while (tmp && tmp->info != el) {
        tmp = tmp->next;
    }
    if (!tmp) return; // Node not found

    if (tmp == head) {
        deleteFromHead();
    } else if (tmp == tail) {
        deleteFromTail();
    } else {
        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;
        delete tmp;
    }
}

template <typename T>
bool DLL<T>::isInList(T el) const {
    DLLNode<T>* tmp = head;
    while (tmp && tmp->info != el) {
        tmp = tmp->next;
    }
    return tmp != nullptr;
}

template <typename T>
void DLL<T>::printAll() const {
    for (DLLNode<T>* tmp = head; tmp != nullptr; tmp = tmp->next) {
        std::cout << tmp->info << " ";
    }
    std::cout << std::endl;
}

template <typename T>
void DLL<T>::insertBeforeAndAfter(DLLNode<T>* p, T data) {
    if (p == head) {
        addToHead(data);
    } else {
        DLLNode<T>* newNode = new DLLNode<T>(data, p, p->prev);
        p->prev->next = newNode;
        p->prev = newNode;
    }

    if (p == tail) {
        push_back(data);
    } else {
        DLLNode<T>* newNode = new DLLNode<T>(data, p->next, p);
        p->next->prev = newNode;
        p->next = newNode;
    }
}
template <typename T>
template <typename Predicate>
void DLL<T>::remove_if(Predicate pred) {
    DLLNode<T>* current = head;
    while (current) {
        if (pred(current->info)) {
            DLLNode<T>* toDelete = current;
            if (current == head) {
                head = head->next;
                if (head) {
                    head->prev = nullptr;
                } else {
                    tail = nullptr;
                }
            } else if (current == tail) {
                tail = tail->prev;
                if (tail) {
                    tail->next = nullptr;
                } else {
                    head = nullptr;
                }
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            current = current->next;
            delete toDelete;
        } else {
            current = current->next;
        }
    }
}
template<typename T>
void DLL<T>::clear() {
    DLLNode<T>* current = head;
    while (current != nullptr) {
        DLLNode<T>* next = current->next; // Store next node before deletion
        delete current; // Delete current node
        current = next; // Move to next node
    }
    head = nullptr; // Reset head pointer
    tail = nullptr; // Reset tail pointer
}
