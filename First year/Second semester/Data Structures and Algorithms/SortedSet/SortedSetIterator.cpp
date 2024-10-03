#include "SortedSetIterator.h"
#include <exception>

using namespace std;

SortedSetIterator::SortedSetIterator(const SortedSet& m) : multime(m)
{
	this->currentPosition = 0;
}


void SortedSetIterator::first() {
    this->currentPosition = 0;
}


void SortedSetIterator::next() {
    if (this->valid())
        this->currentPosition++;
    else
        throw exception();
}


TElem SortedSetIterator::getCurrent()
{
    if (this->currentPosition == this->multime.setSize)
        throw exception();

	return this->multime.elements[this->currentPosition];
}

bool SortedSetIterator::valid() const {
	return this->currentPosition < this->multime.setSize;
}

