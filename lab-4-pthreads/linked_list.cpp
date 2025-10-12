#include "linked_list.h"
#include <iostream>

using namespace std;

Node::Node(int val) : data(val), next(nullptr) {}

ThreadSafeList::ThreadSafeList() : head(nullptr) {
    pthread_rwlock_init(&list_rwlock, NULL);
}

ThreadSafeList::~ThreadSafeList() {
    Destroy();
    pthread_rwlock_destroy(&list_rwlock);
}

void ThreadSafeList::Destroy() {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
}

void ThreadSafeList::Destroy() {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
}

void ThreadSafeList::Init(int initial_size) {
    for (int i = 0; i < initial_size; ++i) {
        Insert(i * 2);
    }
}

bool ThreadSafeList::Member(int value) {
}

bool ThreadSafeList::Insert(int value) {
    pthread_rwlock_wrlock(&list_rwlock);
    Node* current = head;
    Node* pred = nullptr;

    while (current != nullptr && current->data < value) {
        pred = current;
        current = current->next;
    }

    if (current == nullptr || current->data > value) {
        Node* new_node = new Node(value);
        new_node->next = current;
        if (pred == nullptr) {
            head = new_node;
        } else {
            pred->next = new_node;
        }
        pthread_rwlock_unlock(&list_rwlock);
        return true;
    }

    pthread_rwlock_unlock(&list_rwlock);
    return false;
}


bool ThreadSafeList::Delete(int value) { return false; }
void ThreadSafeList::Print() {}