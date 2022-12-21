#include <iostream>
using namespace std;

class SerVivo {
    protected:
    string plant{"Planta"};
    bool alive {true};
    int age{0};
    public:
    SerVivo(int age) {
        this->age = age;
        cout << plant << " esta nascendo\n";
    }
    virtual void breathe() const {
        if (alive)
            cout << plant << " esta respirando\n";
    }

    virtual void isAlive() const {
        if (alive)
            cout << plant << " esta vivo(a)\n";
        else 
            cout << plant << " esta morto(a)\n";
    }

    virtual void die(){
        if (alive)
            cout << plant << " morreu\n";
        alive = false;
    }

        friend ostream& operator<<(ostream& os, SerVivo& viv){
        os << viv.plant << " tem " << viv.age << " anos e ";
        viv.isAlive();
        return os;
    }

};

class Planta : public SerVivo {
    protected:
    int bubs{0};
    public:
    Planta(int bubs, int age): bubs{bubs}, SerVivo{age} {
        plant = "Planta";
        cout << plant << " esta precisando de agua\n";
    }
    virtual void water() const{
        cout << plant << " esta sendo agoada\n";
    }

    friend ostream& operator<<(ostream& os, const Planta& planta){
        os << planta.plant << " tem " << planta.bubs << " brotos\n";
        return os;
    }
};

class Goiabeira : public Planta {
    int flowers{0};
    int fruits{0};
    public:
    Goiabeira(int bubs, int flowers, int fruits, int age):
    Planta{bubs, age}, flowers{flowers}, fruits{fruits} {
        plant = "Goiabeira";
        cout << plant << " esta brotando\n";
    }

    virtual void bloom() {
        cout << plant << " esta florecendo\n";
        flowers += 2;
    }

    virtual void frutar() {
        cout << plant << " esta produzindo frutas\n";
        fruits += 2;
    }

    friend ostream& operator<<(ostream& os, const Goiabeira& ang){
        os << ang.plant << " tem " << ang.flowers << " flores e " << ang.fruits << " frutos\n";
        return os;
    }

};

int main () {
    Goiabeira angi (3, 0, 7, 10);
    cout << angi;
    angi.water();
    Planta& planta = angi;
    cout << planta;
    SerVivo& ser = angi;
    angi.frutar();
    angi.bloom();
    planta.breathe();
    ser.isAlive();
    cout << angi;
    cout << planta;
    cout << ser;
    return 0;
}