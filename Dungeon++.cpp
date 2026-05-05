#include <iostream>
#include <cstring>
#include <ctime>
#include <vector>
#include <iomanip>
using namespace std;

#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define GREEN   "\033[32m"
#define RESET   "\033[0m"

class Entity{

    protected:
    int HP;
    int level;
    string name;
    int strength;
    int defence;

    public:
    Entity(int h,int l,string n, int s, int d)
    {
        HP = h;
        level = l;
        name = n;
        strength = s;
        defence = d;
    }

    int getHP(){ return HP; }
    int getLevel(){ return level; }
    string getName(){ return name; }
    int getStrength(){ return strength; }
    int getDefence(){ return defence; }

    int Attack(int dam)
    {
        int att = dam;
        att *= (1 + (0.25*strength));
        return att;
    }

    void damage_taken(int dam)
    {
        dam -= defence;
        if(dam < 1)
            dam = 1;
        HP -= dam;
    }
};

class Hero:public Entity{
    vector<int> potions = {20};
    vector<string> buffs = {"Strength"};
    int gold;
    int EXP;
    int req;
    int weapon;
    int StrBuffA;
    int DefBuffA;
    int MaxHP;

    public:
    Hero(int h, int l, string n, int s, int d):Entity(h,l,n,s,d){
        cout<<GREEN<<"A Hero has been born!"<<RESET<<endl;
        MaxHP = h;
        gold = 150;
        EXP = 0;
        req = 100;
        weapon = 0;
        StrBuffA = 0;
        DefBuffA = 0;
    }

    int getGold(){ return gold; }
    int getExp(){ return EXP; }
    int GetExpReq(){ return req; }
    int getWeapon(){ return weapon; }
    int getMaxHP(){ return MaxHP; }

    void RestoreHP()
    {
        HP = MaxHP;
    }

    void AddGold(int rec)
    {
        gold += rec;
        cout<<YELLOW<<"You earned "<<rec<<" Gold!"<<RESET<<endl;
    }

    void AddExp(int e)
    {
        EXP += e;
        cout<<YELLOW<<"You got "<<e<<" EXP!"<<RESET<<endl;
        if(EXP >= req)
            LevelUp();
    }

    void LevelUp()
    {
        level++;
        MaxHP += 15;
        strength += 2;
        defence += 2;

        if(level % 3 == 0)
        {
            strength += 1;
            defence += 1;
            cout << YELLOW << "Bonus level! Extra STR and DEF gained!" << RESET << endl;
        }

        req *= 1.25;
        RestoreHP();

        cout << GREEN << "You Levelled Up to " << level << "!!!!" << endl;
        cout << "  HP:  +" << 15 << "  (Max: " << MaxHP << ")" << endl;
        cout << "  STR: +" << 2  << "  (Total: " << strength << ")" << endl;
        cout << "  DEF: +" << 2  << "  (Total: " << defence  << ")" << RESET << endl;
    }

    void inv_details(int InBattle)
    {
        cout << "=========================================" << endl;
        cout << "              INVENTORY"                   << endl;
        cout << "=========================================" << endl;
        cout << "  Potions: " << potions.size()             << endl;
        cout << "  Buffs:   " << buffs.size()               << endl;
        cout << "-----------------------------------------" << endl;
        cout << "[1] Potions" << endl;
        cout << "[2] Buffs"   << endl;
        cout << "[0] Back"    << endl;
        cout << "Choose: ";

        int c = 0;
        cin >> c;

        if(c == 1)
            view_potions();
        else if(c == 2)
            view_buffs(InBattle);
        else if(c == 0)
            return;
        else
            cout << "Invalid Choice!" << endl;
    }

