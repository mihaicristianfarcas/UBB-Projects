#include "MultiMap.h"
#include "MultiMapIterator.h"
#include <exception>
#include <iostream>

using namespace std;


MultiMap::MultiMap() {
    capacity = 10;
    nodes = new DLLANode[capacity];

    head = -1;
    tail = -1;
    firstEmpty = 0;

    for (int i = 0; i < capacity - 1; i++) {
        nodes[i].next = i + 1;
        nodes[i].prev = i - 1;
    }
    nodes[capacity - 1].next = -1;

    listSize = 0;
}

int MultiMap::allocate() {
    int newElem = firstEmpty;
    if (newElem != -1) {
        firstEmpty = nodes[firstEmpty].next;
        if (firstEmpty != -1) {
            nodes[firstEmpty].prev = -1;
        }
        nodes[newElem].prev = -1;
        nodes[newElem].next = -1;
    }
    return newElem;
}

void MultiMap::free(int pos) {
    nodes[pos].next = firstEmpty;
    nodes[pos].prev = -1;
    if (firstEmpty != -1) {
        nodes[firstEmpty].prev = pos;
    }
    firstEmpty = pos;
}

void MultiMap::add(TKey c, TValue v) {

    TElem newPair(c, v);
    int newElem = allocate();

    if (newElem == -1) {
        resize();
        newElem = allocate();
    }

    nodes[newElem].data = newPair;
    nodes[newElem].prev = tail;
    nodes[newElem].next = -1;

    if (isEmpty()) {
        head = newElem;
    } else {
        nodes[tail].next = newElem;
    }
    tail = newElem;
    listSize++;
}

void MultiMap::resize() {

    capacity *= 2;
    auto* newNodes = new DLLANode[capacity];

    for (int i = 0; i < listSize; ++i) {
        newNodes[i] = nodes[i];
    }

    for (int i = listSize; i < capacity; ++i) {
        newNodes[i].next = i + 1;
        newNodes[i].prev = i - 1;
    }
    newNodes[capacity - 1].next = -1;

    delete[] nodes;
    nodes = newNodes;
    firstEmpty = listSize;
}

bool MultiMap::remove(TKey c, TValue v) {

    TElem target;
    target.first = c;
    target.second = v;

    int current = head;
    while (current != -1) {
        if (nodes[current].data == target) {
            if (nodes[current].prev != -1) {
                nodes[nodes[current].prev].next = nodes[current].next;
            } else {
                head = nodes[current].next;
            }

            if (nodes[current].next != -1) {
                nodes[nodes[current].next].prev = nodes[current].prev;
            } else {
                tail = nodes[current].prev;
            }

            free(current);
            listSize--;
            return true;
        }
        current = nodes[current].next;
    }
    return false;
}

vector<TValue> MultiMap::search(TKey c) const {
    vector<TValue> allItemsOfKeyC;
    int current = head;
    while (current != -1) {
        if (nodes[current].data.first == c)
            allItemsOfKeyC.push_back(nodes[current].data.second);
        current = nodes[current].next;
    }
    return allItemsOfKeyC;

//    vector<TValue> allItemsOfKeyC;
//    MultiMapIterator it = this->iterator();
//    it.first();
//    while(it.valid()) {
//        TElem element = it.getCurrent();
//        if (element.first == c)
//            allItemsOfKeyC.push_back(element.second);
//        it.next();
//    }
//
//    return allItemsOfKeyC;
}

bool MultiMap::isKeyInMap(TKey c) {
    int current = head;
    while (current != -1) {
        if (nodes[current].data.first == c)
            return true;
        current = nodes[current].next;
    }
    return false;
}

int MultiMap::size() const {
    return listSize;
}

bool MultiMap::isEmpty() const {
	return listSize == 0;
}

MultiMapIterator MultiMap::iterator() const {
	return MultiMapIterator(*this);
}

MultiMap::~MultiMap() {
	delete[] nodes;
}

int MultiMap::addIfNotPresent(MultiMap &m) {
    int nrPairs = 0;
    std::vector<TElem> pairs;
    MultiMapIterator itm = m.iterator();

    while(itm.valid()) {
        TElem current = itm.getCurrent();
        if (!this->isKeyInMap(current.first)) {
            for (auto value: m.search(current.first)) {
                TElem elem{current.first, value};
                this->add(elem.first, elem.second);
                nrPairs++;
            }
        }
        itm.next();
    }

    return nrPairs;
}
