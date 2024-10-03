#include <assert.h>
#include "Matrix.h"

using namespace std;

void testAll() { 
	Matrix m(4, 4);
	assert(m.nrLines() == 4);
	assert(m.nrColumns() == 4);	
	m.modify(1, 1, 5);
	assert(m.element(1, 1) == 5);
	TElem old = m.modify(1, 1, 6);
	assert(m.element(1, 2) == NULL_TELEM);
	assert(old == 5);

    // setMainDiagonal tests:
    try {
        Matrix m2(1, 3);
        m2.setMainDiagonal(1);
        assert(false);
    }
    catch (out_of_range&) {
        assert(true);
    }

    Matrix m3(3, 3);
    m3.setMainDiagonal(1);
    assert(m3.element(0,0) == 1);
    assert(m3.element(1,1) == 1);
    assert(m3.element(2,2) == 1);
}