    void view_potions()
    {
        cout << "=========================================" << endl;
        cout << "              POTIONS"                     << endl;
        cout << "=========================================" << endl;

        if(potions.empty())
        {
            cout << "  No potions in bag." << endl;
            cout << "=========================================" << endl;
            return;
        }

        int index = 1;
        for(auto it = potions.begin(); it != potions.end(); it++, index++)
            cout << "  [" << index << "] +" << *it << " HP" << endl;

        cout << "  [0] Back" << endl;
        cout << "-----------------------------------------" << endl;

        int h = 0;
        while(1)
        {
            cout << "Choose: ";
            cin >> h;

            if(h > 0 && h < index)
            {
                cout << GREEN << ">> Used potion! +" << potions[h-1] << " HP" << RESET << endl;
                HP += potions[h-1];
                potions.erase(potions.begin() + (h-1));
                break;
            }
            else if(h == 0)
                break;
            else
                cout << "Invalid Choice" << endl;
        }
        cout << "=========================================" << endl;
    }

    void view_buffs(int inBattle)
    {
        cout << "=========================================" << endl;
        cout << "               BUFFS"                      << endl;
        cout << "=========================================" << endl;

        if(buffs.empty())
        {
            cout << "  No buffs in bag." << endl;
            cout << "=========================================" << endl;
            return;
        }

        int index = 1;
        for(auto it = buffs.begin(); it != buffs.end(); it++, index++)
            cout << "  [" << index << "] " << *it << " Buff" << endl;

        cout << "  [0] Back" << endl;
        cout << "-----------------------------------------" << endl;

        if(inBattle == 0)
        {
            cout << YELLOW << "  Buffs can only be used during battle." << RESET << endl;
            cout << "=========================================" << endl;
            return;
        }

        int h = 0;
        while(1)
        {
            cout << "Choose: ";
            cin >> h;

            if(h > 0 && h < index)
            {
                string choice = buffs[h-1];
                buffs.erase(buffs.begin() + (h-1));
                ApplyBuff(choice);
                break;
            }

            else if(h == 0)
                break;

            else
                cout << "Invalid Choice!" << endl;
        }
        cout << "=========================================" << endl;
    }

    void add_potion(int h)
    {
        potions.push_back(h);
    }

    void add_buff(string h)
    {
        buffs.push_back(h);
    }

    void ApplyBuff(string c)
    {
        if(c == "Strength")
        {
            strength *= 2;
            StrBuffA += 1;
            cout<<"Strength Doubled for Battle!"<<endl;
        }

        else if(c == "Defence")
        {
            defence *= 2;
            DefBuffA += 1;
            cout<<"Defence Doubled for Battle!"<<endl;
        }
    }

    void RemoveBuff()
    {
        while(StrBuffA != 0)
        {
            strength /= 2;
            StrBuffA--;
        }

        while(DefBuffA != 0)
        {
            defence /= 2;
            DefBuffA--;
        }
    }

    void change_weapon(int NewIndex)
    {
        weapon = NewIndex;
    }

};

class Enemy:public Entity{
    int exp_given;
    int gold_given;

    public:
    Enemy(int h,int l, string n, int s, int d,int e, int g):Entity(h,l,n,s,d){
        exp_given = e;
        gold_given = g;
    }

    int getExp(){ return exp_given; }
    int getGoldGive(){ return gold_given; }
};

Enemy FinalBoss(550, 10, "Qual The Sage Of Corruption", 50, 40, 500, 300);
string enemies[] = {"Skeleton", "Bat", "Spider"};
int base_hp[] = {55,30,80};
int base_str[] = {5,8,2};
int base_def[] = {5,0,8};
int base_exp[] = {20,15,25};
int base_gold[] = {10, 5, 15};

string weapon_names[]  = {"Rusty Dagger", "Short Sword", "Broad Sword", "Battle Axe", "Shadow Blade"};
int    weapon_damage[] = {10,  18,  28,  38,  55 };
int    weapon_cost[]   = { 0,  80, 200, 400, 750 };

string potion_names[] = {"Small Potion", "Medium Potion", "Large Potion"};
int    potion_heal[]  = {20, 50, 100};
int    potion_cost[]  = {30, 70, 150};

