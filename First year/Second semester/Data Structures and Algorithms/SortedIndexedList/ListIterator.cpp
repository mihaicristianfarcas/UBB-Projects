#include "ListIterator.h"
#include "SortedIndexedList.h"
#include <iostream>

using namespace std;

ListIterator::ListIterator(const SortedIndexedList& list) : list(list), current(list.head) {}

void ListIterator::first(){
	current = list.head;
}

void ListIterator::next(){
    if (!valid())
        throw std::runtime_error("Iterator is not valid");

    current = current->next;
}

bool ListIterator::valid() const{
	return current != nullptr;
}

TComp ListIterator::getCurrent() const{
    if (!valid())
        throw std::runtime_error("Iterator is not valid");

	return current->data;
}


