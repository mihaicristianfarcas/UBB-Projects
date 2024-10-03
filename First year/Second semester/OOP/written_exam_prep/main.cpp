#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>

using namespace std;

class A {
private:
    int* x;

public:

    static int nr;
    A(int _x = 5) {
        x = new int{ _x };
        nr++;
    }
    int get() { return *x; }

    void set(int _x) { *x = _x;}
    ~A() { delete x; }
};

int A::nr = 0;

int main() {
    A a1, a2;
    cout << a1.nr << " ";
    A a3 = a1;
    cout << A::nr << " ";
    a1.set(10);
    cout << a1.get() << " ";
    cout << a2.get() << " ";
    cout << a3.get() << " ";
    std::cout << "Hello, World!" << std::endl;
    return 0;
}

#include <iostream>

using namespace std;

class B {
public:
    B() {}
    B(const B&) { cout << "copy "; }
    virtual B f() {
        cout << "B.f ";
        return *this;
    }
    virtual ~B() { cout << "~B "; }
};

class D : public B {
private:
    B* b;

public:
    D(B* _b) : b{_b} { cout << "D "; }
    B f() override {
        cout << "D.f ";
        return b->f();
    }
};

int main() {
    B* b = new B();
    B* d = new D{b};
    d->f();
    delete d;
    delete b;
    return 0;
}

// D D.f B.f copy ~B ~B ~B
// return *this -> creates new B by copy constr. and deletes it right after since not used

#include <iostream>

using namespace std;

int except(int v) {
    if (v < 0) {
        throw string("1 ");
    } else if (v > 0) {
        throw runtime_error("2 ");
    }
    return 0;
}

int main() {
    try {
        cout << except(10 < 2) << " ";
        cout << except(-2) << " ";
        cout << except(2) << " ";
    } catch (runtime_error& e) {
        cout << e.what();
    } catch (string x) {
        cout << x;
    }
    cout << "Done ";
    return 0;
}


#include <deque>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    vector<int> v{10, 9, 8, 7, 6, 5};
    auto it = v.begin() + 2;
    v.erase(it);
    it = v.begin() + 3;
    *it = 11;
    it++;
    *it = 1;
    deque<int> x;
    copy_if(v.begin(), v.end(), front_inserter(x), [](int a) { return a % 2 == 1; });
    for (auto a : x) {
        cout << a << " ";
    }
    return 0;
}

// 10 9 7 11 1 -> 1 11 7 9


class B {
public:
    B() { std::cout << "B{}"; }
    virtual void print() { std::cout <<
                                     "B"; }
    virtual ~B() { std::cout << "~B()"; }
};

class D : public B {
public:
    D() { std::cout << "D{}"; }
    void print() override { std::cout <<
                                      "D"; }
    virtual ~D() { std::cout << "~D()"; }
};

int main()
{
    B* b[] = { new B{}, new D{} };
    b[0]->print();
    b[1]->print();
    delete b[0];
    delete b[1];
    return 0;
}

// B{} B{} D{} B D ~B() ~D() ~B()

class Person
{
public:
    Person() { std::cout << "Person{}"; }
    virtual void print() = 0;
    virtual ~Person() { std::cout <<
                                  "~Person()"; }
};
class Student : public Person
{
public:
    Student() { std::cout << "Student{}";
    }
    void print() override { std::cout <<
                                      "Student"; }
    virtual ~Student() { std::cout <<
                                   "~Student()"; }
};

int main()
{
//    Person* p = new Person{};
//    delete p;
    Person* s = new Student{};
    s->print();
    delete s;
    return 0;
}

//  can't allocate an object of abstract type Person
// Person{} Student{} Student ~Student{} ~Person{}


class E
{
public:
    E() { std::cout << "E{}"; }
    virtual ~E() { std::cout << "~E()"; }
};
class DE : public E
{
public:
    static int n;
    DE() { n++; }
};
int DE::n = 0;
int fct2(int x)
{
    if (x < 0)
    {
        throw E{};
        std::cout << "number less than 0" << std::endl;
    }
    else if (x == 0)
    {
        throw DE{};
        std::cout << "number equal to 0"
                  << std::endl;
    }
    return x % 10;
}

int main()
{
    try
    {
        int res = 0;
        res = fct2(-5);
        std::cout << DE::n;
        res = fct2(0);
        std::cout << DE::n;
        res = fct2(25);
        std::cout << DE::n;
    }
    catch (E&)
    {
        std::cout << DE::n;
    }
    return 0;
}

// E{} 0 ~E{} !! destroys after catch and it's block's execution!!


class A {
private:
    int x;
public:
    A(int _x = 0) : x{_x} {}
    int get() const {return x;};
    friend class B;
};

class B {
private:
    A a;
public:
    B(const A& _a) : a{_a} {}
    B& operator+(const A& _a) {
        a.x += _a.x;
        return *this;
    }
    int get() {return a.x;}
};

int main() {
    A a1{1}, a2{2};
    cout << a1.get() << " " << a2.get() << " ";
    B b1{a1};
    B b2 = b1 + a2 + a1;
    cout << b1.get() << " " << b2.get() << " ";
    return 0;
}

// 1 2 1 4 X -> 1 2 4 4 b1 gets modified and returned to b2, which basically calls copy constructor


int main() {
    vector<string> v{"rain", "in", "spain", "falls", "mainly", "plain"};
    sort(v.begin(), v.end(), [](string a, string b) { return a > b; });
    vector<string>::iterator it = v.begin() + 4;
    *it = "brain";
    it = v.begin();
    while(it != v.end()) {
        string aux = *it;
        char c1 = aux[aux.size() - 3];
        char c2 = aux[aux.size() - 2];
        char c3 = aux[aux.size() - 1];
        if(c1 == 'a' && c2 == 'i' && c3 == 'n')
            cout << *it << '\n';
        it++;
    }
    return 0;
}

// spain rain plain mainly in falls -> spain rain plain mainly brain falls
// spain rain plain brain

#include <string>
#include <unordered_map>

using namespace std;

class Person {
    int age;
    string name;
public:
    Person(int age, string name) : age{age}, name{name} {}
    friend ostream& operator<<(ostream& os, Person& p) {
        os << p.name << " is " << p.age << " years old";
        return os;
    }
};

template<typename T, typename A>
class MultiDictionary {
private:
    unordered_multimap<T, A> M;
public:
    MultiDictionary() {}

    MultiDictionary& add(T k, A v) {
        M.insert({k, v});
        return *this;
    }

    void print(ostream& os) {
        for (const auto& e : M) {
            os << e.first << " " << e.second << "; ";
        }
    }

    void erase(T k, A v) {
        auto range = M.equal_range(k);
        for (auto it = range.first; it != range.second; ++it) {
            if (it->second == v) {
                M.erase(it);
                return;
            }
        }
        throw runtime_error("Given value does not exist for given key!");
    }

    int sizeForKey(T k) {
        return M.count(k);
    }
};

int main()
{
    MultiDictionary<int, string> d1{};
    d1.add(1, "a").add(2, "b").add(1, "c").add(3, "d");
    d1.print(cout);
    cout << endl;

    try {
        d1.erase(1, "c");
        d1.print(cout);
        cout << endl;

        cout << "Size for key 1: " << d1.sizeForKey(1) << endl;
    } catch (const exception& e) {
        cout << e.what() << endl;
    }

    return 0;
}
