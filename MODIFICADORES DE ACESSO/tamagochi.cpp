#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <aux.hpp>
using namespace std;


class Pet{
private:
    int energyMax, hungryMax, cleanMax;
    int energy, hungry, clean;
    int diamonds {0};
    int age {0};
    bool alive;

    bool testAlive() {
        if (alive)
            return true;
        cout << "fail: pet esta morto" << '\n';
        return false;
    }

    void setHungry(int value) {
        if (value <= 0) {
            hungry = 0;
            cout << "fail: pet morreu de fome" << '\n';
            alive = false;
        } else if (value > hungryMax) {
            hungry = hungryMax;
        } else {
            hungry = value;
        }
    }
    void setEnergy(int value) {
        if(value <= 0) {
            energy = 0;
            cout << "fail: pet morreu de fraqueza" << '\n';
            alive = false;
        } else if(value > energyMax){
            energy = energyMax;
        } else {
            energy = value;
        }
    }

    void setClean(int value) {
        if(value <= 0){
            clean = 0;
            cout << "fail: pet morreu de sujeira" << '\n';
            alive = false;
            return;
        } else if(value > cleanMax){
            clean = cleanMax;
            return;
        } else {
            clean = value;
            return;
        }
    }


public:
    Pet(int energy = 0, int hungry = 0, int clean = 0) {
        this->energy = energy; this->energyMax = energy;
        this->hungry = hungry; this->hungryMax = hungry;
        this->clean = clean;   this->cleanMax  = clean;
        diamonds = 0;
        age = 0;
        alive = true;
    }

    void play() {
        if (!testAlive())
            return;
        setEnergy(getEnergy() - 2);
        setHungry(getHungry() - 1);
        setClean(getClean() - 3);
        diamonds += 1;
        age += 1;
    }

    void shower() {
        if(!testAlive())
            return;
        setEnergy(getEnergy() - 3);
        setHungry(getHungry() - 1);
        setClean(getCleanMax());
        age += 2;
    }

    void eat() {
        if(!testAlive())
            return;
        setEnergy(getEnergy() - 1);
        setHungry(getHungry() + 4);
        setClean(getClean() - 2);
        age += 1;
    }

    void sleep() {
        if(!testAlive())
            return;
        if((getEnergyMax() - getEnergy()) < 5){
            cout << "fail: nao esta com sono" << '\n';
            return;
        }
            age += (getEnergyMax() - getEnergy());
            setEnergy(getEnergyMax());
            setHungry(getHungry() - 1);
            return;
    }


    int getClean() {
        return clean;
    }
    int getHungry() {
        return hungry;
    }
    int getEnergy() {
        return energy;
    }
    int getEnergyMax() {
        return energyMax;
    }
    int getCleanMax() {
        return cleanMax;
    }
    int getHungryMax() {
        return hungryMax;
    }

    string toString() {
        stringstream ss;
        ss <<  "E:" << energy << "/" << energyMax << ", "
           <<  "S:" << hungry << "/" << hungryMax << ", "
           <<  "L:" << clean << "/" << cleanMax << ", "
           <<  "D:" << diamonds << ", " << "I:"  << age;
        return ss.str();
    }

};


int main() {
    aux::Chain chain;
    aux::Param ui;
    Pet pet;
    auto toint = aux::STR2<int>();
    
    chain["show"]   = [&]() { cout << pet.toString() << '\n'; };
    chain["init"]   = [&]() { pet = Pet(toint(ui[1]), toint(ui[2]), toint(ui[3])); };
    chain["play"]   = [&]() { pet.play(); };
    chain["eat"]    = [&]() { pet.eat(); };
    chain["shower"] = [&]() { pet.shower(); };
    chain["sleep"]  = [&]() { pet.sleep(); };
    
    aux::execute(chain, ui);
};