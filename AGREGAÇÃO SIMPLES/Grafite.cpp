#include <iostream>
#include <sstream>
#include <memory>  //sharedptr
#include <iomanip> //setprecision
#include <utility> //exchange
#include <aux.hpp>
using namespace std;

class Lead {
    float thickness;
    string hardness;
    int size;

public:
    Lead(float thickness, string hardness, int size) : thickness(thickness), hardness(hardness), size(size) {
    }

    int usagePerSheet() {
        if(hardness == "HB"){
            return 1;
        }
        if(hardness == "2HB") {
            return 2;
        }
        if(hardness == "4B") {
            return 4;
        }
        if(hardness == "6HB") {
            return 6;
        }
        return 0;
    }

    float getThickness() {
        return thickness;
    }

    string getHardness() {
        return hardness;
    }

    int getSize() {
        return size;
    }

    void setSize(int size) {
        this->size = size;
    }
    string str() {
        stringstream ss;
        ss << fixed << setprecision(1) 
           << thickness << ":" << hardness << ":" << size;
        return ss.str();
    }
};

ostream& operator<<(ostream& ss, Lead gr) {
    return ss << gr.str();
}

struct Pencil {
    float thickness {0.f};
    shared_ptr<Lead> tip {nullptr};

    Pencil(float thickness = 0.0) {
        this->thickness = thickness;
        tip = nullptr;
    }

    bool hasGrafite() {
        if(tip == nullptr)
            return false;
        return true;
    }

    bool insert(shared_ptr<Lead> grafite) {
        if(hasGrafite()) {
            cout << "fail: ja existe grafite\n";
            return false;
        }
        if(grafite->getThickness() == thickness) {
            tip = grafite;
            return true;
        }
        else {
            cout << "fail: calibre incompativel\n";
            return false;
        }
        return false;
    }

    shared_ptr<Lead> remove() {
       if(hasGrafite()) {
            tip = nullptr;
       } else {
            cout << "fail:	nao tem grafite\n";
       }
       shared_ptr<Lead> aux = this->tip;
       this->tip = nullptr;
       return aux;
    }

    void writePage() {
        if(!hasGrafite()) {
            cout << "fail: nao existe grafite\n";
            return;
        }
        if(tip->getSize() <= 10) {
            cout << "fail: tamanho insuficiente\n";
            return;
        }
        if((tip->getSize() - tip->usagePerSheet()) < 10) {
            tip->setSize(10);
            cout << "fail: folha incompleta\n";
            return;
        }
        tip->setSize(tip->getSize() - tip->usagePerSheet());
        return;
    }

    string str() {
        stringstream ss;
        ss << "calibre: " << thickness << ", grafite: ";
        if (tip != nullptr)
            ss << "[" << *tip << "]";
        else
            ss << "null";
        return ss.str();
    }
};

ostream& operator<<(ostream& os, Pencil l) {
    return os << l.str();
}

int main() {
    aux::Chain chain;
    aux::Param ui;

    Pencil pencil;

    auto tofloat = aux::to<float>;
    auto toint   = aux::to<int>;

    chain["show"]   = [&]() { cout << pencil << endl; };
    chain["init"]   = [&]() { pencil = Pencil(tofloat(ui[1])); };
    chain["insert"] = [&]() { pencil.insert(make_shared<Lead>(tofloat(ui[1]), ui[2], toint(ui[3]))); };
    chain["remove"] = [&]() { pencil.remove(); };
    chain["write"]  = [&]() { pencil.writePage(); };

    aux::execute(chain, ui);
}