#include "knight.h"

//some constants
const int maxLevel = 10;
const int maxItem = 99;
const int maxEvent = (int)2e5 + 7; //why it doesn't have a max number of events
int fib[40];
string l3files[3]={"", "", ""};

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
    cout << file_input << endl;
    int tmp = 0;
    cout << l3files[0] << " " << l3files[1] << " " << l3files[2] << endl;
    for(int i = 0; i < (int)file_input.size(); ++i){
        //cout<<file_input[i]<<endl;
        if(file_input[i] != ','){
            l3files[n] = l3files[n] + file_input[i];
        }
        else{
            ++n;
        }
    }
}

//I served with his majesty
struct Knights{
    int HP, Level, Remedy, MaidenKiss, PhoenixDown; //always update this
    int Rescue = -1; //fuck me in the ass
    int prevLevel;
    int maxHP = 999;
    int ghostno = 1;

    bool metAsclepius = 0;
    bool metMerlin = 0;
    bool isArthur = 0;
    bool isLancelot = 0;

    void Input(string s){
        stringstream ss(s);
        ss >> HP >> Level >> Remedy >> MaidenKiss >> PhoenixDown;
        maxHP = HP;

        if(HP == 999) 
            isArthur = 1;
        if(isPrime(HP)) 
            isLancelot = 1;
    }

    void healthCheck(){
        if (HP <= 0 && PhoenixDown == 0) 
            Rescue = 0;
        else if(HP <= 0 && PhoenixDown > 0){
            --PhoenixDown;
            HP = maxHP;
        }
    }

    void display(int index){
        cout << "HP=" << HP
            << ", level=" << Level
            << ", remedy=" << Remedy
            << ", maidenkiss=" << MaidenKiss
            << ", phoenixdown=" << PhoenixDown
            << ", rescue=" << Rescue  <<endl;
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
        string evet;
        //cout << s <<endl;
        while(ss >> evet){
            //cout << evet << endl;
            if((int)evet.size() >= 2 && evet[0] == '1' && evet[1] == '3'){
                ListOfEvents[++numEvents] = 13;
                GhostMush[++numGmEvents] = evet;
            }
            else
                ListOfEvents[++numEvents] = strtoi(evet);
            //  cout << numEvents << endl;
            //  for(int i = 1; i <= numEvents; ++i)
            //      cout << ListOfEvents[i] << " ";
            //  cout << endl;
        }
    }
    
    //id = 0, I've won, but at what cost?
    void success(Knights &knight){
        knight.Rescue = 1;
    }

    int levelCalculate(int index){
        int b = index % 10;
        return (index > 6 ? (b > 5 ? b : 5) : b);
    }

    //id = 1 -> 5, you weak mtfk
    void meetNormal(Knights &knight, int mons, int index){
        int level0 = levelCalculate(index);
        //cout << "id = " << index <<", level = " << level0<<endl;

        if (knight.Level > level0 || knight.isArthur || knight.isLancelot){
            knight.Level = min(maxLevel, knight.Level + 1);
            return;
        }
        if(knight.Level < level0){
            int baseDamage[6] = {0, 10, 15, 45, 75, 95};
            int damage = level0 * baseDamage[mons];
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
            return; //nice
        }
        if(knight.Level < level0)
            knight.turnTiny();
    }

