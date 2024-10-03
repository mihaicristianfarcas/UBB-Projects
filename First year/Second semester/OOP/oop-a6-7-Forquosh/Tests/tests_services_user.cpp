////
//// Created by mihai on 28.03.2024.
////
//#include "tests.h"
//
//void test_adoptDog() {
//    Repository repository;
//    User user(repository);
//
//    repository.addDog(Dog("Golden Retriever", "Rex", 2, "url1"));
//    repository.addDog(Dog("Labrador Retriever", "Aky", 3, "url2"));
//
//    assert(user.adoptDog("Rex") == true);
//    assert(user.adoptDog("Aky") == true);
//
//    assert(user.adoptDog("Rex") == false);
//    assert(user.adoptDog("abc") == false);
//
//    assert(user.adoptDog("Max") == true);
//}
//
//void test_seeAdoptionList() {
//    Repository repository;
//    User user(repository);
//
//    repository.addDog(Dog("Golden Retriever", "Rex", 2, "url1"));
//    repository.addDog(Dog("Labrador Retriever", "Aky", 3, "url2"));
//
//    user.adoptDog("Rex");
//    user.adoptDog("Aky");
//
//    const std::vector<Dog>& adoptionList = user.seeAdoptionList();
//    assert(adoptionList.size() == 2);
//    assert(adoptionList[0].getName() == "Rex");
//    assert(adoptionList[1].getName() == "Aky");
//}
//
//
//void testViewAllDogs() {
//    Repository repository;
//    User user(repository);
//
//    auto elements = user.viewAllDogs();
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
//void testServicesUser() {
//    test_adoptDog();
//    test_seeAdoptionList();
//    testViewAllDogs();
//}
