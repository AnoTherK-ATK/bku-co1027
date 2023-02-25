#include "knight.h"

//some constants
const int maxLevel = 10;
const int maxItem = 99;
const int maxEvent = (int)2e5 + 7; //why it doesn't have a max number of events
int fib[40];
string l3files[3];

//some functions
bool isPrime(int x){
    if(x <= 1) return 0;
    for(int i = 2; i * i <= x; ++i){
        if(x % i == 0) return 0;
    }
    return 1;
}

void fibo(){
    fib[0] = 1;
    fib[1] = 1;
    for(int i = 2; i <= 40; ++i){
        fib[i] = fib[i - 1] + fib[i - 2];
    }
}

int strtoi(string s){
    int ans = 0;
    for(int i = 0; i < (int)s.size(); ++i){
        ans = ans * 10 + (s[i] - '0');
    }
    return ans;
}

void l3input(string file_input){
    int n = 0;
    file_input += ',';
    for(int i = 0; i < (int)file_input.size(); ++i){
        if(file_input[i] != ',')
            l3files[n] += file_input[i];
        else
            ++n;
    }
}

//I served with his majesty
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
        if (HP <= 0 && PhoenixDown == 0) 
            Rescue = 0;
        else{
            --PhoenixDown;
            HP = maxHP;
        }
    }

    void display(){
        cout << "HP=" << HP
            << ", level=" << Level
            << ", remedy=" << Remedy
            << ", maidenkiss=" << MaidenKiss
            << ", phoenixdown=" << PhoenixDown
            << ", rescue=" << Rescue << endl;
    }

    //tiny man with a tiny nose and tiny penis
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

    //you have a long tongue, babe
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
    string GhostMush[maxEvent];
    int numGmEvents = 0;
    void Input(string s){
        stringstream ss(s);
        string event;
        while(ss >> event)
            if(event[0] == '1' && event[1] == '3'){
                ListOfEvents[++numEvents] = 13;
                GhostMush[++numGmEvents] = event;
            }
            ListOfEvents[++numEvents] = strtoi(event);
    }
    
    //id = 0, I've won, but at what cost?
    void success(Knights &knight){
        knight.Rescue = 1;
    }

    int levelCalculate(int index){
        return (index > 6 ? ((index % 10) > 5 ? (index % 10) : 5) : (index % 10));
    }

    //id = 1 -> 5, you weak mtfk
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

    //id = 12, fi"porn"acci
    void pickFibmush(Knights &knight){
        int x = fib[1];
        for(int i = 1; i <= 40; ++i){
            if(fib[i] <= knight.HP)
                x = fib[i];
            else    
                break;
        }
        knight.HP = x;
    }

    //id = 13, what the fuck is ghost mushroom
    //ghost ha, i'm a ghostbuster
    struct ghostMushroom{
        int array[102], arrayTrans[102];
        int n;

        void Input(string file_mush_ghost){
            ifstream mg(file_mush_ghost);
            mg >> n;
            string s;
            getline(mg, s);
            getline(mg, s);
            s = s + ',';
            int idx = 0;
            for(int i = 0; i < (int)s.size(); ++i){
                if(s[i] != ',')
                    array[idx] = array[idx] * 10 + (s[i] - '0');
                else
                    ++idx;
            }
            for(int i = 0; i < n; ++i){
                arrayTrans[i] = abs(array[i]);
                arrayTrans[i] = (17 * arrayTrans[i] + 9) % 257;
            }
            mg.close();
        }
        
        //ms = 1, min max shit
        void pickMush1(int &x, int &y){
            int mx = INT_MIN;
            int mn = INT_MAX;
            for(int i = 0; i < n; ++i){
                if(array[i] >= mx){
                    x = i;
                    mx = array[i];
                }
                if(array[i] <= mn){
                    y = i;
                    mn = array[i];
                }
            }
        }

        //ms = 2, you're on the top of the world, you're high as fuck
        void pickMush2(int &val, int &pos){
            val = -2;
            pos = -3;
            int l = 0, r = n - 1;
            while(l != n - 1 && array[l] < array[l + 1])
                ++l;
            while(r != 1 && array[r] > array[r - 1]);
                --r;
            if(l != r) return;
            val = array[l], pos = l;
        }

        //ms = 3, what the hell is this
        void pickMush3(int &x, int &y){
            int mx = INT_MIN;
            int mn = INT_MAX;
            for(int i = 0; i < n; ++i){
                if(arrayTrans[i] > mx){
                    x = i;
                    mx = arrayTrans[i];
                }
                if(arrayTrans[i] < mn){
                    y = i;
                    mn = arrayTrans[i];
                }
            }
        }

        //ms = 4, the "second place" king
        void pickMush4(int &val, int &pos){
            val = -5;
            pos = -7;
            int mx = max(max(arrayTrans[0], arrayTrans[1]), arrayTrans[2]);
            int mn = min(min(arrayTrans[0], arrayTrans[1]), arrayTrans[2]);
            if(mx == mn) return;
            if(arrayTrans[0] == arrayTrans[1] || arrayTrans[0] == arrayTrans[2] || arrayTrans[1] == arrayTrans[2]){
                for(int i = 0; i < 3; ++i){
                    if(arrayTrans[i] < mx && arrayTrans[i] >= mn){
                        val = arrayTrans[i];
                        pos = i;
                        return;
                    }
                }
            }else{
                for(int i = 0; i < 3; ++i){
                    if(arrayTrans[i] < mx && arrayTrans[i] > mn){
                        val = arrayTrans[i];
                        pos = i;
                        return;
                    }
                }
            }
        }
    } ghostmush;

    void pickGhostmush(Knights &knight, string s){
        int list[maxEvent], numlist = 0;
        for(int i = 2; i < (int)s.size(); ++i)
            list[++numlist] = s[i] - '0';
        ghostmush.Input(l3files[0]);
        for(int i = 1; i <= numlist; ++i){
            int x, y;
            if(list[i] == 1)
                ghostmush.pickMush1(x, y);
            if(list[i] == 2)
                ghostmush.pickMush2(x, y);
            if(list[i] == 3)
                ghostmush.pickMush3(x, y);
            if(list[i] == 4)
                ghostmush.pickMush4(x, y);
            knight.HP = knight.HP - (x + y);
            knight.healthCheck();
            if(knight.Rescue == 0)
                return;
        }
    }

    //id = 15 -> 17, goods for everyone
    void pickItem(Knights &knight, int index){
        if(index == 15)
            knight.Remedy = min(knight.Remedy + 1, maxItem);
        if(index == 16)
            knight.MaidenKiss = min(knight.MaidenKiss + 1, maxItem);
        if(index == 17)
            knight.PhoenixDown = min(knight.PhoenixDown + 1, maxItem);
    }

    
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
    l3input(sinp);
    in.close();
}

void adventureToKoopa(string file_input, int & HP, int & level, int & remedy, int & maidenkiss, int & phoenixdown, int & rescue) {
    //cout << "Function isn't implemented" << endl;
    fileINP(file_input);
}