    //id = 7, meet Siren Vajsh, and can be turn into frog and wait for a chick to suck dick
    void meetSirenVajsh(Knights &knight, int index){
        if(knight.tiny.isTiny || knight.frog.isFrog) return; //became tiny/frog, don't fight
        int level0 = levelCalculate(index);

        if(knight.Level > level0 || knight.isArthur || knight.isLancelot){
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
            int mx = -2147483648;
            int mn = 2147483647;
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
            int mx = -2147483648;
            int mn = 2147483647;
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
        cout << s << endl;
        for(int i = 2; i < (int)s.size(); ++i){
            list[++numlist] = s[i] - '0';
            cout << list[numlist] <<endl;
        }
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

    //id = 19, Asclepius with a dildo
    void meetAsclepius(Knights &knight){
        knight.metAsclepius = 1;
        ifstream asc(l3files[1]);

        int n, m;
        asc >> n >> m;
        for(int i = 1; i <= n; ++i){
            int cnt = 0;
            for(int j = 1; j <= m; ++j){
                int x;
                asc >> x;
                if(cnt != 3 && (16 <= x && x <= 18)){
                    ++cnt;
                    pickItem(knight, x - 1);
                }
                if(cnt == 3)
                    break;
            }
        }
    }

    //id = 18, witch, kill him
    void meetMerlin(Knights &knight){
        knight.metMerlin = 1;
        ifstream mer(l3files[2]);
        int n;
        mer >> n;
        string Merlin = "Merlin";
        string merlin = "merlin";
        for(int i = 0; i < n; ++i){
            string s;
            mer >> s;
            bool check[256] = {};
            if((int)s.size() < 6) continue;
            for(int j = 0; j < (int)s.size(); ++j)
                check[s[j]] = 1;
            if ((check['m'] || check['M']) && 
                (check['e'] || check['E']) && 
                (check['r'] || check['R']) && //nice
                (check['l'] || check['L']) && 
                (check['i'] || check['I']) && 
                (check['n'] || check['N'])){
                    knight.HP = min(knight.maxHP, knight.HP + 2);
                }
            for(int i = 0; i < (int)s.size() - 5; ++i){
                string tmp = s.substr(i, 6);
                if(tmp == merlin || tmp == Merlin)
                    knight.HP = min(knight.maxHP, knight.HP + 1);
            }
        }
        mer.close();
    }

    //id = 99, Bowsette my queen
    void meetBowser(Knights &knight){
        if(knight.isArthur || (knight.isLancelot && knight.Level >= 8) || knight.Level == maxLevel)
            knight.Level = maxLevel;
        else
            knight.Rescue = 0;
    }

    //finally, I can write this function
    void meetEvent(Knights &knight, int index, int eventid){
        if(index == 0)
            success(knight);
        if(1 <= index && index <= 5)
            meetNormal(knight, index, eventid);
        if(index == 6)
            meetShaman(knight, eventid);
        if(index == 7)
            meetSirenVajsh(knight, eventid);
        if(index == 11)
            pickMariomush(knight);
        if(index == 12)
            pickFibmush(knight);
        if(index == 13)
            pickGhostmush(knight, GhostMush[knight.ghostno++]);
        if(15 <= index && index <= 17)
            pickItem(knight, index);
        if(index == 18 && !knight.metMerlin)
            meetMerlin(knight);
        if(index == 19 && !knight.metAsclepius)
            meetAsclepius(knight);
        if(index == 99)
            meetBowser(knight);
    }
} event;


void fileINP(string file_input){
    ifstream in(file_input);
    string sinp; //don't ask why I named this
    getline(in, sinp);
    knight.Input(sinp);

    getline(in, sinp);
    event.Input(sinp);
    for(int i = 1; i <= event.numEvents; ++i)
        cout << event.ListOfEvents[i] << " ";
    cout << endl << event.numEvents << endl;
    cout << event.numGmEvents << endl;
    for(int i = 1; i <= event.numGmEvents; ++i)
        cout << event.GhostMush[i] << " ";
    getline(in, sinp);
    cout << sinp << endl;
    l3input(sinp);
    cout << l3files[0] << " " << l3files[1] << " " << l3files[2] <<endl;
    in.close();
}

void adventureToKoopa(string file_input, int & HP, int & level, int & remedy, int & maidenkiss, int & phoenixdown, int & rescue) {
    fibo();
    fileINP(file_input);
    //freopen("test.txt","w",stdout);

    int eventid = 1;
    while(knight.Rescue == -1 && eventid <= event.numEvents){
        event.meetEvent(knight, event.ListOfEvents[eventid], eventid);
        //knight.healthCheck();
        if(eventid >= event.numEvents && knight.HP > 0){
            knight.Rescue = 1;
        }
        knight.display(event.ListOfEvents[eventid]);
        eventid++;
    }
    //knight.display(0);
}