string buff_names[] = {"Strength", "Defence"};
int    buff_cost[]  = {60, 60};

void enter()
{
    cin.get();
}

string intro()
{
    cout << "In a small quiet village, a young boy lived with his father..." << endl;
    cout << "His father, the great warrior Gary, watched over the village.";
    enter();

    cout <<GREEN<< "Gary"<<RESET<<": \"Son, come here.\"";
    enter();
    cout <<GREEN<< "Gary"<<RESET<<": \"I've been meaning to ask... what was your name again?\"" << endl;
    cout <<GREEN<< "Gary"<<RESET<<": \"Heh, forgetting names runs in the family I suppose.\"" << endl;

    string heroName;
    cout <<YELLOW<< "Enter your name: "<<RESET;
    cin >> heroName;
    cin.ignore();

    cout <<GREEN<< "Gary"<<RESET<<": \"Ah yes... " << heroName << ". How could I forget.\"" << endl;
    enter();

    cout << "That night, a thunderous roar shook the village.";
    enter();
    cout << "A dark shadow fell over every home." << endl;
    cout << "The ground trembled. Windows shattered." << endl;
    enter();

    cout <<RED<< "Qual, The Elder Sage of Corruption had come..."<<RESET;
    enter();

    cout << "The screams were short.";
    enter();
    cout << "Gary fought bravely.";
    enter();
    cout<<".....";
    enter();
    cout << "Gary did"<<RED<<" not"<<RESET<<" survive.";
    enter();

    cout << "Nobody did.";
    enter();
    cout << "Nobody... except " << heroName << "." << endl;
    enter();

    cout << heroName << " ran. Stumbling through the burning village," << endl;
    cout << "a"<<YELLOW<<" glint"<<RESET<<" caught his eye in the dirt.";
    enter();
    cout << "Gary's old Rusty Dagger. The only thing left of him.";
    enter();
    cout << heroName << " grabbed it and ran into the dark forest." << endl;
    cout << "Alone. Afraid. But alive." << endl;
    enter();

    cout << "And so, from the ashes of tragedy..." << endl;
    enter();

    return heroName;
}

void BattlePrint(Hero h, Enemy e)
{
    cout << "=========================================" << endl;
    cout << GREEN << left << setw(20) << h.getName() + " Lv." + to_string(h.getLevel())
         << RESET
         << RED << e.getName() + " Lv." + to_string(e.getLevel())
         << RESET << endl;
    cout << "-----------------------------------------" << endl;

    cout << left << setw(20) << ("HP:  " + to_string(h.getHP()) + "/" + to_string(h.getMaxHP()))
         << RED << "HP:  " << e.getHP() << RESET << endl;

    cout << left << setw(20) << ("STR: " + to_string(h.getStrength()))
         << "STR: " << e.getStrength() << endl;

    cout << left << setw(20) << ("DEF: " + to_string(h.getDefence()))
         << "DEF: " << e.getDefence() << endl;

    cout << left << setw(20) << ""
         << YELLOW << "EXP: " << e.getExp() << RESET << endl;

    cout << "=========================================" << endl;
}

void GetStats(Hero h)
{
    cout<<"========================================="<<endl;
    cout<<"NAME:  "<<GREEN<<h.getName()<<RESET<<"   LV: "<<h.getLevel()<<endl;
    cout<<"HP:    "<<h.getHP()<<"/"<<h.getMaxHP()<<endl;
    cout<<"STR:   "<<h.getStrength()<<endl;
    cout<<"DEF:   "<<h.getDefence()<<endl;
    cout<<"GOLD:  "<<YELLOW<<h.getGold()<<RESET<<endl;
    cout<<"EXP:   "<<h.getExp()<<"/"<<h.GetExpReq()<<endl;
    cout<<"WEAPON: "<<weapon_names[h.getWeapon()]<<"  DMG: "<<weapon_damage[h.getWeapon()]<<endl;
    cout<<"========================================="<<endl;
}

