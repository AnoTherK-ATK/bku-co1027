#include "knight.h"

//some constants
const int maxLevel = 10;
const int maxRemedy = 99;
const int maxMaidenKiss = 99;
const int maxPhoenixDown = 99;
const int maxEvent = (int)2e5 + 7; //why it doesn't have a max number of events

//some math functions
bool isPrime(int x){
    if(x <= 1) return 0;
    for(int i = 2; i * i <= x; ++i){
        if(x % i == 0) return 0;
    }
    return 1;
}

struct Knights{
    int HP, Level, Remedy, MaidenKiss, PhoenixDown; //always update this
    int Rescue = -1; //fuck me in the ass
    int prevLevel;
    int maxHP = 999;
    void Input(string s){
        stringstream ss(s);
        ss >> HP >> Level >> Remedy >> MaidenKiss >> PhoenixDown;
        maxHP = HP;
    }

    void healthCheck(){
        if (HP <= 0) Rescue = 0;
    }

    void display(){
        cout << "HP=" << HP
            << ", level=" << Level
            << ", remedy=" << Remedy
            << ", maidenkiss=" << MaidenKiss
            << ", phoenixdown=" << PhoenixDown
            << ", rescue=" << Rescue << endl;
    }

    struct Tiny{
        bool isTiny = 0;
        int eventCount;
    } tiny;

    void turnTiny(){
        tiny.isTiny = 1;
        tiny.eventCount = 4;
        HP = max(1, HP / 5);
    }

    void returnTiny(){
        tiny.isTiny = 0;
        tiny.eventCount = 0;
        HP = min(maxHP, HP * 5);
    }

    void tinyExecute(){
        if(tiny.isTiny){
            if(Remedy){
                --Remedy;
                returnTiny();
            }else{
                if(tiny.eventCount - 1 <= 0)
                    returnTiny();
                else
                    tiny.eventCount--;
            }
        }
    }

    struct frog{
        bool isFrog = 0;
        int eventCount = 0;
    } frog;

    void turnFrog(){
        frog.isFrog = 1;
        frog.eventCount = 4;
        prevLevel = Level;
        Level = 1;
    }

    void returnFrog(){
        frog.isFrog = 1;
        frog.eventCount = 0;
        Level = prevLevel;
    }

    void frogExecute(){
        if(frog.isFrog){
            if(MaidenKiss){
                --MaidenKiss;
                returnFrog();
            }else{
                if(frog.eventCount - 1 <= 0)
                    returnFrog();
                else
                    frog.eventCount--;
            }
        }
    }

    void checkStatus(){
        tinyExecute();
        frogExecute();
    }
} knight;

struct Events{
    int ListOfEvents[maxEvent];
    int numEvents = 0;
    
    void Input(string s){
        stringstream ss(s);
        int event;
        while(ss >> event)
            ListOfEvents[++numEvents] = event;
    }
    
    //id = 0, journey ends
    void success(Knights &knight){
        knight.Rescue = 1;
    }

    int levelCalculate(int index){
        return (index > 6 ? ((index % 10) > 5 ? (index % 10) : 5) : (index % 10));
    }

    //id from 1 to 5, meet normal opponents
    void meetNormal(Knights &knight, int index){
        int level0 = levelCalculate(index);

        if (knight.Level > level0){
            knight.Level = min(maxLevel, knight.Level + 1);
            return;
        }
        if(knight.Level < level0){
            int baseDamage[6] = {0, 10, 15, 45, 75, 95};
            int damage = level0 * baseDamage[index];
            knight.HP -= damage;
            knight.healthCheck();
            return;
        }
    }

    //id = 6, meet Shaman, and can be turn into tiny with a lil penis
    void meetShaman(Knights &knight, int index) {
        if(knight.tiny.isTiny || knight.frog.isFrog) return; //became tiny/frog, don't fight
        int level0 = levelCalculate(index);

        if(knight.Level > level0){
            knight.Level = min(maxLevel, knight.Level + 2);
            return;
        }
        if(knight.Level < level0)
            knight.turnTiny();
    }

    //id = 7, meet Siren Vajsh, and can be turn into frog and wait for a chick to suck dick
    void meetSirenVajsh(Knights &knight, int index){
        if(knight.tiny.isTiny || knight.frog.isFrog) return; //became tiny/frog, don't fight
        int level0 = levelCalculate(index);

        if(knight.Level > level0){
            knight.Level = min(maxLevel, knight.Level + 2);
            return;
        }
        if(knight.Level < level0)
            knight.turnFrog();
    }

    //id = 11, it's-a-me, Mario
    void pickMariomush(Knights &knight){
        int n1 = ((knight.Level + knight.PhoenixDown) % 5 + 1) * 3;
        int s1 = 0;
        for (int i = 99; n1 <= 0; --n1, i -= 2)
            s1 += i;
        knight.HP += knight.HP + (s1 % 100);
        while(!isPrime(knight.HP)) knight.HP++;
        knight.HP = max(knight.maxHP, knight.HP);
    }

    //id = 13, what the fuck is ghost mushroom
    
} event;

struct SpecialEvents{

};

void fileINP(string file_input){
    ifstream in(file_input);
    string sinp; //don't ask why I named this
    getline(in, sinp);
    knight.Input(sinp);

    getline(in, sinp);
    event.Input(sinp);

    getline(in, sinp);
    in.close();
}

void adventureToKoopa(string file_input, int & HP, int & level, int & remedy, int & maidenkiss, int & phoenixdown, int & rescue) {
    //cout << "Function isn't implemented" << endl;
    fileINP(file_input);
}