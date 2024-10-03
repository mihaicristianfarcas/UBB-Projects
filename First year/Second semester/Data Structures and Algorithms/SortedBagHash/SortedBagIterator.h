#pragma once
#include "SortedBag.h"

class SortedBag;

class SortedBagIterator
{
    friend class SortedBag;

private:
    const SortedBag& bag;
    SortedBagIterator(const SortedBag& b);

    int currentPosition;
    struct Node* currentElement;
    TComp* copyElements;
    int copyCurrentPosition;

public:
    TComp getCurrent();
    bool valid();
    void next();
    void first();
};