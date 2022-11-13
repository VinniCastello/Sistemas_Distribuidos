#include <iostream>
#include <vector>
using namespace std;

int count(vector<int> vet, int x) {
    int appeared = { 0 };
    for(auto elem : vet)
        if(elem == x){
            appeared++;
            if(appeared == 0)
                appeared++;
        }
    return appeared;
}

int sum(vector<int> vet) {
    int sum {};
    for(auto elem : vet)
        sum = sum + abs(elem);
    return sum;
}

double average(const vector<int>& vet) {
    double value {};
    for (auto elem : vet)
        value += abs(elem);
    return value / vet.size();
}

string more_men(const vector<int>& vet) {
    int men = { 0 }, women = { 0 };
    for(auto elem : vet)
        if(elem > 0)
            men++;
        else 
            women++;
    if(men > women)
        return "men";
    if(men < women)
        return "women";
    return "draw";
}

string half_compare(const vector<int>& vet) {
    auto met = (int)vet.size() / 2;
    int m1{0}, m2{0};
    for(auto i = 0; i < met; i++)
        m1 = m1 + abs(vet[i]);
    for(auto i = met; i < (int)vet.size(); i++)
        m2 = m2 + abs(vet[i]);
    if(m1 > m2)
        return "first";
    if(m1 < m2)
        return "second";
    return "draw";
}

string sex_battle(const vector<int>& vet) {
    int men{0}, women{0};
    int countMen{0}, countWomen{0};
    for (auto element : vet) {
        if (element > 0) {
            countMen++;
            men += element;
        }
        else {
            countWomen++;
            women = women + abs(element);
        }
    }
    if (men / countMen > women / countWomen)
        return "men";
    if (men / countMen < women / countWomen)
        return "women";
    return "draw";
}

#include <aux.hpp>
using namespace aux;

//loop principal
int main(){
    Chain chain;
    Param ui;

    auto float2str= LAMBDA(x, x | aux::STR("%.2f"));                            //converte de float para string com 2 casas
    auto INT      = LAMBDA(x, x | aux::STR2<int>());                            //converte de string para int
    auto VET      = LAMBDA(x, x | COPY(1, -1) | SPLIT(',') | MAP(STR2<int>())); //converte de string para vetor de int

    chain["count"]        = [&] {        count(VET(ui[1]), INT(ui[2])) | PRINT(); };
    chain["sum"]          = [&] {          sum(VET(ui[1])            ) | PRINT(); };
    chain["average"]      = [&] {      average(VET(ui[1])            ) | PIPE(float2str) | PRINT(); };
    chain["more_men"]     = [&] {     more_men(VET(ui[1])            ) | PRINT(); };
    chain["half_compare"] = [&] { half_compare(VET(ui[1])            ) | PRINT(); };
    chain["sex_battle"]   = [&] {   sex_battle(VET(ui[1])            ) | PRINT(); };

    execute(chain, ui);
}