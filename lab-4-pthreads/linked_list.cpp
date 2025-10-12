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

void ThreadSafeList::Init(int initial_size) { }
bool ThreadSafeList::Member(int value) { return false; }
bool ThreadSafeList::Insert(int value) { return false; }
bool ThreadSafeList::Delete(int value) { return false; }
void ThreadSafeList::Print() { }