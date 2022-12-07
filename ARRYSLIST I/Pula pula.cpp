#include <iostream>
#include <list>
#include <sstream>
#include <memory>
#include <aux.hpp>
using namespace std;

class Kid {
private:
    int age;
    string name;
public:
    Kid(string name, int age) {
        this->age = age;
        this->name = name;
    }
    int getAge() {
        return age;
    }
    string getName() {
        return name;
    }
    string str() {
        return name + ":" + to_string(age);
    }
};

using PtrKid = shared_ptr<Kid>;

ostream& operator<<(ostream& os,  PtrKid kid) {
    return (os << kid->str());
}

class Trampoline {
    list<PtrKid> waiting;
    list<PtrKid> playing;
    
    PtrKid removeFromList(string name, list<PtrKid>& lista){
        for(auto it = lista.begin(); it != lista.end(); it++) {
            if( (*it)->getName() == name) {
                auto aux = (*it);
                lista.remove(*it);
                return aux;
            }
            
        }
        return nullptr;
    }

public:
    Trampoline() {
    }
    
    void arrive(PtrKid kid) {
        this->waiting.push_front(kid);
        return;
    }

    void enter() {
        if(this->waiting.empty()) {
            cout << "fail: nao tem ninguem na fila\n";
            return;
        }
        this->playing.push_front(this->waiting.back());
        this->waiting.pop_back();
        return;
    }

    void leave() {
        if(this->playing.size() != 0) {
            this->waiting.push_front(this->playing.back());
            this->playing.pop_back();
            return;
        }
        
    }

    PtrKid removeKid(string name) {
        removeFromList(name, this->waiting);
        removeFromList(name, this->playing);
        return nullptr;
    }
    string str() {
        return (waiting | aux::FMT()) + " => " + (playing | aux::FMT());
    }
};

int main() {
    aux::Chain chain;
    aux::Param param;
    Trampoline tr;
    chain["arrive"] = [&]() { tr.arrive(make_shared<Kid>(param[1], aux::to<int>(param[2]))); };
    chain["enter"]  = [&]() { tr.enter(); };
    chain["leave"]  = [&]() { tr.leave(); };
    chain["remove"] = [&]() { tr.removeKid(param[1]); };
    chain["show"]   = [&]() { cout << tr.str() << endl; };

    aux::execute(chain, param);
}