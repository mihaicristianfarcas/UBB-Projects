#pragma once
//DO NOT INCLUDE SORTEDBAGITERATOR

//DO NOT CHANGE THIS PART
typedef int TComp;
typedef TComp TElem;
typedef bool(*Relation)(TComp, TComp);
#define NULL_TCOMP -11111;

typedef TComp (*TFunction)(TComp);

struct Node {
    TElem element;
    int frequency;
    struct Node* next;
};

class SortedBagIterator;

class SortedBag {
    friend class SortedBagIterator;

private:
    struct Node** hashTable;
    int m;
    [[nodiscard]] TComp h (TComp) const;
    int bagSize;
    Relation relation;

public:
    //constructor
    SortedBag(Relation r);

    //adds an element to the sorted bag
    void add(TComp e);

    //removes one occurence of an element from a sorted bag
    //returns true if an eleent was removed, false otherwise (if e was not part of the sorted bag)
    bool remove(TComp e);

    //checks if an element appears in the sorted bag
    [[nodiscard]] bool search(TComp e) const;

    //returns the number of occurrences for an element in the sorted bag
    [[nodiscard]] int nrOccurrences(TComp e) const;

    //returns the number of elements from the sorted bag
    [[nodiscard]] int size() const;

    //returns an iterator for this sorted bag
    SortedBagIterator iterator() const;

    //checks if the sorted bag is empty
    [[nodiscard]] bool isEmpty() const;

    void resize();

    [[nodiscard]] int elementsWithMinimumFrequency() const;

    //destructor
    ~SortedBag();
};