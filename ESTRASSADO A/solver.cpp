#include <iostream>
#include <vector>
using namespace std;

vector<int> get_men(vector<int> input) {
    vector<int> men;
    for (int elem : input)
        if (elem > 0)
            men.push_back(elem);
    return men;
}

vector<int>get_women(vector<int> input) {
    vector<int> women;
    for (int elem : input)
        if (abs(elem) < 10)
            women.push_back(elem);
    return women;
}

void sort(vector<int>& input)
{
    int output = { 0 };
    for(int i = 0; i < (int)input.size(); i++)
        for(int j = 0; j < (int)input.size() - i - 1; j++)
            if(input[j] > input[j+1]){
                output = input[j];
                input[j] = input[j+1];
                input[j+1] = output;
            }
}

vector<int> reverse(vector<int> input) {
    vector<int> output;
    for (auto i = (int)input.size() - 1; i >= 0; i--)
        output.push_back(input[i]);
    return output;
}

int main() {
    vector<int> men{1,3,-10,20,-50};
    vector<int> women{-1,-2,-3,-4,-5};
    vector<int> reverse_{-5, 4, 0, 2, 1};

    vector<int> homem  { get_men(men) };
    for (auto element : homem)
        cout << element << " ";
    cout << endl;

    vector<int> mulher { get_women(women) };
    for (auto element : mulher)
        cout << element << " ";
    cout << endl;

    vector<int> rep {5,3,-1,-50,-1,-99};
    sort(rep);
    for(auto element : rep)
        cout << element << " ";
    cout << endl;

    vector<int> invertida { reverse(reverse_) };
    for(auto element : invertida)
        cout << element << " ";
    cout << endl;
}