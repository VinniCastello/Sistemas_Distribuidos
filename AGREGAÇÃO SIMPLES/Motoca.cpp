#include <iostream>
#include <sstream>
#include <memory>
#include <aux.hpp>
using namespace std;

class Person {
    string name;
    int age;
public:
    Person(string name = "", int age = 0) : name(name), age(age) {}

    // Person(int age) : Person("", age) {}

    int getAge() {
        return this->age;
    }
    string getName() {
        return this->name;
    }
    string str() {
        ostringstream oss;
        oss << name << ":" << age;
        return oss.str();
    }
};

ostream& operator<<(ostream& os, Person& p) {
    return os << p.str();
}

class Motorcycle {
    shared_ptr<Person> person {nullptr};
    int tempo {0};
    int power {1};

public:
    Motorcycle(int power = 1) : power(power) {}

    bool insertPerson(shared_ptr<Person> p) {
        if(this->person != nullptr){
            cout << "fail: busy motorcycle" << endl;
            return false;
        }
        this->person = p;
        return true;
    }

    string honk() {
        return "P" + string(power, 'e') + "m";
    }

    shared_ptr<Person> remove() {
        if(this->person == nullptr){
            cout << "fail: empty motorcycle" << endl;
            return nullptr;
        }
        cout << this->person->str() << endl;
        this->person = nullptr;
        return nullptr;
    }

    void buyTime(int time) {
        this->tempo += time;
    }

    void drive(int time) {
        if(this->tempo == 0){
            cout << "fail: buy time first" << endl;
            return;
        }
        if(this->person == nullptr){
            cout << "fail: empty motorcycle" << endl;
            return;
        }
        if(this->person != nullptr && this->person->getAge() > 10) {
            cout << "fail: too old to drive" << endl;
            return;
        }
        if(this->tempo < time) {
            cout << "fail: time finished after " << this->tempo<< " minutes" << endl;
            this->tempo = 0;
            return;
        }
        this->tempo-= time;
    }

    string str() const{
        ostringstream os;
        os << "power:" << power << ", time:" << tempo;
        os << ", person:(" << (person == nullptr ? "empty" : person->str()) << ")";
        return os.str();
    }
};

ostream& operator<<(ostream& os, const Motorcycle& m) {
    return os << m.str();
}


int main() {
    aux::Chain chain;
    aux::Param param;

    Motorcycle m(1);

    auto INT = aux::to<int>;

    chain["show"]  = [&]() { aux::show << m; };
    chain["leave"] = [&]() { 
        auto person = m.remove(); 
        if (person != nullptr) {
            aux::show << *person;
        }
    };
    chain["honk"]  = [&]() { aux::show << m.honk(); };
    chain["init"]  = [&]() { m = Motorcycle(INT(param.at(1)));};
    chain["enter"] = [&]() { m.insertPerson(std::make_shared<Person>(param.at(1), INT(param.at(2)))); };
    chain["buy"]   = [&]() { m.buyTime(INT(param.at(1))); };
    chain["drive"] = [&]() { m.drive  (INT(param.at(1))); };

    aux::execute(chain, param);
}