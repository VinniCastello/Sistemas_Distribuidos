#include <iostream>
#include <memory>
#include <list>
#include <map>
#include <sstream>
#include <iomanip>
using namespace std;

class Account {
protected:
    double balance = 0.00;
    int id;
    string key;
    string type;
public:
    Account(int id, string name, string type) : id(id), key(name), type(type)
    {
    }

    float getBalance() const { return this->balance; }

    int getId() const { return this->id; }

    string getClientId() const { return this->key; }

    string getType() const { return this->type; }

    virtual void mothyUpdate() = 0;

    void deposit(float value) {
        this->balance += value;
    }

    bool transfer(Account * other, float value) {
        other->deposit(value);
        this->balance -= value;
        return true;
    }

    bool withdraw(float value) {
        if(this->balance < value) {
            cout << "fail: saldo insuficiente\n";
            return false;
        }
        this->balance -= value;
        return true;
    }

    friend ostream& operator<<(ostream&, const Account&);
};
ostream& operator<<(ostream& out, const Account& account) {
    out << account.id << ":" << account.key << ":" << fixed << setprecision(2) << account.balance << ":" << account.type << "\n";
    return out;
}

class Cheking : public Account {
public:
    Cheking(int id, string name) : Account(id, name, "CC")
    {
    }

    void mothyUpdate() override {
        this->balance -= 20;
    }
};

class Saving : public Account {
public:
    Saving(int id, string name) : Account(id, name, "CP")
    {
    }

    void mothyUpdate() override {
        this->balance = this->balance + (this->balance * 0.01);
    }
};

class Client {
    list<shared_ptr<Account>> accounts;
    string nameId;
public:
    Client(string nameId) : nameId(nameId)
    {
    }
    
    friend ostream& operator<<(ostream&, const Client&);

    list<shared_ptr<Account>> getAccounts() {
        return this->accounts;
    }

    string getNameId() {
        return this->nameId;
    }

    void addAccount(shared_ptr<Account> account) {
        this->accounts.push_back(account);
    }

};
ostream& operator<<(ostream& out, const Client& client) {
    out << client.nameId << " ";
    for(auto account : client.accounts) {
        out << *account;
    }
    return out;
}

class Bank {
    map<int, shared_ptr<Account>> accounts; 
    map<string, shared_ptr<Client>> clients;
    double nextId { 0.00 };
    Account* getAccount(int id) {
        auto it = this->accounts.find(id);
        if(it == this->accounts.end()) {
            return nullptr;
        }
        return it->second.get();
    }
public:
    Bank() {}

    void addClient(string nameId) {
        auto it = this->clients.find(nameId);
        if(it != this->clients.end()) {
            it->second->addAccount(make_shared<Cheking>(this->nextId++, nameId));
            it->second->addAccount(make_shared<Saving>(this->nextId++, nameId));
            auto accounts = it->second->getAccounts();
            for(auto account : accounts) {
                this->accounts.insert(pair<int, shared_ptr<Account>>(account->getId(), account));
            }
            return;
        } else {
            auto client = make_shared<Client>(nameId);
            client->addAccount(make_shared<Cheking>(this->nextId++, nameId));
            client->addAccount(make_shared<Saving>(this->nextId++, nameId));
            this->clients.insert(make_pair(nameId, client));
            auto accounts = client->getAccounts();
            for(auto account : accounts) {
                this->accounts.insert(pair<int, shared_ptr<Account>>(account->getId(), account));
            }
        }
    }

    bool deposit(int id, float value) {
        auto account = this->getAccount(id);
        if(account == nullptr) {
            cout << "fail: conta nao encontrada\n";
            return false;
        }
        account->deposit(value);
        return true;
    }

    bool transfer(int id, int id2, float value) {
        auto youSent = this->getAccount(id);
        auto hasReceived = this->getAccount(id2);
        if(youSent == nullptr || hasReceived == nullptr) {
            cout << "fail: conta nao encontrada\n";
            return false;
        }
        return youSent->transfer(hasReceived, value);
    }

    bool whitdraw(int id, float value) {
        auto account = this->getAccount(id);
        if(account == nullptr) {
            cout << "fail: conta nao encontrada\n";
            return false;
        }
        return account->withdraw(value);
    }
    
    void mothyUpdate() {
        for(auto& account : this->accounts) {
            account.second->mothyUpdate();
        }
    }

    friend ostream& operator<<(ostream&, const Bank&);
};
ostream& operator<<(ostream& out, const Bank& bank) {
    out << "- Clients" << endl;
    for(auto client : bank.clients) {
        out << client.first << " " << "[";
        for(auto account : client.second->getAccounts()) {
            out << account->getId();
            if(account != client.second->getAccounts().back()) {
                out << ", ";
            }
        }
        out << "]" << endl;
    }
    out << "- Accounts" << endl;
    for(auto account : bank.accounts) {
        out << *account.second;
    }
    return out;
}

int main() {
    Bank bank;
    while(true) {
        string line, cmd;
        getline(cin, line);
        cout << "$" << line << endl;
        stringstream ss(line);
        ss >> cmd;
        if(cmd == "end") {
            break;
        } else if(cmd == "addCli") {
            string nameId;
            ss >> nameId;
            bank.addClient(nameId);
        } else if(cmd == "show") {
            cout << bank;
        } else if(cmd == "deposito") {
            int id;
            float value;
            ss >> id >> value;
            bank.deposit(id, value);
        } else if(cmd == "saque") {
            int id;
            float value;
            ss >> id >> value;
            bank.whitdraw(id, value);
        } else if(cmd == "transf") {
            int id, id2;
            float value;
            ss >> id >> id2 >> value;
            bank.transfer(id, id2, value);
        } else if(cmd == "update") {
            bank.mothyUpdate();
        } else {
            cout << "Comando invalido" << endl;
        }
    }
}