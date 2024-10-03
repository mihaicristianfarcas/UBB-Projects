#pragma once
#include <vector>
#include <fstream>
#include <algorithm>
#include "../domain/Dog.h"

class Repository {

protected:
    std::vector<Dog> dogs;
    std::string textFile = "../dogs.txt";
    std::string fileName;

    void loadFromFile(const std::string& filename);

public:

    explicit Repository();

    virtual ~Repository();

    void addDog(const Dog& dog);

    void removeDog(const std::string &name); // Assuming name is unique

    unsigned long long getLength() const;

    Dog& getDogAt(int index);

    Dog* getDogByName(const std::string& name);

    const std::vector<Dog>& getAllDogs() const;

    virtual void saveToFile(const std::string& filename) const = 0;
};

class CSVFileRepository : public Repository {

public:

    explicit CSVFileRepository(const std::string &filename) {
        fileName = filename;
        CSVFileRepository::saveToFile(fileName);
    }

    // Saves the repository data to a CSV file
    void saveToFile(const std::string& filename) const override {
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
};


class HTMLFileRepository : public Repository {

public:

    explicit HTMLFileRepository(const std::string &filename) {
        fileName = filename;
        HTMLFileRepository::saveToFile(fileName);
    }

    // Saves the repository data to an HTML file as a table
    void saveToFile(const std::string& filename) const override {

        std::ofstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        file << "<!DOCTYPE html>\n"
             << "<html>\n"
             << "<head>\n"
             << "<title>Dogs</title>\n"
             << "</head>\n"
             << "<body>\n"
             << "<table border=\"1\">\n"
             << "<tr>\n"
             << "<td>Breed</td>\n"
             << "<td>Name</td>\n"
             << "<td>Age</td>\n"
             << "<td>Picture</td>\n"
             << "</tr>\n";

        for (const auto& dog : dogs) {
            file << "<tr>\n"
                 << "<td>" << dog.getBreed() << "</td>\n"
                 << "<td>" << dog.getName() << "</td>\n"
                 << "<td>" << dog.getAge() << "</td>\n"
                 << "<td><a href=\"" << dog.getPhotograph() << "\">Link</a></td>\n"
                 << "</tr>\n";
        }

        file << "</table>\n"
             << "</body>\n"
             << "</html>\n";

        file.close();
    }
};

