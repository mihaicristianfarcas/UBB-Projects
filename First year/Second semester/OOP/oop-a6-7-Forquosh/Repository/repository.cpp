//
// Created by mihai on 15.04.2024.
//
#include "repository.h"

// Constructor of the Repository
Repository::Repository() {
    Repository::loadFromFile(textFile);
}

// Destructor of the repository
Repository::~Repository() = default;

// Adds a new dog to the vector
void Repository::addDog(const Dog& dog) {
    this->dogs.push_back(dog);
    this->saveToFile(this->fileName);
}

// Removes a dog from the vector
void Repository::removeDog(const std::string& name) {
    auto it = std::find_if(this->dogs.begin(), this->dogs.end(),
                           [&name](const Dog& dog) { return dog.getName() == name; });
    if (it != this->dogs.end()) {
        this->dogs.erase(it);
    }
    this->saveToFile(this->fileName);
}

// Get the length of the vector
unsigned long long Repository::getLength() const { return this->dogs.size(); }

// Get the dog positioned at a specific index
Dog& Repository::getDogAt(int index) { return this->dogs[index]; }

Dog* Repository::getDogByName(const std::string& name) {
    for (auto& dog : this->dogs) {
        if (dog.getName() == name) {
            return &dog;
        }
    }
    return nullptr;
}

const std::vector<Dog>& Repository::getAllDogs() const { return this->dogs; }

// Loads the repository data from a text file
void Repository::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    Dog dog;
    while (file >> dog) {
        this->dogs.push_back(dog);
    }

    file.close();
}

// Saves the repository data to a file
void Repository::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    for (const Dog& dog : this->dogs) {
        file << dog << std::endl;
    }

    file.close();
}

// INITIAL DOGS

//Golden Retriever,Bella,2,https://www.purina.ro/sites/default/files/2023-05/BREED_golden_retriever.jpg
//Labrador Retriever,Aky,3,https://www.vidavetcare.com/wp-content/uploads/sites/234/2022/04/labrador-retriever-dog-breed-info.jpeg
//German Shepherd,Ares,5,https://beyondthedogtraining.com/wp-content/uploads/german-shepherd-dog-768x512.jpg
//Yorkshire Terrier,Cupcake,1,https://cdn.sanity.io/images/4ij0poqn/production/d23ed7c34d2d78b5af0050c5302e04e9c8bc048a-1200x700.jpg
//Poodle,Lady,4,https://image.petmd.com/files/styles/978x550/public/2023-01/toy-poodle.jpg
//Bulldog,Leo,6,https://dogtime.com/wp-content/uploads/sites/12/2011/01/GettyImages-168620477-e1691273341205.jpg?w=1024
//Doberman,Max,8,https://www.animaland.ro/wp-content/uploads/2022/04/Dobernam-closeup-620x800.jpg
//Rottweiler,Rex,2,https://gomagcdn.ro/domains2/epetshop.ro/files/files/caine-rotweiler-656483.webp
//Beagle,Puffy,3,https://www.zooplus.ro/ghid/wp-content/uploads/2021/07/caine-beagle.webp
//Pekingese,Jessie,8,https://www.purina.co.uk/sites/default/files/2022-07/Pekingese.jpg?itok=lUhQR_w1
