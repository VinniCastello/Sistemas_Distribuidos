#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <string>
#include <utility>
#include <aux.hpp>
using namespace std;


#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <string>
#include <utility>
#include <aux.hpp>
using namespace std;

class Pessoa {
    string nome;
public:
    Pessoa(string nome) { 
        this->nome = nome; 
    }
    string getNome() const {
        return nome; 
    }
    string str() const {
        return nome;
    }
};
ostream& operator<<(ostream& os, const Pessoa& p) {
    return (os << p.str());
}

class Mercantil {
    vector<shared_ptr<Pessoa>> caixas; //caixas do supermercado
    list  <shared_ptr<Pessoa>> esperando; //lista de clientes esperando

    bool validarIndice(int indice) {
        if(indice < 0 || indice >= (int) caixas.size()) {
            return false;
        }
        return true;
    }

public:
    Mercantil(int qtd_caixas) : caixas(qtd_caixas, nullptr) {}
    
    void chegar(const shared_ptr<Pessoa>& person) {
        esperando.push_back(person);
    }

    bool chamarNoCaixa(int indice) {
        if(!validarIndice(indice)){
            cout << "fail: indice invalido\n";
            return false;
        }
        if(indice < 0 || indice >= (int) this->caixas.size() ) {
            cout << "fail: caixa inexistente\n";
            return false;
        }
        if(this->esperando.size() == 0) {
            cout << "fail: sem clientes\n";
            return false;
        }
        if(this->caixas[indice] != nullptr) {
            cout << "fail: caixa ocupado\n";
            return false;
        }
        this->caixas[indice] = this->esperando.front();
        this->esperando.pop_front();
        return true;
    }
    
    shared_ptr<Pessoa> finalizar(int indice) {
        if(indice < 0 || indice >= (int) this->caixas.size() ) {
            cout << "fail: caixa inexistente\n";
            return nullptr;
        }
        if(this->caixas[indice] == nullptr){
            cout << "fail: caixa vazio\n";
            return nullptr;
        }
        auto aux = this->caixas[indice];
        this->caixas[indice] = nullptr;
        return aux;
    }

    string str() const {
        int i = 0;
        auto fn = [&i](auto p) {
            stringstream ss; 
            ss << " " << i++ << ":" << (p == nullptr ? "-----" : p->getNome()) << " ";
            return ss.str();
        };
        stringstream os;
        os  << "Caixas: |" << (caixas | aux::MAP(fn) | aux::JOIN("|")) << "|\n"
            << "Espera: " << (esperando | aux::MAP(LAMBDA(x, *x)) | aux::FMT());
        return os.str();
    }
};

ostream& operator<<(ostream& os, const Mercantil& b) {
    return (os << b.str());
}

int main() {
    aux::Chain chain;
    aux::Param par;
    
    Mercantil bank(0);

    chain["init"]   = [&]() {   bank = Mercantil(aux::to<int>(par[1])); };
    chain["call"]   = [&]() { bank.chamarNoCaixa(aux::to<int>(par[1])); };
    chain["finish"] = [&]() {     bank.finalizar(aux::to<int>(par[1])); };
    chain["arrive"] = [&]() { bank.chegar(make_shared<Pessoa>(par[1])); };
    chain["show"]   = [&]() { cout << bank << '\n'; };

    aux::execute(chain, par);    
}