int BattleMenu()
{
    while(1)
    {
        int choice;
        cout<<"1: Fight"<<"   "<<"2: Bag"<<"   "<<"3: Run"<<endl;
        cout<<"Choose Action: ";
        cin>>choice;

        if(choice >= 1 && choice <= 3)
            return choice;
        else
            cout<<"invalid Choice"<<endl<<"Choose Again";
    }
}

void InBattle(Hero &h, Enemy e)
{
    while(e.getHP() > 0)
    {
        BattlePrint(h, e);

        int c = BattleMenu();
        int dam;

        if(c == 1)
        {
            dam = h.Attack(weapon_damage[h.getWeapon()]);
            e.damage_taken(dam);
            cout << GREEN << ">> " << h.getName() << " hits " << e.getName()
                 << " for " << dam << " damage!" << RESET << endl;

            if(e.getHP() <= 0)
            {
                h.RestoreHP();
                break;
            }
        }
        else if(c == 2)
            h.inv_details(1);
        else if(c == 3)
            break;

        dam = e.Attack(8);
        h.damage_taken(dam);
        cout << RED << ">> " << e.getName() << " hits " << h.getName()
             << " for " << dam << " damage!" << RESET << endl;

        if(h.getHP() <= 0)
        {
            cout << RED << ">> You were defeated..." << RESET << endl;
            break;
        }

        cout << "-----------------------------------------" << endl;
    }

    if(e.getHP() <= 0)
    {
        h.AddGold(e.getGoldGive());
        h.AddExp(e.getExp());
    }
}

Enemy gen_enemy(int type,int hero_level)
{
    cout<<"Hero Level is "<<hero_level<<endl<<endl;
    int t = (rand() % 7) - 3 + hero_level;

    if(t<1)
    {
        t = 1;
    }

    else if(t>10)
    {
        t = 10;
    }

    Enemy temp(base_hp[type]*t, t, enemies[type], base_str[type] *t, base_def[type] *t, base_exp[type]*t, base_gold[type]*t);
    return temp;
}

void Shop(Hero& h)
{
    while(1)
    {
        cout << "=========================================" << endl;
        cout << "              SHOP" << endl;
        cout << "  Gold: " << h.getGold() << endl;
        cout << "=========================================" << endl;
        cout << "[1] Weapons" << endl;
        cout << "[2] Potions" << endl;
        cout << "[3] Buffs"   << endl;
        cout << "[4] Leave"   << endl;
        cout << "Choose: ";

        int c;
        cin >> c;

        if(c == 1)
        {
            cout << "--- Weapons ---" << endl;
            for(int i = 0; i < 5; i++)
            {
                if(i == h.getWeapon())
                    cout << GREEN;
                else
                    cout << RED;

                cout << "[" << i+1 << "] " << weapon_names[i]
                     << "  DMG: " << weapon_damage[i]
                     << "  Cost: " << weapon_cost[i] << "g";

                if(i == h.getWeapon())
                    cout << "  <EQUIPPED>";

                cout << RESET << endl;
            }

            cout << "Choose weapon to buy (0 to back): ";
            int w;
            cin >> w;

            if(w >= 1 && w <= 5)
            {
                int id = w - 1;
                if(id == h.getWeapon())
                    cout << "Already equipped!" << endl;
                else if(id < h.getWeapon())
                    cout << "Can't buy a weaker weapon!" << endl;
                else if(h.getGold() < weapon_cost[id])
                    cout << "Not enough gold!" << endl;
                else
                {
                    h.AddGold(-weapon_cost[id]);
                    h.change_weapon(id);
                    cout << "Equipped " << weapon_names[id] << "!" << endl;
                }
            }
        }

        else if(c == 2)
        {
            cout << "--- Potions ---" << endl;
            for(int i = 0; i < 3; i++)
                cout << "[" << i+1 << "] " << potion_names[i]
                     << "  HP: +" << potion_heal[i]
                     << "  Cost: " << potion_cost[i] << "g" << endl;

            cout << "Choose potion to buy (0 to back): ";
            int p; cin >> p;

            if(p >= 1 && p <= 3)
            {
                int idx = p - 1;
                if(h.getGold() < potion_cost[idx])
                    cout << "Not enough gold!" << endl;
                else
                {
                    h.AddGold(-potion_cost[idx]);
                    h.add_potion(potion_heal[idx]);
                    cout << "Bought " << potion_names[idx] << "!" << endl;
                }
            }
        }

        else if(c == 3)
        {
            cout << "--- Buffs ---" << endl;
            for(int i = 0; i < 2; i++)
                cout << "[" << i+1 << "] " << buff_names[i]
                     << "  Cost: " << buff_cost[i] << "g" << endl;

            cout << "Choose buff to buy (0 to back): ";
            int b; cin >> b;

            if(b >= 1 && b <= 2)
            {
                int idx = b - 1;
                if(h.getGold() < buff_cost[idx])
                    cout << "Not enough gold!" << endl;
                else
                {
                    h.AddGold(-buff_cost[idx]);
                    // strip "Buff" suffix to match ApplyBuff's expected strings
                    string bname = (idx == 0) ? "Strength" : "Defence";
                    h.add_buff(bname);
                    cout << "Bought " << buff_names[idx] << "!" << endl;
                }
            }
        }

        else if(c == 4)
            break;

        else
            cout << "Invalid choice!" << endl;
    }
}

