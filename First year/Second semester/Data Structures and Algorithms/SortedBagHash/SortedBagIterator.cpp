#include "SortedBagIterator.h"
#include "SortedBag.h"
#include <exception>

using namespace std;


//BC: Theta(N^2), where N is the number of elements
//WC: Theta(N^2)
//Total: Theta(N^2)
SortedBagIterator::SortedBagIterator(const SortedBag& b) : bag(b) {
    this->copyElements = new TComp[b.bagSize];
    int k = 0;
    for (int i = 0; i < this->bag.m; i++) {
        struct Node* current_node = this->bag.hashTable[i];
        while (current_node != nullptr) {
            for(int j=0; j<current_node->frequency; j++) {
                this->copyElements[k++] = current_node->element;
            }
            current_node = current_node->next;
        }
    }

    for(int i=0; i< this->bag.bagSize - 1; i++) {
        for (int j = i + 1; j < this->bag.bagSize; j++) {
            if (!this->bag.relation(this->copyElements[i], this->copyElements[j])) {
                int aux = this->copyElements[i];
                this->copyElements[i] = this->copyElements[j];
                this->copyElements[j] = aux;
            }
        }
    }

    this->copyCurrentPosition = 0;
    int first_elem = this->copyElements[0];
    this->currentPosition = this->bag.h(first_elem);
    this->currentElement = this->bag.hashTable[this->currentPosition];
}


//BC: Theta(1)
//WC: O(M), where M is the maximum number of distinct elements associated to a key
//Total: O(N)
TComp SortedBagIterator::getCurrent() {
    if (!this->valid()) {
        throw std::exception();
    }

    this->currentPosition = this->bag.h(copyElements[copyCurrentPosition]);
    struct Node* current_node = this->bag.hashTable[this->currentPosition];

    while (current_node != nullptr && copyElements[copyCurrentPosition] != current_node->element) {
        current_node = current_node->next;
    }
    this->currentElement = current_node;

    return this->currentElement->element;
}


//BC: Theta(1)
//WC: Theta(1)
//Total: Theta(1)
bool SortedBagIterator::valid() {
    return (this->copyCurrentPosition != this->bag.bagSize);
}


//BC: Theta(1)
//WC: O(M), where M is the maximum number of distinct elements associated to a key
//Total: Theta(1)
void SortedBagIterator::next() {
    if (!this->valid()) {
        throw std::exception();
    }

    this->copyCurrentPosition++;

    if (this->copyElements[this->copyCurrentPosition] != copyElements[this->copyCurrentPosition - 1]) {
        this->currentPosition = this->bag.h(copyElements[copyCurrentPosition]);
        struct Node* current_node = this->bag.hashTable[this->currentPosition];

        while (current_node != NULL && copyElements[copyCurrentPosition] != current_node->element) {
            current_node = current_node->next;
        }
        this->currentElement = current_node;
    }
}


//BC: Theta(1)
//WC: Theta(1)
//Total: Theta(1)
void SortedBagIterator::first() {
    this->copyCurrentPosition = 0;
    int first_elem = this->copyElements[0];
    this->currentPosition = this->bag.h(first_elem);
    this->currentElement = this->bag.hashTable[this->currentPosition];
}