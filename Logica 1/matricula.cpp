#include <iostream>
#include <memory>
#include <iomanip>
#include <map>
#include <vector>
#include <sstream>
#include <iterator>
using namespace std;

class Disciplina;

class Aluno{
    private:
        string name;
        map<string, Disciplina*> disciplinas;
    public:
        Aluno(string name);
        void addDisciplina(Disciplina * disciplina);
        void rmDisciplina(string name);
        string getName() const;
        vector<Disciplina*> getDisciplinas();

        friend ostream& operator<<(ostream& os, const Aluno& aluno);
};

class Disciplina{
    private:
        string name;
        map<string, Aluno*> alunos;
    public:
        Disciplina(string name);
        string getName() const;

        void addAluno(Aluno * aluno) {
            auto key = aluno->getName();
            if(this->alunos.find(key) == this->alunos.end()) {
                this->alunos.insert(pair<string, Aluno*>(key, aluno));
                aluno->addDisciplina(this);
            }
        }

        void rmAluno(string nome_aluno) {
            auto it = this->alunos.find(nome_aluno);
            if(it != this->alunos.end()) {
                auto it2 = it->second;
                this->alunos.erase(it);
                it2->rmDisciplina(this->name);
            }
        }

        vector<Aluno*> getAlunos() {
            vector<Aluno*> alunos;
            for(auto it = this->alunos.begin(); it != this->alunos.end(); it++) {
                alunos.push_back(it->second);
            }
            return alunos;
        }

        friend ostream& operator<<(ostream& os, const Disciplina& disciplina) {
            for(auto it : disciplina.alunos) {
                os << it.first << " ";
            }
            return os;
        }
};

Disciplina::Disciplina(string name) : name(name) {}

string Disciplina::getName() const {
    return this->name;
}

Aluno::Aluno(string name) : name(name) {}

void Aluno::addDisciplina(Disciplina * disciplina) {
    auto key = disciplina->getName();
    if(this->disciplinas.find(key) == this->disciplinas.end()) {
        this->disciplinas.insert(pair<string, Disciplina*>(key, disciplina));
        disciplina->addAluno(this);
    }
}

void Aluno::rmDisciplina(string name) {
    auto it = this->disciplinas.find(name);
    if(it != this->disciplinas.end()) {
        auto it2 = it->second;
        this->disciplinas.erase(it);
        it2->rmAluno(this->name);
    }
}

vector<Disciplina*> Aluno::getDisciplinas() {
    vector<Disciplina*> disciplinas;
    for(auto it = this->disciplinas.begin(); it != this->disciplinas.end(); it++) {
        disciplinas.push_back(it->second);
    }
    return disciplinas;
}

ostream& operator<<(ostream& os, const Aluno& aluno) {
    for(auto it : aluno.disciplinas) {
        os << it.first << " ";
    }
    return os;
}

string Aluno::getName() const { return this->name; }

template <typename T>
class Manager {
    private:
        map<string, shared_ptr<T>> objects;
    public:
        Manager() = default;

        void add(string name, shared_ptr<T> obj) {
            auto [it, suss] = this->objects.insert(make_pair(name, obj));
            if(!suss)
                throw string("fail add " + name);
        }

        T * get(string name) {
            auto it = this->objects.find(name);
            if(it == this->objects.end())
                throw string("fail get " + name);
            return it->second.get();
        }

        void rm(string name) {
            auto qtd = this->objects.erase(name);
            if(qtd == 0)
                throw string("fail rm " + name);
        }

        friend ostream& operator<<(ostream& os, const Manager<T>& manager) {
            for(auto it : manager.objects) {
                if constexpr (is_same<T, Aluno>::value)
                    os << setw(9) << it.first << " [ " << *it.second << "]\n";
                else
                    os << setw(7) << it.first << " [ " << *it.second << "]\n";
                
            }
            return os;
        }
};

class Sys {
    private:
        Manager<Disciplina> dscpl;
        Manager<Aluno> alns;
    public:
        void addAluno(string name) {
            this->alns.add(name, make_shared<Aluno>(name));
        }

        void addDisciplina(string name) {
            this->dscpl.add(name, make_shared<Disciplina>(name));
        }

        void matricular(string aluno, string disciplina) {
            this->alns.get(aluno)->addDisciplina(this->dscpl.get(disciplina));
        }

        void desmatricular(string aluno, string disciplina) {
            this->alns.get(aluno)->rmDisciplina(disciplina);
        }

        void rmAluno(string name) {
            auto disciplinas = this->alns.get(name)->getDisciplinas();
            for(auto it : disciplinas)
                it->rmAluno(name);
            this->alns.rm(name);
        }

        void rmDisciplina(string name) {
            auto alunos = this->dscpl.get(name)->getAlunos();
            for(auto it : alunos)
                it->rmDisciplina(name);
            this->dscpl.rm(name);
        }

        friend ostream& operator<<(ostream& os, const Sys& sys) {
            os << "alunos:\n";
            os << sys.alns;
            os << "discps:\n";
            os << sys.dscpl;
            return os;
        }
};

using namespace std;

int main() {
    string line, cmd;
    Sys sistema;
    while(true){
        try{
            getline(cin, line);
            cout << "$" << line << endl;
            stringstream ss(line);
            vector<string> ui(istream_iterator<string>{ss}, istream_iterator<string>());
            string cmd = ui[0];
            if(cmd == "end"){
                break;
            }else if(cmd == "nwalu"){
                for(size_t i = 1; i < ui.size(); i++)
                    sistema.addAluno(ui[i]);
            }else if(cmd == "nwdis"){
                for(size_t i = 1; i < ui.size(); i++)
                    sistema.addDisciplina(ui[i]);
            }else if(cmd == "show"){
                cout << sistema;
            }else if(cmd == "tie"){
                for(size_t i = 2; i < ui.size(); i++)
                    sistema.matricular(ui[1], ui[i]);
            }else if(cmd == "untie"){
                for(size_t i = 2; i < ui.size(); i++)
                    sistema.desmatricular(ui[1], ui[i]);
            }else if(cmd == "rmalu"){
                sistema.rmAluno(ui[1]);
            }else{
                cout << "comando invalido " << "[" << cmd << "]\n";
            }
        }catch(string e){
            cout << e << endl;
        }
    }
}
