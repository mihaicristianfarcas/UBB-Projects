////
//// Created by mihai on 26.03.2024.
////
//#include "tests.h"
//
//void testAddElement() {
//    Repository repository;
//    Dog dog1("Labrador", "Buddy", 3, "url0");
//    Dog dog2("Poodle", "Max", 5, "url0");
//
//    repository.addDog(dog1);
//    repository.addDog(dog2);
//
//    assert(repository.getLength() == 12);
//    assert(repository.getDogAt(10).getName() == "Buddy");
//    assert(repository.getDogAt(11).getName() == "Max");
//}
//
//void testRemoveElement() {
//    Repository repository;
//
//    repository.removeDog("Buddy");
//    repository.removeDog("Max");
//
//    assert(repository.getLength() == 10);
//}
//
//void testGetLength() {
//    Repository repository;
//    assert(repository.getLength() == 10);
//}
//
//
//void testGetElementAt() {
//    Repository repository;
//    Dog dog1("Labrador", "Buddy", 3, "url0");
//
//    repository.addDog(dog1);
//
//    assert(repository.getDogAt(10).getName() == dog1.getName());
//
//    repository.removeDog("Buddy");
//
//}
//
//void testGetAllElements() {
//    Repository repository;
//    auto& elements = repository.getAllDogs();
//
//    assert(repository.getDogAt(0).getName() == elements[0].getName());
//    assert(repository.getDogAt(1).getName() == elements[1].getName());
//    assert(repository.getDogAt(2).getName() == elements[2].getName());
//    assert(repository.getDogAt(3).getName() == elements[3].getName());
//    assert(repository.getDogAt(4).getName() == elements[4].getName());
//    assert(repository.getDogAt(5).getName() == elements[5].getName());
//    assert(repository.getDogAt(6).getName() == elements[6].getName());
//    assert(repository.getDogAt(7).getName() == elements[7].getName());
//    assert(repository.getDogAt(8).getName() == elements[8].getName());
//    assert(repository.getDogAt(9).getName() == elements[9].getName());
//
//}
//
//void testRepository() {
//    testAddElement();
//    testRemoveElement();
//    testGetLength();
//    testGetElementAt();
//    testGetAllElements();
//}