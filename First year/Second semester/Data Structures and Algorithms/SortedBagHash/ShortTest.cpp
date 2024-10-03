#include "ShortTest.h"
#include "SortedBag.h"
#include "SortedBagIterator.h"
#include <assert.h>

bool relation1(TComp e1, TComp e2) {
	return e1 <= e2;
}

void testElementsWithMinimumFrequency() {
    SortedBag bag(relation1);

    assert(bag.elementsWithMinimumFrequency() == 0);

    bag.add(5);
    bag.add(3);
    bag.add(5);
    bag.add(7);
    bag.add(3);
    bag.add(3);

    // Bag: 3 (3 times), 5 (2 times), 7 (1 time)
    assert(bag.elementsWithMinimumFrequency() == 1); // Only 7 appears once

    bag.add(7); // Now 7 appears twice
    assert(bag.elementsWithMinimumFrequency() == 2); // 5 and 7 both appear twice

    bag.add(5); // Now 5 appears 3 times
    assert(bag.elementsWithMinimumFrequency() == 1); // Only 7 appears twice

    bag.add(7); // Now 7 appears 3 times
    assert(bag.elementsWithMinimumFrequency() == 3);
}


void testAll() {
	SortedBag sb(relation1);
	sb.add(5);
	sb.add(6);
	sb.add(0);
	sb.add(5);
	sb.add(10);
	sb.add(8);

	assert(sb.size() == 6);
	assert(sb.nrOccurrences(5) == 2);

	assert(sb.remove(5) == true);
	assert(sb.size() == 5);

	assert(sb.search(6) == true);
	assert(sb.isEmpty() == false);

	SortedBagIterator it = sb.iterator();
	assert(it.valid() == true);
	while (it.valid()) {
		it.getCurrent();
		it.next();
	}
	assert(it.valid() == false);
	it.first();
	assert(it.valid() == true);

    testElementsWithMinimumFrequency();
}