#include "ListIterator.h"
#include "SortedIndexedList.h"
#include <iostream>
using namespace std;
#include <exception>

SortedIndexedList::SortedIndexedList(Relation r) : head(nullptr), relation(r), listSize(0) {}

int SortedIndexedList::size() const {
	return listSize;
}

bool SortedIndexedList::isEmpty() const {
	return listSize == 0;
}

TComp SortedIndexedList::getElement(int pos) const {
    if (pos < 0 || pos >= listSize)
        throw std::out_of_range("Invalid position");

    Node *current = head;
    for (int i = 0; i < pos; i++) {
        current = current->next;
    }
    return current->data;
}

TComp SortedIndexedList::remove(int pos) {
    if (pos < 0 || pos >= listSize)
        throw std::out_of_range("Invalid position");

    Node* current = head;
    for (int i = 0; i < pos; i++) {
        current = current->next;
    }

    if (current->prev == nullptr) {
        head = current->next;
        if (head != nullptr)
            head->prev = nullptr;
    } else {
        current->prev->next = current->next;
        if (current->next != nullptr)
            current->next->prev = current->prev;
    }
    TComp removed = current->data;
    delete current;
    listSize--;
    return removed;
}

int SortedIndexedList::removeFromKtoK(int k) {
    if (k <= 0)
        throw std::out_of_range("Invalid position");

    Node* current = head;
    int i;
    for (i = 0; i < k; i++) {
        current = current->next;
    }

    int count = 0;
    while(i < listSize) {
        if (i % k == 0) {
            current->prev->next = current->next;
            if (current->next != nullptr)
                current->next->prev = current->prev;
            // TODO last node?
            count++;
            listSize--;
        }
        i++;
    }

    return count;
}

int SortedIndexedList::search(TComp e) const {
    int pos = 0;
    Node* current = head;
    while (current != nullptr && relation(current->data, e)) {
        if (current->data == e)
            return pos;
        current = current->next;
        pos++;
    }
    return -1;
}

void SortedIndexedList::add(TComp e) {
    Node* new_node = new Node(e);
    if (head == nullptr || relation(e, head->data)) {
        new_node->next = head;
        if (head != nullptr)
            head->prev = new_node;
        head = new_node;
    } else {
        Node* current = head;
        while (current->next != nullptr && !relation(e, current->next->data)) {
            current = current->next;
        }
        new_node->next = current->next;
        if (current->next != nullptr)
            current->next->prev = new_node;
        current->next = new_node;
        new_node->prev = current;
    }
    listSize++;
}

ListIterator SortedIndexedList::iterator(){
	return ListIterator(*this);
}

//destructor
SortedIndexedList::~SortedIndexedList() {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}
