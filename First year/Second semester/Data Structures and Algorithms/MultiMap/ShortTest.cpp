#include "ShortTest.h"
#include "MultiMap.h"
#include "MultiMapIterator.h"
#include <assert.h>
#include <vector>
#include<iostream>

void testAll() {
	MultiMap m;
	m.add(1, 100);
	m.add(2, 200);
	m.add(3, 300);
	m.add(1, 500);
	m.add(2, 600);
	m.add(4, 800);

	assert(m.size() == 6);

	assert(m.remove(5, 600) == false);
	assert(m.remove(1, 500) == true);

	assert(m.size() == 5);

    vector<TValue> v;
	v=m.search(6);
	assert(v.size()==0);

	v=m.search(1);
	assert(v.size()==1);

	assert(m.isEmpty() == false);

	MultiMapIterator im = m.iterator();
	assert(im.valid() == true);
	while (im.valid()) {
		im.getCurrent();
		im.next();
	}
	assert(im.valid() == false);
	im.first();
	assert(im.valid() == true);


    // addIfNotPresent() TESTS
    MultiMap m1;
    m1.add(1, 100);
    m1.add(2, 200);
    m1.add(3, 300);
    m1.add(1, 500);
    m1.add(2, 600);

    MultiMap m2;
    m2.add(4, 800);
    m2.add(2, 300); // won't be added
    m2.add(4, 900);
    m2.add(5, 200);
    m2.add(3, 700); // won't be added

    int nrPairs = m1.addIfNotPresent(m2);
    assert(nrPairs == 3);
    assert(m1.size() == 8);

    nrPairs = m2.addIfNotPresent(m1);
    assert(nrPairs == 2);
}
