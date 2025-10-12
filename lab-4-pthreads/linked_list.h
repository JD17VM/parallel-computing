#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <pthread.h>

struct Node {
    int data;
    Node* next;
    Node(int val);
};

class ThreadSafeList {
private:
    Node* head;
    pthread_rwlock_t list_rwlock;

public:
    ThreadSafeList();
    ~ThreadSafeList();

    void Init(int initial_size);
    bool Member(int value);
    bool Insert(int value);
    bool Delete(int value);
    void Print();
    void Destroy();
};

#endif