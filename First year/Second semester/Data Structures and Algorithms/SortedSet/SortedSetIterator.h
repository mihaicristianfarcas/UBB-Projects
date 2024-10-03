#pragma once
#include "SortedSet.h"

//DO NOT CHANGE THIS PART
class SortedSetIterator
{
	friend class SortedSet;
private:
	const SortedSet& multime;
    int currentPosition = 0;

	explicit SortedSetIterator(const SortedSet& m);

public:
	void first();
	void next();
	TElem getCurrent();
	bool valid() const;
};

