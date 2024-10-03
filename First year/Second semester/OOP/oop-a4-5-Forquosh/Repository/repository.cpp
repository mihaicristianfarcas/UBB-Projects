////
//// Created by mihai on 23.03.2024.
////
//
//#include "repository.h"
//#include <algorithm>
//
///// Constructor of the Repository
//template <typename T>
//Repository<T>::Repository(int capacity) {
//    this->capacity = capacity;
//    this->size = 0;
//    this->elements = new T[this->capacity];
//    Repository<T>::defaultElements();
//}
//
//template <typename T>
//void Repository<T>::defaultElements() {
//    T element;
//
//    element = Dog("Golden Retriever", "Bella", 2, "https://www.purina.ro/sites/default/files/2023-05/BREED_golden_retriever.jpg");
//    addElement(element);
//    element = Dog("Labrador Retriever", "Aky", 3, "https://www.vidavetcare.com/wp-content/uploads/sites/234/2022/04/labrador-retriever-dog-breed-info.jpeg");
//    addElement(element);
//    element = Dog("German Shepherd", "Ares", 5, "https://beyondthedogtraining.com/wp-content/uploads/german-shepherd-dog-768x512.jpg");
//    addElement(element);
//    element = Dog("Yorkshire Terrier", "Cupcake", 1, "https://cdn.sanity.io/images/4ij0poqn/production/d23ed7c34d2d78b5af0050c5302e04e9c8bc048a-1200x700.jpg");
//    addElement(element);
//    element = Dog("Poodle", "Lady", 4, "https://image.petmd.com/files/styles/978x550/public/2023-01/toy-poodle.jpg");
//    addElement(element);
//    element = Dog("Bulldog", "Leo", 6, "https://dogtime.com/wp-content/uploads/sites/12/2011/01/GettyImages-168620477-e1691273341205.jpg?w=1024");
//    addElement(element);
//    element = Dog("Doberman", "Max", 8, "https://www.animaland.ro/wp-content/uploads/2022/04/Dobernam-closeup-620x800.jpg");
//    addElement(element);
//    element = Dog("Rottweiler", "Rex", 2, "https://gomagcdn.ro/domains2/epetshop.ro/files/files/caine-rotweiler-656483.webp");
//    addElement(element);
//    element = Dog("Beagle", "Puffy", 3, "https://www.zooplus.ro/ghid/wp-content/uploads/2021/07/caine-beagle.webp");
//    addElement(element);
//    element = Dog("Pekingese", "Jessie", 8, "https://www.purina.co.uk/sites/default/files/2022-07/Pekingese.jpg?itok=lUhQR_w1");
//    addElement(element);
//}
//
///// Destructor of the repository
//template <typename T>
//Repository<T>::~Repository() {
//    delete[] elements;
//}
//
///// Adds a new dog to the array
//template <typename T>
//void Repository<T>::addElement(const T& element) {
//
//    if (this->size == this->capacity) {
//        this->capacity *= 2;
//        T* temp = new T[capacity];
//        std::copy(this->elements, this->elements + this->size, temp);
//        delete[] this->elements;
//        this->elements = temp;
//    }
//    this->elements[this->size++] = element;
//}
//
///// Removes a dog from the array
//template <typename T>
//void Repository<T>::removeElement(const std::string& name) {
//    for (int i = 0; i < this->size; i++) {
//        if (this->elements[i].getName() == name) {
//            for (int j = i; j < this->size - 1; j++) {
//                this->elements[j] = this->elements[j + 1];
//            }
//            this->size--;
//            break;
//        }
//    }
//}
//
///// Get the length of the array
//template <typename T>
//int Repository<T>::getLength() const { return this->size; }
//
///// Get the dog positioned at a specific index
//template <typename T>
//T& Repository<T>::getElementAt(int index) { return this->elements[index]; }
//
///// Get all elements in the vector
//template <typename T>
//T* Repository<T>::getAllElements() const { return this->elements; }
