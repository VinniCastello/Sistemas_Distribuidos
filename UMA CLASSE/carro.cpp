#include <iostream>
#include <sstream>
#include <aux.hpp>
using namespace std;

class Car {
public:
    int pass {0};
    int passMax {2};
    int gas {0};
    int gasMax {100};
    int km {0};

    Car() { // todo
    }
    
    void show() {
    cout << "pass: " << pass << ", gas: " << gas << ", km: " << km << endl;
    }
    
    void enter() {
        if (this->pass == this->passMax)
            cout << "fail: limite de pessoas atingido\n";
        else
           pass++;
    }

    void leave() {
        if (this->pass == 0)
            cout << "fail: nao ha ninguem no carro\n";
        else
            pass--;
    }

    void fuel(int gas) {
        this-> gas += gas;
        if (this-> gas > this-> gasMax)
            this-> gas = this-> gasMax;
    }

    void drive(int km) {
        if(this->pass == 0) {
            cout << "fail: nao ha ninguem no carro\n";
            return;
        }
        if(this->gas == 0){
            cout << "fail: tanque vazio\n";
            return;
        }
        if(this->gas <= km) {
            cout << "fail: tanque vazio apos andar " << this->gas << " km\n";
            this->km += this->gas;
            this->gas = 0;
            return;
        }
        this->km += km;
        this-> gas -= km;
    }

    std::string str() {
        std::stringstream ss;
        ss << "pass: " << pass << ", gas: " << gas << ", km: " << km << "";
        return ss.str();
    }
};

int main() {
    aux::Chain chain;
    aux::Param ui;

    Car car;

    chain["help"]  = [&](){ aux::show << "show; enter; leave; fuel _gas; drive _km; end"; };
    chain["show"]  = [&](){ aux::show << car.str(); };
    chain["enter"] = [&](){ car.enter(); };
    chain["leave"] = [&](){ car.leave(); };
    chain["fuel"]  = [&](){ car.fuel(aux::to<int>(ui[1])); };
    chain["drive"] = [&](){ car.drive(aux::to<int>(ui[1])); };

    aux::execute(chain, ui);
}