void BossFight(Hero& h, Enemy boss)
{
    cout << RED << "=========================================" << endl;
    cout << "   QUAL, ELDER SAGE OF CORRUPTION AWAITS..." << endl;
    cout << "=========================================" << RESET << endl;
    cout <<RED<<"Qual"<<RESET<<": \"You dare return, little rat?\"" << endl;
    cout <<RED<<"Qual"<<RESET<<": \"Your father begged too. It didn't help him.\"" << endl;
    cout << endl;

    InBattle(h, boss);

    if(boss.getHP() <= 0)
    {
        cout << GREEN << "=========================================" << endl;
        cout << "Qual crashes to the ground." << endl;
        cout << "The darkness lifts." << endl;
        cout << endl;
        cout << h.getName() << " stands over the body, breathing hard." << endl;
        cout << "Gary is avenged." << endl;
        cout << endl;
        cout << "But somewhere out there, darker things stir." << endl;
        cout << "Qual was never the real threat." << endl;
        cout << "He was just the beginning." << endl;
        cout << endl;
        cout << h.getName() << "'s journey has only just begun..." << endl;
        cout << "=========================================" << RESET << endl;
        enter();
        exit(0);
    }
    else
    {
        cout << RED<<"You weren't strong enough..."<<RESET<<" Qual still lives." << endl;
        cout << "Live to fight another day." << endl;
    }
}

int main()
{

    srand(time(NULL));
    string name;
    name = intro();
    Hero h(100,1,name,5,5);
    int ch = 1;

    while(1)
    {
        GetStats(h);
        cout<<"[1] New enemy"<<endl;
        cout<<"[2] Open inventory"<<endl;
        cout<<"[3] Shop"<<endl;
        cout<<"[4] Reclaim The Village"<<endl;
        cout<<"[5] Quit"<<endl;
        cout<<"Choose: ";
        cin>>ch;

        if(ch==1)
        {
            Enemy e1 = gen_enemy(rand() % 3,h.getLevel());
            InBattle(h,e1);

        }

        else if(ch == 2)
        {
            h.inv_details(0);

        }

        else if(ch == 3)
        {
            Shop(h);
        }

        else if(ch == 4)
        {
            BossFight(h, FinalBoss);
        }

        else if(ch == 5)
        {
            cout<<"Thanks for playing";
            break;
        }

        if(h.getHP() <= 0)
        {
            cout<<RED<<"Game Over!!"<<RESET<<endl;
            enter();
            break;
        }
    }
    getchar();

    return 0;
}
