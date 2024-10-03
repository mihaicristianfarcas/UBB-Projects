#include "MultiMapIterator.h"
#include "MultiMap.h"


MultiMapIterator::MultiMapIterator(const MultiMap& c): multiMap(c) {
    currentIndex = multiMap.head;
}

TElem MultiMapIterator::getCurrent() const{
    if (currentIndex == -1)
        throw exception();

	return multiMap.nodes[currentIndex].data;
}

bool MultiMapIterator::valid() const {
	return currentIndex != -1;
}

void MultiMapIterator::next() {
    if (!valid())
        throw exception();

    currentIndex = multiMap.nodes[currentIndex].next;
}

void MultiMapIterator::first() {
    currentIndex = multiMap.head;
}

