#include <iostream>
#include <vector>
#include <aux.hpp>
using namespace aux;

bool in(std::vector<int> vet, int x) {
    for (auto elem : vet)
        if (elem == x)
            return true;
    return false;
}

int index_of(std::vector<int> vet, int x) {
    for (int i = 0; i < (int) vet.size(); i++)
        if (vet[i] == x)
            return i;
    return -1;
}

int find_if(const std::vector<int>& vet) {
    for(int i = 0; i < vet.size(); i++)
        if(vet[i] > 0)
            return i;
    return -1;
}

int min_element(const std::vector<int>& vet) {
    int pos_min = { -1 };
    for(int i = 0; i < vet.size(); i++)
        if(vet[i] < vet[pos_min])
            pos_min = i;
    return pos_min;
}

int find_min_if(const std::vector<int>& vet) {
    int pos_men = { -1 };
    for(int i = 0; i < vet.size(); i++)
        if(vet[i] > 0 && (vet[pos_men] > vet[i] || pos_men == -1))
        pos_men = i;
    return pos_men;
}


using namespace aux;

int main(){
    Chain chain;
    Param ui;

    auto bool2str = LAMBDA(x, x ? "true" : "false"); //converte de bool para string true or false
    auto INT  = LAMBDA(x, x | aux::STR2<int>());                            //converte de string para int
    auto VET  = LAMBDA(x, x | COPY(1, -1) | SPLIT(',') | MAP(STR2<int>())); //converte de string para vetor de int

    chain["in"]           = [&] {          in(VET(ui.at(1)), INT(ui.at(2))) | PIPE(bool2str) | PRINT(); };
    chain["index_of"]     = [&] {    index_of(VET(ui.at(1)), INT(ui.at(2))) | PRINT(); };
    chain["find_if"]      = [&] {     find_if(VET(ui.at(1))               ) | PRINT(); };
    chain["min_element"]  = [&] { min_element(VET(ui.at(1))               ) | PRINT(); };
    chain["find_min_if"]  = [&] { find_min_if(VET(ui.at(1))               ) | PRINT(); };

    execute(chain, ui);
}