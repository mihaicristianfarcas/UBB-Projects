//
// Created by mihai on 26.03.2024.
//
#include "tests.h"

void testAddDogService() {
    Repository<Dog> repository(5);
    Administrator administrator(repository);

    std::string breed = "Labrador";
    std::string name = "Buddy";
    int age = 3;
    std::string photograph = "url0";

    bool added = administrator.addDog(breed, name, age, photograph);
    assert(added);

    added = administrator.addDog(breed, name, age, photograph);
    assert(added == false);

    assert(administrator.getRepoSize() == 11);
    assert(administrator.viewAllDogs()[10].getName() == "Buddy");
}

void testDeleteDogService() {
    Repository<Dog> repository(5);
    Administrator administrator(repository);

    std::string breed = "Labrador";
    std::string name = "Buddy";
    int age = 3;
    std::string photograph = "url0";

    administrator.addDog(breed, name, age, photograph);

    bool deleted = administrator.deleteDog(name);

    assert(deleted);
    assert(administrator.getRepoSize() == 10);
}

void testUpdateDogService() {
    Repository<Dog> repository(5);
    Administrator administrator(repository);

    std::string breed = "Labrador";
    std::string name = "Buddy";
    int age = 3;
    std::string photograph = "labrador.jpg";

    administrator.addDog(breed, name, age, photograph);

    bool updated = administrator.updateDog(name, 4);

    assert(updated);
    assert(administrator.viewAllDogs()[10].getAge() == 4);
}

void testServicesAdmin() {
    testAddDogService();
    testDeleteDogService();
    testUpdateDogService();
}