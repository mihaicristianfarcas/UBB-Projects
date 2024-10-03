#pragma once
#include <utility>
#include<vector>
#include<utility>
//DO NOT INCLUDE MultiMapIterator

using namespace std;

//DO NOT CHANGE THIS PART
typedef int TKey;
typedef int TValue;
typedef std::pair<TKey, TValue> TElem;
#define NULL_TVALUE -111111
#define NULL_TELEM pair<int,int>(-111111, -111111)
class MultiMapIterator;


struct DLLANode {
    TElem data;
    int prev, next;

    explicit DLLANode(TElem pair) : data(std::move(pair)), prev(-1), next(-1) {}

    explicit DLLANode() : data(NULL_TELEM), prev(-1), next(-1) {}
};


class MultiMap {
	friend class MultiMapIterator;

private:
    DLLANode* nodes;
    int capacity, head, tail, firstEmpty, listSize;

    int allocate();

    void free(int pos);

    void resize();

public:
	//constructor
	MultiMap();

	//adds a key value pair to the multimap
	void add(TKey c, TValue v);

    //adds all pairs from the multimap, whose key is not in the multimap already
    //returns the number of added pairs
    int addIfNotPresent(MultiMap &m);

    //returns true if the key c exists in the multimap, false otherwise
    bool isKeyInMap(TKey c);

	//removes a key value pair from the multimap
	//returns true if the pair was removed (if it was in the multimap) and false otherwise
	bool remove(TKey c, TValue v);

	//returns the vector of values associated to a key. If the key is not in the MultiMap, the vector is empty
	vector<TValue> search(TKey c) const;

	//returns the number of pairs from the multimap
	int size() const;

	//checks whether the multimap is empty
	bool isEmpty() const;

	//returns an iterator for the multimap
	MultiMapIterator iterator() const;

	//descturctor
	~MultiMap();
};

