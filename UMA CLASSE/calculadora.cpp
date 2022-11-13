#include <iostream>
#include <sstream>
#include <iomanip>
#include "aux.hpp"
using namespace std;

struct Calculator {
    int batteryMax;
    int battery { 0 };
    float display { 0 };

    Calculator(int batteryMax) {
        this->batteryMax = batteryMax;
        this->battery = battery;
        this->display = display;
    }

    void chargeBattery(int value) { 
        if(battery + value > batteryMax) {
            battery = batteryMax;
            return;
        }
        else {
            battery += value;
            return;
        }
    }

    bool useBattery() {
        if(battery > batteryMax) {
            battery = batteryMax;
            return true;
        }
        return false;
    }

    void sum(int a, int b) { 
        if(battery == 0){
            battery = 0;
            cout << "fail: bateria insuficiente\n";
            return;
        }
        else {
            display = a + b;
            battery--;
        return;
        }
    }

    void division(int num, int den) {
        if(battery == 0) {
            cout << "fail: bateria insuficiente\n";
            return;
        }
        if(den == 0) {
            cout << "fail: divisao por zero\n"; 
            battery--;
            return;
        }
        else {
            display = num/den;
            battery--;
            return;
         }
    }

    string str() { 
        stringstream ss;
        ss << "display = " << fixed << setprecision(2) << this->display;
        ss << ", battery = " << this->battery;
        return ss.str();
    }
};

ostream& operator<<(ostream& os, Calculator c) {
    return (os << c.str());
}

int main() {
    Calculator c(0);
    aux::Chain chain;
    aux::Param param;

    // função para obter um parâmetro convertido para inteiro
    auto par_int = [&param](int index) { return aux::to<int>(param[index]); };

    chain["show"]   = [&]() { cout << c << endl;         };
    chain["init"]   = [&]() {  c = Calculator(par_int(1)          ); };
    chain["charge"] = [&]() { c.chargeBattery(par_int(1)          ); };
    chain["sum"]    = [&]() {           c.sum(par_int(1), par_int(2)); };
    chain["div"]    = [&]() {      c.division(par_int(1), par_int(2)); };

    aux::execute(chain, param);
}
