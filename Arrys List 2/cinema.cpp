#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <sstream>
using namespace std;

class Cliente {
    string nome;
    string fone;

public:
    Cliente(string nome = "", string fone = "") : nome{nome}, fone{fone} {}
     
    friend ostream& operator<<(ostream& os, const Cliente& c) {
        os << c.nome << ":" << c.fone;
        return os;
    }

    void setNome(string nome) {
        this -> nome = nome;
    }

    void setFone(string fone) {
        this -> fone = fone;
    } 

    string getNome() {
        return this -> nome;
    }

    string getFone() {
        return this -> fone;
    } 
};

class Cinema {
private:
    map<int, shared_ptr<Cliente>> assentos;
public:
    Cinema(int capacidade = 0) {
        for (int i = 0; i < capacidade; i++) {
            assentos[i] = nullptr;
        }
    }

    int procurarCliente(string nome) {
        for (int i = 0; i < assentos.size(); i++) {
            if (assentos[i] != nullptr && assentos[i]->getNome() == nome) {
                return i;
            }
        }
        return -1;
    }

    bool reservar(string nome, string fone, int indice) {
        if (indice < 0 || indice >= this->assentos.size()) {
            cout << "fail: cadeira nao existe" << endl;
            return false;
        }
        if (procurarCliente(nome) != -1) {
            cout << "fail: cliente ja esta no cinema" << endl;
            return false;
        }
        if (assentos[indice] != nullptr) {
            cout << "fail: cadeira ja esta ocupada" << endl;
            return false;
        }
        this -> assentos[indice] = make_shared<Cliente>(nome, fone);
        return true;
    }

    void cancelar(string nome) {
        int indice = procurarCliente(nome);
        if (indice == -1) 
            cout << "fail: cliente nao esta no cinema" << endl;
        else
            this->assentos[indice] = nullptr;
        
    }

    friend ostream& operator<<(ostream& os, const Cinema& c) {
        os << "[";	
        for (int i = 0; i < c.assentos.size(); i++) {
            if (i > 0)
                os << " ";
            if (c.assentos.at(i) == nullptr)
                os << "-";
            else
                os << "" << *c.assentos.at(i);
        }
        os << "]";
        return os;

    }
};

int main() {
    Cinema cinema(0);
    while(true) {
        string line, cmd;
        getline(cin, line);
        cout << "$" << line << endl;
        stringstream ss(line);
        ss >> cmd;
        if (cmd == "end") {
            break;
        } else if (cmd == "init") {
            int capacidade;
            ss >> capacidade;
            cinema = Cinema(capacidade);
        } else if (cmd == "show") {
            cout << cinema << endl;
        } else if (cmd == "reservar") {
            string nome, fone;
            int indice;
            ss >> nome >> fone >> indice;
            cinema.reservar(nome, fone, indice);
        } else if (cmd == "cancelar") {
            string nome;
            ss >> nome;
            cinema.cancelar(nome);
        } else {
            cout << "comando invalido" << endl;
        }
    }
}