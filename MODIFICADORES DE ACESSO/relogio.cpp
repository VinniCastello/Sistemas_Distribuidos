#include <iostream>
#include <sstream>
#include <iomanip>
#include <aux.hpp>
using namespace std;

class Time {
private:
    int hour {0}, minute {0}, second {0};

public:
    Time(int hour = 0, int minute = 0, int second = 0) {
    }
    void setHour(int hour) {
        if(hour < 24) {
            this->hour = hour;
            return;
        }
        cout << "fail: hora invalida\n";
    }
    void setMinute(int minute) {
        if(minute < 60) {
            this->minute = minute;
            return;
        }
        cout << "fail: minuto invalido\n";
    }
    void setSecond(int second) {
        if(second < 60) {
            this->second = second;
            return;
        }
        cout << "fail: segundo invalido\n";
    }
    int getHour() {
        return hour;
    }
    int getMinute() {
        return minute;
    }
    int getSecond() {
        return second;
    }
    void nextSecond() {
        this->second++;
        if(getSecond() > 59){
            this->minute += 1;
            setSecond(00);
        } 
        if(getMinute() > 59){
            this->hour += 1;
            setMinute(00);
        }
        if(getHour() > 23) {
            setHour(00);
        }
    }
    string str() {
        stringstream ss;
        auto sw = setw(2);
        ss  << setfill('0') << sw << this->hour << ":" << sw << this->minute << ":" << sw << this->second;
        return ss.str();
    }
};

int main() {
    aux::Chain chain;
    aux::Param param;
    Time time;

    auto INT = aux::STR2<int>();

    chain["set"] = [&]() {
        time.setHour(INT(param[1]));
        time.setMinute(INT(param[2]));
        time.setSecond(INT(param[3]));
    };
    chain["init"] = [&]() {
        time = Time(INT(param[1]), INT(param[2]), INT(param[3]));
    };
    chain["show"] = [&]() {
        cout << time.str() << '\n';
    };
    chain["next"] = [&]() {
        time.nextSecond();
    };

    aux::execute(chain, param);
}