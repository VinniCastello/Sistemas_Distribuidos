#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

enum Label {
    EXTRACT,
    WITHDRAW,
    DEPOSIT,
    FEE,
    REVERSE
};

class LabelData {
    string name = "error";
public:
    LabelData(Label op) {
        switch(op) {
            case EXTRACT:
                name = "opening";
                break;
            case WITHDRAW:
                name = "withdraw";
                break;
            case DEPOSIT:
                name = "deposit";
                break;
            case FEE: 
                name = "fee";
                break;
            case REVERSE:
                name = "reverse";
                break;
        }
    }

    string getName() {
        return this->name;
    }
};

class Operation {
    Label label;
    int index;
    int balance;
    int valor;
public:
    Operation(int index, int balance, int valor, Label label) {
        this->index = index;
        this->balance = balance;
        this->valor = valor;
        this->label = label;
    }
    
    int getIndex() const { 
        return index;
    }

    int getValor() const {
        return valor;
    }

    int getBalance() const {
        return balance;
    }

    Label getLabel() const {
        return label;
    }

    string str() {
        return LabelData(label).getName();
    }
};

class BalanceManager {
    int saldo { 0 };
    vector<Operation> extrato;
    int nextIndex { 0 };
public:
    BalanceManager() {}

    void addOperation(int value, Label label) {
        saldo += value;
        extrato.push_back(Operation(nextIndex, saldo, value, label));
        nextIndex++;
    }
    void resetAccount() {
        saldo = { 0 };
        nextIndex = { 0 };
        extrato.clear();
    }

    int getSaldo() const {
        return saldo;
    }

    vector<Operation> getExtrato() const {
        return extrato;
    }
};

class Conta {
    BalanceManager manager;
    int id;
public:
    Conta(int id = 0) {
        this->id = id;
    }

    int getId() {
        return id;
    }

    string str() {
        stringstream ss;
        ss << "account:" << id << " balance:" << manager.getSaldo() << endl;
        return ss.str();
    }

    void init(int value) {
        if(id != 0) {
            id = value;
            manager.resetAccount();
            manager.addOperation(0, Label::EXTRACT);
            return;
        }
        id = value;
        manager.addOperation(0, Label::EXTRACT);
    }

    bool withdraw(int value) {
        if(manager.getSaldo() < value) {
            cout << "fail: insufficient balance" << endl;
            return false;
        }
        manager.addOperation(-value, Label::WITHDRAW);
        return true;
    }

    bool deposit(int value) {
        if(value <= 0) {
            cout << "fail: invalid value"<< endl;
            return false;
        }
        manager.addOperation(value, Label::DEPOSIT);
        return true;
    }

    bool tarif(int value) {
        if(value < 0) {
            cout << "fail: value invalid"<< endl;
            return false;
        }
        manager.addOperation(-value, Label::FEE);
        return true;
    }
    
    void reverse(int value) {
        auto iterador = manager.getExtrato();
        if(value >= (int) iterador.size()) {// se o value ultrapassar o tamanaho do meu extrato
            cout << "fail: index " << value << " invalid" << endl;
            return;
        }
        for(auto aux = 0; aux < (int) iterador.size(); aux++) {// para pecorrer meu vector extrato
            if(iterador[aux].getIndex() == value) {// verifico a operaçao com o value/index
                if(iterador[aux].getLabel() == Label::FEE) {// caso ela seja do tipo FEE faço...
                    manager.addOperation(-iterador[aux].getValor(), Label::REVERSE);
                } 
                else {// retorno dizendo que nao é do tipo FEE
                    cout << "fail: index " << aux << " is not a fee" << endl;
                }
            }
        }
    }
    
    void extract(int index) {
        auto iterador = manager.getExtrato();
        if(index == 0) {// meu index for zero, começo do inicio
            for(auto aux =  0; aux < (int) iterador.size(); aux++) {
                cout << setw(2) << aux << ":" ;
                cout << setw(9) << iterador[aux].str() << ":";
                cout << setw(5) << iterador[aux].getValor() << ":";
                cout << setw(5) << iterador[aux].getBalance() << endl;
            }
        }
        else {// se nao, pego o tamanho do meu vetor e subtrario do meu index e assim vou conseguir acessar a pociçao apartir daquele ponto
            for(auto aux = (int) iterador.size() - index; aux < (int) iterador.size(); aux++){
                cout << setw(2) << aux << ":" ;
                cout << setw(9) << iterador[aux].str() << ":";
                cout << setw(5) << iterador[aux].getValor() << ":";
                cout << setw(5) << iterador[aux].getBalance() << endl;
            }
        }
    }
};

int main() {
    Conta conta;
    while(true) {
        string line;
        getline(cin, line);
        stringstream ss(line);
        string cmd;
        ss >> cmd;
        cout << "$" << line << endl;

        if(cmd == "end") {
            break;
        }
        else if(cmd == "init") {
            int value = { 0 };
            ss >> value;
            conta.init(value);
        }
        else if(cmd == "deposit") {
            int value = { 0 };
            ss >> value;
            conta.deposit(value);
        }
        else if(cmd == "withdraw") {
            int value = { 0 };
            ss >> value;
            conta.withdraw(value);
        }
        else if(cmd == "fee") {
            int value = { 0 };
            ss >> value;
            conta.tarif(value);
        }
        else if(cmd == "extract") {
            int index = { 0 };
            ss >> index;
            conta.extract(index);
        }
        else if(cmd == "reverse") {
            int value = { 0 };
            while(ss >> value) {
                conta.reverse(value);
            }
        }
        else if(cmd == "show"){
            cout << conta.str();
        }
        else {
            cout << "fail: invalid command" << endl;
        }
    }
}