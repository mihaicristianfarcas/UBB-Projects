#include "SortedSet.h"
#include "SortedSetIterator.h"

SortedSet::SortedSet(Relation r) {
    this->setSize = 0;
    this->capacity = 100;
    this->relation = r;
    this->elements = new TComp[capacity];
}

bool SortedSet::add(TComp elem) {

    if (this->setSize == this->capacity)
        this->resize();

    if (this->setSize == 0) {
        this->elements[this->setSize++] = elem;
        return true;
    }

    int index = 0;
    while (index < this->setSize && elem != this->elements[index] && !relation(elem, this->elements[index]))
        index++;

    if (index == this->setSize) {
        this->elements[this->setSize++] = elem;
        return true;
    }

    if (elem == this->elements[index])
        return false;

    if (relation(elem, this->elements[index])) {
        this->setSize++;
        for (int i = setSize - 1; i > index; i--)
            this->elements[i] = this->elements[i - 1];

        this->elements[index] = elem;
        return true;
    }

    return false;
}

void SortedSet::resize() {
    this->capacity *= 2;
    auto* aux = new TComp[capacity];
    for (int i = 0; i < this->setSize; i++) {
        aux[i] = this->elements[i];
    }
    delete[] this->elements;
    this->elements = aux;
}

bool SortedSet::remove(TComp elem) {
    int index = 0;
    while (index < this->setSize) {
        if (this->elements[index] == elem) {
            for (int i = index; i < this->setSize - 1 ; i++) {
                this->elements[i] = this->elements[i + 1];
            }
        this->setSize--;
        return true;
        }
    index++;
    }

    return false;
}

void SortedSet::setUnion(const SortedSet& s) {
    int i = 0, j = 0;
    SortedSet res = SortedSet(this->relation);

    while(i < this->setSize && j < s.setSize) {
        if (res.setSize == res.capacity)
            resize();

        if(relation(this->elements[i], s.elements[j]))
            res.elements[res.setSize++] = this->elements[i++];
        else
            res.elements[res.setSize++] = s.elements[j++];
    }
    while(i < this->setSize) {
        if (res.setSize == res.capacity)
            resize();

        res.elements[res.setSize++] = this->elements[i++];
    }

    while(j < s.setSize) {
        if (res.setSize == res.capacity)
            resize();

        res.elements[res.setSize++] = s.elements[j++];
    }

    // TODO this fails?
    delete [] this->elements;
    this->elements = new TComp[this->capacity];
    this->setSize = 0;
    for(int k = 0; k < res.setSize; k++) {
        if (this->setSize == this->capacity)
            resize();
        this->elements[k] = res.elements[k];
        this->setSize++;
    }

}

bool SortedSet::search(TComp elem) const {
	int index = 0;
    while (index < this->setSize) {
        if (this->elements[index] == elem)
            return true;
    index++;
    }

	return false;
}


int SortedSet::size() const {
	return this->setSize;
}



bool SortedSet::isEmpty() const {
	return this->setSize == 0;
}

SortedSetIterator SortedSet::iterator() const {
	return SortedSetIterator(*this);
}


SortedSet::~SortedSet() {
	delete[] this->elements;
}


