#ifndef DLL_H
#define DLL_H

template<typename T>
class DLLNode {
public:
    T info;
    DLLNode* next;
    DLLNode* prev;

    DLLNode() : next(nullptr), prev(nullptr) {}
    DLLNode(T el, DLLNode* ptr_next = nullptr, DLLNode* ptr_prev = nullptr)
        : info(el), next(ptr_next), prev(ptr_prev) {}


    DLLNode& operator=(const DLLNode& other);
};




template<typename T>
class DLL {
public:
    DLL();
    ~DLL();
    bool isEmpty() const;

    DLLNode<T>* begin() const;
    DLLNode<T>* end() const;
    void addToHead(T el);
    void push_back(T el);
    T deleteFromHead();
    T deleteFromTail();
    void deleteNode(T el);
    bool isInList(T el) const;
    void printAll() const;
    void insertBeforeAndAfter(DLLNode<T>* p, T data);
    void clear();

    template <typename Predicate>
    void remove_if(Predicate pred);
private:
    DLLNode<T>* head;
    DLLNode<T>* tail;
};



#include "DLL.cpp"

#endif // DLL_H
