//
// Created by mihai on 28.03.2024.
//
#include "tests.h"

void test_adoptDog() {
    Repository<Dog> repository(10);
    User user(repository);

    repository.addElement(Dog("Golden Retriever", "Bella", 2, "url1"));
    repository.addElement(Dog("Labrador Retriever", "Aky", 3, "url2"));

    assert(user.adoptDog("Bella") == true);
    assert(user.adoptDog("Aky") == true);

    assert(user.adoptDog("Bella") == false);
    assert(user.adoptDog("abc") == false);

    assert(user.adoptDog("Max") == true);
}

void test_seeAdoptionList() {
    Repository<Dog> repository(10);
    User user(repository);

    repository.addElement(Dog("Golden Retriever", "Bella", 2, "url1"));
    repository.addElement(Dog("Labrador Retriever", "Aky", 3, "url2"));

    user.adoptDog("Bella");
    user.adoptDog("Aky");

    const std::vector<Dog>& adoptionList = user.seeAdoptionList();
    assert(adoptionList.size() == 2);
    assert(adoptionList[0].getName() == "Bella");
    assert(adoptionList[1].getName() == "Aky");
}

void testGetRepoSize() {
    Repository<Dog> repository(10);
    User user(repository);

    assert(user.getRepoSize() == 10);
    repository.addElement(Dog("Golden Retriever", "Bella", 2, "url1"));
    assert(user.getRepoSize() == 11);

}

void testViewAllDogs() {
    Repository<Dog> repository(10);
    User user(repository);

    Dog* elements = user.viewAllDogs();

    assert(repository.getElementAt(0).getName() == elements[0].getName());
    assert(repository.getElementAt(1).getName() == elements[1].getName());
    assert(repository.getElementAt(2).getName() == elements[2].getName());
    assert(repository.getElementAt(3).getName() == elements[3].getName());
    assert(repository.getElementAt(4).getName() == elements[4].getName());
    assert(repository.getElementAt(5).getName() == elements[5].getName());
    assert(repository.getElementAt(6).getName() == elements[6].getName());
    assert(repository.getElementAt(7).getName() == elements[7].getName());
    assert(repository.getElementAt(8).getName() == elements[8].getName());
    assert(repository.getElementAt(9).getName() == elements[9].getName());

}

void testServicesUser() {
    test_adoptDog();
    test_seeAdoptionList();
    testGetRepoSize();
    testViewAllDogs();
}
