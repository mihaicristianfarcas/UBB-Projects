#include <cstddef>
#include "SortedBag.h"
#include "SortedBagIterator.h"


//BC: Theta(C)
//WC: Theta(C), where C is the capacity of the hashTable
//Total: Theta(C)
SortedBag::SortedBag(Relation r) {
    this->relation = r;
    this->m = 127;
    this->bagSize = 0;

    this->hashTable = new struct Node*[this->m];
    for (int i = 0; i < this->m; i++) {
        this->hashTable[i] = nullptr;
    }
}


//BC: Theta(1)
//WC: Theta(1)
//Total: Theta(1)
TComp SortedBag::h(TComp k) const {
    int modulo = k % this->m;
    return (modulo < 0) ? (modulo + this->m) : modulo;
}


//BC: Theta(M)
//WC: Theta(M), where M is the maximum number of distinct elements associated to a key
//Total: Theta(M)
void SortedBag::resize() {
    this->m *= 2;

    TComp* elementsCopy = new TComp[this->bagSize];
    int* frequencyCopy = new int[this->bagSize];
    int k = 0;

    for (int i = 0; i < this->m / 2; i++) {
        struct Node *currentNode = this->hashTable[i];
        while (currentNode != nullptr) {
            elementsCopy[k] = currentNode->element;
            frequencyCopy[k++] = currentNode->frequency;
            currentNode = currentNode->next;
        }
    }

    for (int i = 0; i < this->m / 2; i++) {
        struct Node* current_node = this->hashTable[i];
        while (current_node != nullptr) {
            struct Node* next_node = current_node->next;
            delete current_node;
            current_node = next_node;
        }
    }
    delete[] this->hashTable;

    this->bagSize = 0;
    this->hashTable = new struct Node* [this->m];
    for (int i = 0; i < this->m; i++) {
        this->hashTable[i] = nullptr;
    }

    for (int i = 0; i < k; i++) {
        for(int j = 0; j < frequencyCopy[i]; j++) {
            this->add(elementsCopy[i]);
        }
    }

    delete[] elementsCopy;
}


//BC: O(M),
//WC: O(M), where M is the maximum number of distinct elements associated to a key
//Total: O(M)
void SortedBag::add(TComp e) {
    auto loadFactor = this->bagSize / this->m;
    if (loadFactor >= 1) {
        this->resize();
    }

    TComp pos = this->h(e);

    struct Node* currentNode = this->hashTable[pos];

    while (currentNode != nullptr && e != currentNode->element) {
        currentNode = currentNode->next;
    }

    if(currentNode != nullptr) {
        currentNode->frequency++;
    } else {
        currentNode = this->hashTable[pos];
        struct Node* prevNode = nullptr;

        auto* newNode = new struct Node;
        newNode->element = e;
        newNode->frequency = 1;

        while (currentNode != nullptr && !relation(e, currentNode->element)) {
            prevNode = currentNode;
            currentNode = currentNode->next;
        }

        if (prevNode == nullptr) {
            newNode->next = this->hashTable[pos];
            this->hashTable[pos] = newNode;
        } else {
            newNode->next = currentNode;
            prevNode->next = newNode;
        }
    }

    this->bagSize++;
}


//BC: Theta(1)
//WC: O(M), where M is the maximum number of distinct elements associated to a key
//Total: O(M)
int SortedBag::nrOccurrences(TComp elem) const {
    TComp pos = this->h(elem);
    struct Node* currentNode = this->hashTable[pos];

    while (currentNode != nullptr && elem != currentNode->element) {
        currentNode = currentNode->next;
    }

    return (currentNode != nullptr) ? currentNode->frequency : 0;
}


//BC: Theta(1)
//WC: O(M), where M is the maximum number of distinct elements associated to a key
//Total: O(M)
bool SortedBag::remove(TComp e) {
    TComp pos = this->h(e);
    struct Node* currentNode = this->hashTable[pos];
    struct Node* prevNode = nullptr;

    while (currentNode != nullptr && e != currentNode->element) {
        prevNode = currentNode;
        currentNode = currentNode->next;
    }

    if (currentNode != nullptr) {
        if (currentNode->frequency == 1) {
            if (prevNode == nullptr) {
                this->hashTable[pos] = this->hashTable[pos]->next;
            } else {
                prevNode->next = currentNode->next;
            }
        } else {
            currentNode->frequency--;
        }
        this->bagSize--;
        return true;
    } else {
        return false;
    }
}


//BC: Theta(1)
//WC: O(M), where M is the maximum number of distinct elements associated to a key
//Total: O(M)
bool SortedBag::search(TComp elem) const {
    TComp pos = this->h(elem);
    struct Node* currentNode = this->hashTable[pos];

    while (currentNode != nullptr && elem != currentNode->element) {
        currentNode = currentNode->next;
    }

    return (currentNode != nullptr);
}


//BC: Theta(1)
//WC: Theta(1)
//Total: Theta(1)
int SortedBag::size() const {
    return this->bagSize;
}


//BC: Theta(1)
//WC: Theta(1)
//Total: Theta(1)
bool SortedBag::isEmpty() const {
    return (this->bagSize == 0);
}


SortedBagIterator SortedBag::iterator() const {
    return SortedBagIterator(*this);
}


//BC: Theta(N), where N is the number of elements
//WC: Theta(N)
//Total: Theta(N)
SortedBag::~SortedBag() {
    for (int i = 0; i < this->m / 2; i++) {
        struct Node* currentNode = this->hashTable[i];
        while (currentNode != nullptr) {
            struct Node* nextNode = currentNode->next;
            delete currentNode;
            currentNode = nextNode;
        }
    }
    delete[] this->hashTable;
}


//BC: Theta(1)
//WC: Theta(N), where N is the number of distinct elements in the bag
//Total: Theta(N)
int SortedBag::elementsWithMinimumFrequency() const {
    if (this->isEmpty()) {
        return 0;
    }

    int minFrequency = 100000;
    int count = 0;

    for (int i = 0; i < this->m; i++) {
        struct Node* currentNode = this->hashTable[i];
        while (currentNode != nullptr) {
            if (currentNode->frequency < minFrequency) {
                minFrequency = currentNode->frequency;
                count = 1; // Reset count as we found a new minimum frequency
            } else if (currentNode->frequency == minFrequency) {
                count++;
            }
            currentNode = currentNode->next;
        }
    }

    return count;
}
