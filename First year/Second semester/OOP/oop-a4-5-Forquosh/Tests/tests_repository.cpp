//
// Created by mihai on 26.03.2024.
//
#include "tests.h"

void testAddElement() {
    Repository<Dog> repository(5);
    Dog dog1("Labrador", "Buddy", 3, "url0");
    Dog dog2("Poodle", "Max", 5, "url0");

    repository.addElement(dog1);
    repository.addElement(dog2);

    assert(repository.getLength() == 12);
    assert(repository.getElementAt(10).getName() == "Buddy");
    assert(repository.getElementAt(11).getName() == "Max");
}

void testRemoveElement() {
    Repository<Dog> repository(5);
    Dog dog1("Labrador", "Buddy", 3, "url0");
    Dog dog2("Poodle", "Max", 5, "url0");

    repository.addElement(dog1);
    repository.addElement(dog2);

    repository.removeElement("Buddy");

    assert(repository.getLength() == 11);
    assert(repository.getElementAt(10).getName() == "Max");
}

void testGetLength() {
    Repository<Dog> repository(5);
    assert(repository.getLength() == 10);
}


void testGetElementAt() {
    Repository<Dog> repository(5);
    Dog dog1("Labrador", "Buddy", 3, "url0");
    Dog dog2("Poodle", "Max", 5, "url0");

    repository.addElement(dog1);
    repository.addElement(dog2);

    assert(repository.getElementAt(10).getName() == dog1.getName());
    assert(repository.getElementAt(11).getName() == dog2.getName());

}

void testGetAllElements() {
    Repository<Dog> repository(5);
    Dog* elements = repository.getAllElements();

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

void testRepository() {
    testAddElement();
    testRemoveElement();
    testGetLength();
    testGetElementAt();
    testGetAllElements();
}