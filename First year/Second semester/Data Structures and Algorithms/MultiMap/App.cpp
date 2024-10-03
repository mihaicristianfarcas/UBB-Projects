#include <iostream>
#include "MultiMap.h"
#include "ExtendedTest.h"
#include "ShortTest.h"
#include "MultiMapIterator.h"

using namespace std;

//ADT MultiMap – using a DLLA with (key, value) pairs. A key can appear in multiple pairs.
//Pairs do not have to be ordered.

int main() {

	testAll();
	testAllExtended();
	cout << "End" << endl;
	system("pause");

}
