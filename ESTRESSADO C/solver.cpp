#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

bool exists(vector<int>& v, int x) {
    return find(v.begin(), v.end(), x) != v.end();
}

vector<int> get_men(vector<int> vet) {
    vector<int> men;
    for (int elem : vet)
        if (elem > 0)
            men.push_back(elem);
    return men;
}

vector<int> get_calm_women(vector<int> vet) {
    vector<int> women;
    for (int elem : vet)
        if (elem < 0 && elem > -10)
            women.push_back(elem);
    return women;
}

vector<int> sort(vector<int> vet) {
    sort(vet.begin(), vet.end());
        return vet;
}

vector<int> sort_stress(vector<int> vet) {
    sort(vet.begin(), vet.end(), [](int a, int b) { return abs(a) < abs(b); });
    return vet;
}

vector<int> reverse(vector<int> vet) {
    vector<int> output;
    for (auto i = (int)vet.size() - 1; i >= 0; i--)
        output.push_back(vet[i]);
    return output;
}

void reverse_inplace(vector<int> &vet) {
    vector<int> output;
    for(int i = (int)vet.size() - 1; i >= 0; i--)
        output.push_back(vet[i]);
    vet = output;
}

vector<int> unique(vector<int> vet) {
    vector<int> unique;
    for (auto elem : vet)
        if(! exists(unique, elem))
            unique.push_back(elem);
    return unique;
}

vector<int> repeated(vector<int> vet) {
    vector<int> unique;
    vector<int> repeated;
    for (auto elem : vet)
        if(! exists(unique, elem))
            unique.push_back(elem);
        else
            repeated.push_back(elem);
    return repeated;

}


#include <aux.hpp>
using namespace aux;

//loop principal
int main(){
    Chain chain;
    Param ui;

    auto VET  = LAMBDA(x, x | COPY(1, -1) | SPLIT(',') | MAP(STR2<int>())); //converte de string para vetor de int
    auto par2vet = LAMBDAE(&, index, VET(ui.at(index)));

    chain["get_men"]         = [&] {        get_men(par2vet(1)) | FMT() | PRINT(); };
    chain["get_calm_women"]  = [&] { get_calm_women(par2vet(1)) | FMT() | PRINT(); };
    chain["sort"]            = [&] {           sort(par2vet(1)) | FMT() | PRINT(); };
    chain["sort_stress"]     = [&] {    sort_stress(par2vet(1)) | FMT() | PRINT(); };
    chain["reverse"]         = [&] {        reverse(par2vet(1)) | FMT() | PRINT(); };
    chain["repeated"]        = [&] {       repeated(par2vet(1)) | FMT() | PRINT(); };
    chain["unique"]          = [&] {         unique(par2vet(1)) | FMT() | PRINT(); };
    chain["reverse_inplace"] = [&] { 
        auto vet = par2vet(1);
        reverse_inplace(vet);
        vet | FMT() | PRINT();
    };

    execute(chain, ui);
}

