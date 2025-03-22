#include<iostream>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<string>
#include<chrono>
#include<thread>
#include<fstream>
#include<random>
#include<array>
#include<limits>
#include<iterator>
#include<utility>
#include<future>
#include<cmath>
#include<unordered_map>
#include<map>
#include<memory>
#include<iomanip>
#include<variant>
#include<algorithm>
#include<tuple>

using namespace std;

random_device RG;
mt19937 RandGen(RG());

template <typename T>
class range{//Range of values range<T>(MIN, MAX)
public:

    T MIN;
    T MAX;

    range(T Min = 0, T Max = 0):
    MIN(Min), MAX(Max) {}

    friend range<T> operator+(range<T> first, range<T> second) {
        return {first.MIN + second.MIN, first.MAX + second.MAX};
    }

};

    template <typename T, typename... Args>
    range<T> SumRanges(range<T> first, Args... rest) {
        return first + SumRanges(rest...);
    }

    template <typename T>
    range<T> SumRanges(range<T> range) {
        return range;
    }

    template <typename T>
    range<T> RangeNum(T num, T multiply) {
    range<T> Result(0, 0);
        Result.MAX = num * multiply;
        Result.MIN = num * multiply;
        return Result;
    }

    void printSlowly(const string& msg, int time) {
        for (char k : msg) {
            cout << k << flush;
            this_thread::sleep_for(chrono::milliseconds(time));
        }
    }

    void enter(const string& write, int time){
    string temp;
        printSlowly(write, time);
        cout << "\nPress enter to continue..." << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, temp);
    }

    int VerifyCin(int MIN, int MAX){
    int num;

        while(true){
            cin >> num;

            if (cin.fail() || num < MIN || num > MAX) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "[INVALID OPTION]" << endl;
            }
            else{
                return num;
            }
        }
    }


    int VerifyCinEquip(){
        cout << "1.WEAPONS" << endl;
        cout << "2.ARMORS" << endl;
        cout << "3.GLOVES" << endl;
        cout << "4.NECKLES" << endl;
        cout << "5.GO BACK" << endl;
        return VerifyCin(1, 5);
    }

    float NumRound(float num){
        return roundf(num * 100) / 100;
    }

    template <typename T>
    T DistReal(float Min, float Max){
        if(Min > Max){
            swap(Max, Min);
        }
        uniform_real_distribution<float> DistN(Min, Max);
        float result = DistN(RandGen);

        if(is_same<T, int>::value){
            return static_cast<int>(ceil(result));
        }
        else{
            return NumRound(result);
        }
    }

    template <typename T>
    T DistNormal(float Min, float Max){
    float result;
    int attempts = 0;

        if (Min > Max) {
            swap(Max, Min);
        }

        float mean = (Min + Max) / 2;
        float stddev = (Max - Min) / 6;

        normal_distribution<float> NormalDist(mean, stddev);

        do{
        result = NormalDist(RandGen);
        ++attempts;
        }while((result < Min || result > Max) && attempts < 10);

        if(attempts >= 10){
            result = DistReal<T>(Min, Max);
        }

        if(is_same<T, int>::value){
            return static_cast<int>(ceil(result));
        }
        else{
            return NumRound(result);
        }

    }

    template <typename T>
    pair<T, int> DiscreteDist(vector<pair<T, int>> Target){
        vector<int> Memory;
        for (const auto& tar : Target) {
        Memory.push_back(tar.second);
        }
        discrete_distribution<int> RandVec(Memory.begin(), Memory.end());
        int index = RandVec(RandGen);
        T selected = Target[index].first;
        return make_pair(selected, index);
    }

    template <typename T>
    T SingleStats(T stats, float rarity, int level){
        float prog = 1 + (level * 0.05);//level progression 5% per level
        float base = stats * prog;
        float variable = base * 0.05;//variable = variaton% of base
        float VariantMin = (base * rarity) - variable;
        float VariantMax = (base * rarity) + variable;
        float result = DistReal<T>(VariantMin, VariantMax);

        if(is_same<T, int>::value){
            return static_cast<int>(ceil(result));
        }
        else{
            return NumRound(result);
        }
    }

    template <typename T>
    range<T> RangeStats(T stats, float rarity, int level){
        range<T> result;
        float variable = stats * 0.1;
        float VariantMin = stats - variable;
        float VariantMax = stats + variable;
        result.MIN = SingleStats<T>(VariantMin, rarity, level);
        result.MAX = SingleStats<T>(VariantMax, rarity, level);

        if(result.MIN > result.MAX){
            swap(result.MIN, result.MAX);
        }
        return result;
    }

    string Percent(float num){
        return to_string(NumRound(num * 100)) + "%";
    }

    int LvDet(int LV) {
        int range = LV > 2 ? 2 : LV;
        int minLV = max(1, LV - range);
        return DistReal<int>(minLV, LV + range);
    }

    string EnemyNameGen(string Type){

        unordered_map<string, int> Creature{
        {"Angel", 0},
        {"Demon", 1},
        {"Human", 2},
        {"Creature", 3}
        };

        int TypeX = Creature.count(Type) ? Creature[Type] : 4;

        vector<vector<string>> name1 = {
        {"Assassin", "Holy", "Corupted", "Canibal", "Hollow"},
        {"Inocent", "Torturer", "Bloody", "Sad", "Eyeless"},
        {"Raped", "Butcher", "Lost", "Tormented", "Masoquist", "Abussed"},
        {"Hairy", "Good", "Gentle", "Confused", "Crying"},
        {"Unknow", "Mysterious", "Inexistent"}
        };

        vector<vector<string>> name2 = {
        {"Fallen", "Atheist", "Ocult", "Stalker", "Ill"},
        {"Calm", "Agressive", "Giant", "Depressed", "Abandoned"},
        {"Bipolar", "Schizophrenic", "Suicidal", "Psycho", "Maniac"},
        {"Spiritual", "Undead", "Smoker", "Obese", "Cancerous"},
        {"Sadist", "Evil", "Tormented", "Headless"}
        };

        vector<vector<string>> name3 = {
        {"Angel", "Dark Angel", "Cherurim", "Seraphim"},
        {"Demon", "Imp", "Pithius", "Mammom", "Mamodo"},
        {"Child", "Mother", "Beggar", "Sinner", "Religious", "Nymph"},
        {"Chimera", "Mutant", "Abomination", "Deformed Girl", "Inocence Eater"},
        {"Fisher", "Ovarium", "Lier", "Soul"}
        };

        uniform_int_distribution<int> Rname_A(0, name1[TypeX].size() - 1);
        uniform_int_distribution<int> Rname_B(0, name2[TypeX].size() - 1);
        uniform_int_distribution<int> Rname_C(0, name3[TypeX].size() - 1);

        int First = Rname_A(RandGen);
        int Middle = Rname_B(RandGen);
        int Last = Rname_C(RandGen);

        string RandName = name1[TypeX][First] + " " + name2[TypeX][Middle] + " " + name3[TypeX][Last];
        return RandName;
    }

    string HabNameEnemy(string type) {

        unordered_map<string, int> TypeName{
        {"Angel", 0},
        {"Demon", 1},
        {"Human", 2},
        {"Creature", 3}
        };

        int HabIndex = TypeName.count(type) ? TypeName[type] : 4;

        vector<vector<string>> hab01 = {
        {"Holy", "Unsainted", "Faitfull", "Brain"},
        {"Profane", "Satanic", "Demoniac", "Cruel"},
        {"Strong", "Quick", "Lacerating", "Furious"},
        {"Venomous", "Feral", "Brutal", "Poisoned"},
        {"Ilusional", "Deprived", "Inexistent", "Manipulated"}
        };

        vector<vector<string>> hab02 = {
        {"Devour", "Swallow", "Crush", "Corruption"},
        {"Scratch", "Hit", "Curse", "Lick"},
        {"Slash", "Butt", "Cut", "Stroke"},
        {"Bite", "Claws", "Tail", "Fangs"},
        {"Eyes", "Manipulation", "Stomp", "Slash"}
        };

        uniform_int_distribution<int> rand01(0, hab01[HabIndex].size() - 1);
        uniform_int_distribution<int> rand02(0, hab02[HabIndex].size() - 1);

        string RandHabName = hab01[HabIndex][rand01(RandGen)] + " " + hab02[HabIndex][rand02(RandGen)];
        return RandHabName;

    }

    pair<string, int> CurseGen(int level){

        struct parameters{

            string curse;
            int chance;
            int damage;

        };

        vector<parameters> CURSE{
        {"Inexistence", 3, 10},
        {"Schizophrenia", 10, 9},
        {"Borderline", 20, 8},
        {"Social Anxiety", 26, 7},
        {"Psychosis", 28, 6},
        {"Bipolar", 32, 5},
        {"Panic", 36, 4},
        {"Anxiety", 40, 3},
        {"Depression", 42, 2}
        };

        vector<int> Curses;
        for (const auto& cur : CURSE) {
        Curses.push_back(cur.chance);
        }
        discrete_distribution<int> CURSE_X(Curses.begin(), Curses.end());

        string RandCurse = CURSE[CURSE_X(RandGen)].curse;

        int Damage = CURSE[CURSE_X(RandGen)].damage;

        int CurseDamage = SingleStats<int>(Damage, 1.0f, level);

        return make_pair(RandCurse, CurseDamage);
    }

    tuple<string, int, int> SealGen(int level){

        struct parameters{

            string seal;
            int chance;
            int damage;
            int DmgChance;

        };

        vector<parameters> SEAL{
        {"SAMAEL", 3, 26, 40},
        {"METATRON", 5, 25, 41},
        {"AMON", 7, 24, 42},
        {"HARARIEL", 9, 23, 43},
        {"LUDOCIEL", 12, 22, 44},
        {"URIEL", 15, 21, 45},
        {"GAHBRIEL", 18, 20, 46},
        {"LEOMON", 21, 19, 47},
        {"BAPHOMET", 24, 18, 48},
        {"MIRAI", 27, 17, 49},
        {"SALATIEL", 30, 16, 50}
        };

        vector<int> Seals;

        for (const auto& cur : SEAL) {
        Seals.push_back(cur.chance);
        }
        discrete_distribution<int> SEAL_X(Seals.begin(), Seals.end());

        string RandSeal = SEAL[SEAL_X(RandGen)].seal;

        int DealChance = SEAL[SEAL_X(RandGen)].DmgChance;
        int SealChance = SingleStats<int>(DealChance, 1.0f, level);

        int Damage = SEAL[SEAL_X(RandGen)].damage;
        int SealDamage = SingleStats<int>(Damage, 1.0f, level);

        return make_tuple(RandSeal, SealDamage, SealChance);
    }

    pair<string, float> RarityGen(){

        vector<pair<string, pair<int, float>>> RARITY{
        {"[BR][Broken]", {100, 0.98}},
        {"[C][Commom]", {95, 1.00}},
        {"[UC][Uncommon]", {90, 1.02}},
        {"[N][Normal]", {85, 1.04}},
        {"[R][Rare]", {80, 1.06}},
        {"[SR][Super rare]", {75, 1.08}},
        {"[SSR][Super Super Rare]", {50, 1.10}},
        {"[UR][Ultra Rare]", {40, 1.13}},
        {"[MR][Mega Rare]", {30, 1.15}},
        {"[MSR][Mega Super Rare]", {10, 1.18}},
        {"[XR][EXTRE RARE]", {5, 1.20}}
        };

        vector<int> chance;
        for (const auto& rar : RARITY){
        chance.push_back(rar.second.first);
        }
        discrete_distribution<int> RandRarity(chance.begin(), chance.end());

        int index = RandRarity(RandGen);

        string Rarity = RARITY[index].first;

        float Power = RARITY[index].second.second;
        float Value = NumRound(Power);

        return make_pair(Rarity, Value);
    }

class resist{
public:

    string Resist;
    float Value;

    resist(): Resist("Empty"), Value(1.0f) {}

    resist(string weak, float val):
    Resist(weak), Value(val) {}

};

class body{
public:

    string Part;
    range<int> HP;
    bool Dead;

    body():Part("Empty"), HP{}, Dead(true) {}

    body(string pt, range<int> hp, bool dead):
    Part(pt), HP(hp), Dead(dead) {}

};

class skill{
public:

    string Name;
    string Info;
    range<int> Level;

    skill(): Name("Empty"), Info("None"), Level{} {}

    skill(string name, string info, range<int> lv):
    Name(name), Info(info), Level(lv) {}

};

class skills{
public:

    int POINTS;
    vector<skill> BUFFS;
    vector<skill> KABBALAH;

    skills(): POINTS(0), BUFFS{}, KABBALAH{} {}

};

class factions{
public:

    string Name;
    string Reputation;
    range<int> Trust;
    int Honor;
    int Coins;
    int Level;

    factions(): Name("Empty"), Reputation("None"),
    Trust{0, 120}, Honor(0), Coins(0), Level(1) {}

    factions(string name, string rep,
    range<int> trust, int honor, int coin, int xp):
    Name(name), Reputation(rep), Trust(trust),
    Honor(honor), Coins(coin), Level(xp) {}

};

class ability{
public:

    string Name_AB;
    array<string, 2> Type_AB;
    string Rarity_AB;
    float DMG_AB;
    float MAG_AB;
    float Heal_AB;
    int Times_AB;//how many times hab will be used.
    int AP_Cost;
    int MP_Cost;

    ability(): Name_AB("Empty"), Type_AB{"None", "None"}, Rarity_AB("None"), DMG_AB(0), MAG_AB(0),
    Heal_AB(0), Times_AB(0), AP_Cost(0), MP_Cost(0) {}

    ability(string nam, array<string, 2> type, string rar, float dmg,
    float mag, float heal, int time, int ap, int mp):
    Name_AB(nam), Type_AB(type), Rarity_AB(rar), DMG_AB(dmg), MAG_AB(mag),
    Heal_AB(heal), Times_AB(time), AP_Cost(ap), MP_Cost(mp) {}

    void AbilityInfo(){

        cout << "\n[ABILITY INFORMATION]" << endl;
        cout << "[" << Name_AB << "]" << endl;
        cout << "[Rarity]:" << Rarity_AB << endl;
        cout << "[DMG:" << DMG_AB << "]" << endl;
        cout << "[MAG:" << MAG_AB << "]"  << endl;
        cout << "[HEAL:" << Heal_AB << "]"  << endl;
        cout << "[Times:" << Times_AB << "]" << endl;
        cout << "[AP Cost:" << AP_Cost << "]" << endl;
        cout << "[MP Cost:" << MP_Cost << "]" << endl;
        cout << "[ABILITY TYPE]:" << endl;
        cout << "[Class:" << Type_AB[0] << "]" << endl;
        cout << "[Type:" << Type_AB[1] << "]\n" << endl;
    }

};

class necklace{
public:

    string Name_N;
    string Type_N;
    string Rarity_N;
    int HP_N;
    range<int> ATK_N;
    range<int> MAG_N;
    int LUCK_N;
    int LV_N;
    int Value_N;

    necklace(): Name_N("Empty"), Type_N("None"), Rarity_N("None"), HP_N(0),
    ATK_N{}, MAG_N{}, LUCK_N(0), LV_N(0), Value_N(0) {}

    necklace(string nam, string type, string rar, int hp,
    range<int> atk, range<int> mg, int agl, int lv, int cost):
    Name_N(nam), Type_N(type), Rarity_N(rar), HP_N(hp),
    ATK_N(atk), MAG_N(mg), LUCK_N(agl), LV_N(lv), Value_N(cost) {}

    void NeckleInfo(){

        cout << "\n[NECKLE INFORMATION]" << endl;
        cout << "[" << Name_N << "]" << endl;
        cout << "[Type:" << Type_N << "]" << endl;
        cout << "[Rarity]:" << Rarity_N << endl;
        cout << "[HP:" << HP_N << "]" << endl;
        cout << "[ATK:" << ATK_N.MIN << "/" << ATK_N.MAX << "]" << endl;
        cout << "[MAG:" << MAG_N.MIN << "/" << MAG_N.MAX << "]" << endl;
        cout << "[LUCK:" << LUCK_N << "]" << endl;
        cout << "[LV:" << LV_N << "]" << endl;
        cout << "[Value:" << Value_N << " Souls]\n" << endl;

    }

};

class gloves{
public:

    string Name_GL;
    string Type_GL;
    string Rarity_GL;
    range<int> ATK_GL;
    range<int> MAG_GL;
    range<int> DEF_GL;
    float CRIT_GL;
    range<int> PREC_GL;
    range<int> DODGE_GL;
    int LV_GL;
    int Value_GL;

    gloves(): Name_GL("Empty"), Type_GL("None"), Rarity_GL("None"),
    ATK_GL{}, MAG_GL{}, DEF_GL{}, CRIT_GL(0), PREC_GL{}, LV_GL{}, Value_GL(0) {}

    gloves(string nam, string type, string rar, range<int> at, range<int> mg,
    range<int> def, float ct, range<int> prec, range<int> dog, int lv, int cost):
    Name_GL(nam), Type_GL(type), Rarity_GL(rar), ATK_GL(at), MAG_GL(mg), DEF_GL(def),
    CRIT_GL(ct), PREC_GL(prec), DODGE_GL(dog), LV_GL(lv), Value_GL(cost) {}

    void GlovesInfo(){

        cout << "\n[GLOVES INFORMATION]" << endl;
        cout << "[" << Name_GL << "]" << endl;
        cout << "[TYPE:" << Type_GL << "]" << endl;
        cout << "[RARITY]:" << Rarity_GL << endl;
        cout << "[ATK:" << ATK_GL.MIN << "/" << ATK_GL.MAX << "]" << endl;
        cout << "[MAG:" << MAG_GL.MIN << "/" << MAG_GL.MAX << "]" << endl;
        cout << "[DEF:" << DEF_GL.MIN << "/" << DEF_GL.MAX << "]" << endl;
        cout << "[CRIT:" << Percent(CRIT_GL) << "]" << endl;
        cout << "[PREC:" << PREC_GL.MIN << "/" << PREC_GL.MAX << "]" <<  endl;
        cout << "[DODGE:" << DODGE_GL.MIN << "/" << DODGE_GL.MAX << "]" <<  endl;
        cout << "[LV:" << LV_GL << "]" << endl;
        cout << "[VALUE:" << Value_GL << " SOULS\n" << endl;

    }

};

class armor{
public:

    string Name_A;
    string Type_A;
    string Rarity_A;
    int HP_A;
    range<int> ATK_A;
    range<int> MAG_A;
    range<int> DEF_A;
    range<int> DODGE_A;
    int AP_A;
    int MP_A;
    int LV_A;
    int Value_A;

    armor(): Name_A("Empty"), Type_A("None"), Rarity_A("None"), HP_A(0), ATK_A{},
    MAG_A{}, DEF_A{}, DODGE_A{}, AP_A(0), MP_A(0), LV_A(0), Value_A(0) {}

    armor(string nam, string type, string rar, int hp, range<int> atk, range<int> mag,
    range<int> def, range<int> dog, int ap, int mp, int lv, int cost):
    Name_A(nam), Type_A(type), Rarity_A(rar), HP_A(hp), ATK_A(atk), MAG_A(mag),
    DEF_A(def), DODGE_A(dog), AP_A(ap), MP_A(mp), LV_A(lv), Value_A(cost) {}

    void ArmorInfo(){

        cout << "\n[ARMOR INFORMATION]" << endl;
        cout << "[" << Name_A << "]" << endl;
        cout << "[TYPE:" << Type_A << "]" << endl;
        cout << "[RARITY]:" << Rarity_A << endl;
        cout << "[HP:" << HP_A << "]" << endl;
        cout << "[DEF:" << DEF_A.MIN << "/" << DEF_A.MAX << "]" << endl;
        cout << "[ATK:" << ATK_A.MIN << "/" << ATK_A.MAX << "]" << endl;
        cout << "[MAG:" << MAG_A.MIN << "/" << MAG_A.MAX << "]" << endl;
        cout << "[DODGE:" << DODGE_A.MIN << "/" << DODGE_A.MAX << "]" << endl;
        cout << "[AP:" << AP_A  << "]" << endl;
        cout << "[MP:" << MP_A  << "]" << endl;
        cout << "[LV:" << LV_A << "]" << endl;
        cout << "[VALUE:" << Value_A << " SOULS]\n" << endl;
    }

};

class weapon{
public:

    string Name_W;
    array<string, 2> Type_W;
    string Curse_W;
    string Rarity_W;
    range<int> ATK_W;//melee damage
    range<int> MAG_W;//magic damage
    range<int> CURSE_DMG_W;
    float CRIT_W;//critic damage
    int LUCK_W;//critic chance
    range<int> PREC_W;//precision, chance of executing the hability
    int LV_W;//weapon level
    int Value_W;

    weapon(): Name_W("Empty"), Type_W{}, Curse_W("None"), Rarity_W("None"), ATK_W{},
    MAG_W{}, CURSE_DMG_W{}, CRIT_W(0), LUCK_W(0), PREC_W{}, LV_W(0), Value_W(0) {}

    weapon(string nam, array<string, 2> type, string curse, string rar, range<int> at,
    range<int> mg, range<int> dmg, float crit, int luck, range<int> prec, int lv, int cost):
    Name_W(nam), Type_W(type), Curse_W(curse), Rarity_W(rar), ATK_W(at), MAG_W(mg), CURSE_DMG_W(dmg),
    CRIT_W(crit), LUCK_W(luck), PREC_W(prec), LV_W(lv), Value_W(cost) {}

    void WeaponInfo(){

        cout << "\n[WEAPON INFORMATION]" << endl;
        cout << "[" << Name_W << "]" << endl;
        cout << "[CLASS:" << Type_W[0] << "]" << endl;
        cout << "[TYPE:" << Type_W[1] << "]" << endl;
        cout << "[CURSE:" << Curse_W << "]" << endl;
        cout << "[RARITY]:" << Rarity_W << endl;
        cout << "[ATK:" << ATK_W.MIN << "/" << ATK_W.MAX << "]" << endl;
        cout << "[MAG:" << MAG_W.MIN << "/" << MAG_W.MAX << "]" << endl;
        cout << "[CURSE DMG:" << CURSE_DMG_W.MIN << "/" << CURSE_DMG_W.MAX << "]" << endl;
        cout << "[CRIT:" << Percent(CRIT_W) << "]" << endl;
        cout << "[LUCK:" << LUCK_W << "%]" << endl;
        cout << "[PREC:" << PREC_W.MIN << "/" << PREC_W.MAX << "]" << endl;
        cout << "[LV:" << LV_W << "]" << endl;
        cout << "[VALUE:" << Value_W << " SOULS" << "]\n" << endl;

    }

};

class gun{
public:

    string Name_G;
    string Type_G;
    string Seal_G;
    string Caliber_G;
    string Rarity_G;
    int BURST_G;
    range<int> DAMAGE_G;//Bullet damage
    range<int> ENCHANT_G;//Enchantment damage
    range<int> SEAL_DMG_G;
    int SEAL_CHANCE_G;
    float LETHAL_G;//critic damage
    int HANDLE_G;// critic chance
    range<int> AIM_G;//precision, hiting the target
    int LV_G;//weapon level
    int Value_G;
    range<int> AMMO_G;

    gun(): Name_G("Empty"), Type_G("None"), Seal_G("None"), Caliber_G("None"), Rarity_G("None"), BURST_G(0),
    DAMAGE_G{}, ENCHANT_G{}, SEAL_DMG_G{}, SEAL_CHANCE_G(0), LETHAL_G(0.0), HANDLE_G{}, AIM_G{},
    LV_G(0), Value_G(0), AMMO_G{} {}

    gun(string nam, string ty, string seal, string cal, string rar, int bur, range<int> dmg,
    range<int> ench, range<int> S_dmg, int S_cha, float let, int hand,
    range<int> iam, int lv, int cost, range<int> am):
    Name_G(nam), Type_G(ty), Seal_G(seal), Caliber_G(cal), Rarity_G(rar), BURST_G(bur), DAMAGE_G(dmg),
    ENCHANT_G(ench), SEAL_DMG_G(S_dmg), SEAL_CHANCE_G(S_cha), LETHAL_G(let), HANDLE_G(hand),
    AIM_G(iam), LV_G(lv), Value_G(cost), AMMO_G(am) {}


    void GunInfo(){

        cout << "\n[GUN INFORMATION]" << endl;
        cout << "[" << Name_G << "]" << endl;
        cout << "[Type: " << Type_G << "]" << endl;
        cout << "[Seal: " << Seal_G << "]" << endl;
        cout << "[Caliber: " << Caliber_G << "]" << endl;
        cout << "[BURST: " << BURST_G << "]" << endl;
        cout << "[DAMAGE: " << DAMAGE_G.MIN << "/" << DAMAGE_G.MAX << "]" << endl;
        cout << "[ENCHANT: " << ENCHANT_G.MIN << "/" << ENCHANT_G.MAX << "]" << endl;
        cout << "[SEAL DMG: " << SEAL_DMG_G.MIN << "/" << SEAL_DMG_G.MAX << "]" << endl;
        cout << "[LETALITY: " << Percent(LETHAL_G) << "]" << endl;
        cout << "[HANDLE: " << HANDLE_G << "]" << endl;
        cout << "[AIMING: " << AIM_G.MIN << "/" << AIM_G.MAX << "]" << endl;
        cout << "[AMMO: " << AMMO_G.MIN << "/" << AMMO_G.MAX << "]" << endl;
        cout << "[LV: " << LV_G << "]" << endl;
        cout << "[Value: " << Value_G << " Souls]\n" << endl;

    }

};

class item{
public:

    string Name;
    float Power;
    int Qnt;

    item(): Name("Empty"), Power(0), Qnt(0) {}

    item(string name, float pow, int qnt):
    Name(name), Power(pow), Qnt(qnt) {}

};

class itens{
public:

    int Souls;
    int Potion;
    int Bones;
    int Metal;
    int Skull;

    itens(): Souls(0), Potion(0), Bones(0),
    Metal(0), Skull(0) {}

};

class stash{
public:

    string Name_St;
    array<array<weapon, 12>, 4> Weapon_St;
    array<array<armor, 12>, 4> Armor_St;
    array<array<gloves, 12>, 4> Gloves_St;
    array<array<necklace, 12>, 4> Neckle_St;

    stash(): Name_St("New Stash"), Weapon_St{}, Armor_St{},
    Gloves_St{}, Neckle_St{} {}

    stash(string nam):
    Name_St(nam), Weapon_St{}, Armor_St{},
    Gloves_St{}, Neckle_St{} {}

};

class equipment{
public:

    weapon Weapon;
    gun Gun;
    armor Armor;
    necklace Neckle;
    gloves Gloves;
    array<ability, 4> Abilities;

    array<weapon, 8> WeaponInv;
    array<gun, 8> GunInv;
    array<armor, 8> ArmorInv;
    array<necklace, 8> NeckInv;
    array<gloves, 8> GloveInv;
    array<ability, 8> AbilityInv;

    equipment(): Weapon{}, Gun{}, Armor{}, Neckle{}, Gloves{},
    Abilities{}, WeaponInv{}, GunInv{}, ArmorInv{}, NeckInv{},
    GloveInv{}, AbilityInv{} {}

};

class world_char{
public:

    string Map;
    string Place;
    int Hole;
    int Room;
    int x;
    int y;

    world_char(): Map("The World"), Place("Home"),
    Hole(1), Room(0), x(0), y(0) {}
};

class atributes{
public:

    int RES;
    int STR;
    int DEX;
    int WIS;
    int AGL;
    int POINTS;

    atributes(): RES(0), STR(0), DEX(0),
    WIS(0), AGL(0) {}

};

class stats{
public:

    int HP;
    range<int> ATK;
    range<int> MAG;
    range<int> CURSE_DMG;
    int CURSE_CHANCE;
    range<int> DEF;
    float CRIT;
    int LUCK;
    range<int> PREC;
    range<int> DODGE;
    int AP;
    int MP;

    stats(): HP(0), ATK{}, MAG{}, CURSE_DMG{}, CURSE_CHANCE(0),
    DEF{}, CRIT(0), LUCK(0), PREC{}, DODGE{}, AP(0), MP(0) {}

    stats(int hp, range<int> atk, range<int> mag, range<int> dmg, int chan,
    range<int> def, float crit, int luck, range<int> prec, range<int> dodge, int ap, int mp):
    HP(hp), ATK(atk), MAG(mag), CURSE_DMG(dmg), CURSE_CHANCE(chan),
    DEF(def), CRIT(crit), LUCK(luck), PREC(prec), DODGE(dodge), AP(ap), MP(mp) {}

};

class soul{
public:

    string S_Soul;
    string S_Race;
    int S_HP;
    range<int> S_ATK;
    range<int> S_MAG;
    range<int> S_CURSE_DMG;
    int S_CURSE_CHANCE;
    range<int> S_DEF;
    float S_CRIT;
    int S_LUCK;
    range<int> S_PREC;
    range<int> S_DODGE;
    int S_AP;
    int S_MP;
    int S_LV;
    range<int> S_EXP;

    soul(): S_Soul("Empty"), S_Race("None"), S_HP(0), S_ATK{}, S_MAG{}, S_CURSE_DMG{}, S_CURSE_CHANCE(0),
    S_DEF{}, S_CRIT(0), S_LUCK(0), S_PREC{}, S_DODGE{}, S_AP(0), S_MP(0), S_LV(0), S_EXP{} {}

    soul(string soul, string race, int hp, range<int> atk, range<int> mag, range<int> dmg,
    int chan, range<int> def, float crit, int luck, range<int> prec, range<int> dodge, int ap, int mp, int lv, range<int> xp):
    S_Soul(soul), S_Race(race), S_HP(hp), S_ATK(atk), S_MAG(mag), S_CURSE_DMG(dmg), S_CURSE_CHANCE(chan),
    S_DEF(def), S_CRIT(crit), S_LUCK(luck), S_PREC(prec), S_DODGE(dodge), S_AP(ap), S_MP(mp), S_LV(lv), S_EXP(xp) {}

    void SoulInfo(){

        cout << "\n[SOUL INFORMATION]" << endl;
        cout << "[" << S_Soul << "]" << endl;
        cout << "[Race:" << S_Race << "]" << endl;
        cout << "[HP MAX:" << S_HP << "]" << endl;
        cout << "[ATK:" << S_ATK.MAX << "/" << S_ATK.MIN << "]" << endl;
        cout << "[MAG:" << S_MAG.MAX << "/" << S_MAG.MIN << "]" << endl;
        cout << "[CURSE DMG:" << S_CURSE_DMG.MAX << "/" << S_CURSE_DMG.MIN << "]" << endl;
        cout << "[CURSE CHANCE" << Percent(S_CURSE_CHANCE) << "]" << endl;
        cout << "[DEF:" << S_DEF.MAX << "/" << S_DEF.MIN << "]" << endl;
        cout << "[CRIT:" << Percent(S_CRIT) << "]" << endl;
        cout << "[LUCK:" << S_LUCK << "]" << endl;
        cout << "[PREC:" << S_PREC.MIN << "/" << S_PREC.MAX << "]"<< endl;
        cout << "[DODGE:" << S_DODGE.MIN << "/" << S_DODGE.MAX << "]" << endl;
        cout << "[AP MAX:" << S_AP << "]" << endl;
        cout << "[MP MAX:" << S_MP << "]" << endl;
        cout << "[EXP:" << S_EXP.MIN << "/" << S_EXP.MAX << "]" << endl;
        cout << "[LV:" << S_LV << "]" << endl;
    }
};

class enemy{
public:

    string Name_EN;
    string Race_EN;
    range<int> HP_EN;
    range<int> ATK_EN;
    range<int> MAG_EN;
    range<int> DEF_EN;
    float CRIT_EN;
    int LUCK_EN;//Agility, crit chance %
    range<int> PREC_EN;//precision, chance of dealing damage %
    range<int> DODGE_EN;
    range<int> AP_EN;
    range<int> MP_EN;
    int LV_EN;

    enemy(): Name_EN("Empty"), Race_EN("None"), HP_EN{},
    ATK_EN{}, MAG_EN{}, DEF_EN{}, CRIT_EN(0.0), LUCK_EN{},
    PREC_EN{}, DODGE_EN{}, AP_EN{}, MP_EN{}, LV_EN(0) {}

    void EnemyInfo(){

        cout << "\n[ENEMY INFORMATION]" << endl;
        cout << "[" << Name_EN << "]" << endl;
        cout << "[Race:" << Race_EN << "]" << endl;
        cout << "[HP:" << HP_EN.MIN << "/" << HP_EN.MAX << "]" << endl;
        cout << "[ATK:" << ATK_EN.MIN << "/" << ATK_EN.MAX << "]" << endl;
        cout << "[MAG:" << MAG_EN.MIN << "/" << MAG_EN.MAX << "]" << endl;
        cout << "[DEF:" << DEF_EN.MIN << "/" << DEF_EN.MAX << "]" << endl;
        cout << "[CRIT:" << Percent(CRIT_EN) << "]" << endl;
        cout << "[LUCK:" << LUCK_EN << "%]" << endl;
        cout << "[PREC:" << PREC_EN.MIN << "/" << PREC_EN.MAX << "]" << endl;
        cout << "[DODGE:" << DODGE_EN.MIN << "/" << DODGE_EN.MAX << "]" << endl;
        cout << "[AP:" << AP_EN.MIN << "/" << AP_EN.MAX << "]" << endl;
        cout << "[MP:" << MP_EN.MIN << "/" << MP_EN.MAX << "]" << endl;
        cout << "[LV:" << LV_EN << "]\n" << endl;

    }
};

class character{
public:

    string Name;
    string Race;
    range<int> HP;
    range<int> ATK;
    range<int> MAG;
    range<int> CURSE_DMG;
    int CURSE_CHANCE;
    range<int> DEF;
    float CRIT;
    int LUCK; //crit chance %
    range<int> PREC;
    range<int> DODGE;
    range<int> AP;
    range<int> MP;
    int LV;
    atributes Atributes;
    stats Stats;
    soul Soul;
    skills CharSkills;
    itens Itens;
    equipment Equip;
    world_char Places;
    vector<factions> Factions;
    vector<string> Titles;

    character(): Name("Empty"), Soul{}, Race("None"), HP{}, ATK{}, MAG{}, CURSE_DMG{},
    CURSE_CHANCE(0), DEF{}, CRIT(0), LUCK(0), PREC{}, DODGE(0), MP(0), LV(0) {}

    character(string name, soul so, string race, range<int> hp, range<int> atk, range<int> mg, range<int> dmg,
    int chance, range<int> def, float ct, int agl, range<int> pr, range<int> ap, range<int> mp, int lv):
    Name(name), Soul(so), Race(race), HP(hp), ATK(atk), MAG(mg), CURSE_DMG(dmg), CURSE_CHANCE(chance),
    DEF(def), CRIT(ct), LUCK(agl), PREC(pr), AP(ap), MP(mp), LV(lv) {}


    void CharInfo(){

        cout << "\n[CHARACTER INFORMATION]" << endl;
        cout << "[" << Name << "]" << endl;
        cout << "[Soul:" << Soul.S_Soul << "]" << endl;
        cout << "[Race:" << Race << "]" << endl;
        cout << "[HP:" << HP.MIN << "/" << HP.MAX << "]" << endl;
        cout << "[ATK:" << ATK.MIN << "/" << ATK.MAX << "]" << endl;
        cout << "[MAG:" << MAG.MIN << "/" << MAG.MAX << "]" << endl;
        cout << "[CURSE DMG:" << CURSE_DMG.MIN << "/" << CURSE_DMG.MAX << "]" << endl;
        cout << "[CURSE CHANCE:" << CURSE_CHANCE << "%]" << endl;
        cout << "[DEF:" << DEF.MIN << "/" << DEF.MAX << "]" << endl;
        cout << "[CRIT:" << Percent(CRIT) << "]" << endl;
        cout << "[LUCK:" << LUCK << "%]" << endl;
        cout << "[PREC:" << PREC.MIN << "/" << PREC.MAX << "]" << endl;
        cout << "[DODGE:" << DODGE.MIN << "/" << DODGE.MAX << "]" << endl;
        cout << "[AP:" << AP.MIN << "/" << AP.MAX << "]" << endl;
        cout << "[MP:" << MP.MIN << "/" << MP.MAX << "]" << endl;
        cout << "[LV:" << LV << "]\n" << endl;
        cout << "[EXP:" << Soul.S_EXP.MIN << "]" << endl;
        cout << "[NEXT LV:" << Soul.S_EXP.MAX << "]" << endl;
        cout << "[POINTS:" << Atributes.POINTS << "]" << endl;
        cout << "[BONES:" << Itens.Bones << "]" << endl;
              equipment& eq = Equip;
              weapon& weap = eq.Weapon;
              armor& arm = eq.Armor;
              necklace& neck = eq.Neckle;
              gloves& glo = eq.Gloves;

        cout << "[WEAPON:" << weap.Name_W << "]" << endl;
        cout << "[ARMOR:" << arm.Name_A << "]" << endl;
        cout << "[NECKLE:" << neck.Name_N << "]" << endl;
        cout << "[GLOVES:" << glo.Name_GL << "]" << endl;


    }

    void CreateWeapon(weapon& RandWeapon);
    void CreateArmor(armor& RandArmor);
    void CreateGloves(gloves& RandGloves);
    void CreateNeckle(necklace& RandNeckle);
    void CreateGun(gun& Firearm);
    void CreateSoul(soul& CharSoul);
    void CreateAbility(ability& RandAbility, int type);
    void EquipFound();
    void EquipManager();
    void SwitchEquip();
    void RestoreStats();
    void UpdateCharStatus();
    void UpdateAtributes();
    void WeaponInvDisplay();
    void ArmorInvDisplay();
    void GlovesInvDisplay();
    void NeckleInvDisplay();
    void StartCharacter();
    void CharacterMenu();
    void TheWorld();
    void Merchant();
    void FoundEnemy();
    void DeadReset();
    bool Battle(enemy& E);
    void ShowAbility(int abi);
    void AbilityManeger();
    void PointsDist();
    void LevelUp();
    int CurseDamage(enemy& Enemy);

};

class server{
public:

    string Server;
    stash Storage;
    vector<character> Players;

};

    array<body, 6> SetBody(int hp) {
        int head = ceil(hp * 0.74);
        int torso = ceil(hp * 0.86);
        int member = ceil(hp * 0.56);
        return {
        body("Head", {head, head}, false),
        body("Torso", {torso, torso}, false),
        body("Left Arm", {member, member}, false),
        body("Right Arm", {member, member}, false),
        body("Left Leg", {member, member}, false),
        body("Right Leg", {member, member}, false)
        };
    }

    void character::UpdateCharStatus(){

        equipment& EQ = Equip;
        weapon& W = EQ.Weapon;
        armor& A = EQ.Armor;
        gloves& GL = EQ.Gloves;
        necklace& N = EQ.Neckle;
        soul& S = Soul;
        stats& St = Stats;

        Race = S.S_Race;
        HP.MAX = S.S_HP + A.HP_A + N.HP_N + St.HP;
        ATK = SumRanges(S.S_ATK + W.ATK_W + A.ATK_A + GL.ATK_GL + N.ATK_N + St.ATK);
        MAG = SumRanges(S.S_MAG + W.MAG_W + A.MAG_A + GL.MAG_GL + N.MAG_N + St.MAG);
        CURSE_DMG = SumRanges(S.S_CURSE_DMG + W.CURSE_DMG_W + St.CURSE_DMG);
        CURSE_CHANCE = S.S_CURSE_CHANCE + St.CURSE_CHANCE;
        DEF = SumRanges(S.S_DEF + A.DEF_A + GL.DEF_GL + St.DEF);
        CRIT = S.S_CRIT + W.CRIT_W + GL.CRIT_GL + St.CRIT;
        LUCK = S.S_LUCK + W.LUCK_W + N.LUCK_N + St.LUCK;
        PREC = SumRanges(S.S_PREC + W.PREC_W + GL.PREC_GL + St.PREC);
        DODGE = SumRanges(S.S_DODGE + A.DODGE_A + GL.DODGE_GL + St.DODGE) ;
        AP.MAX = S.S_AP + A.AP_A + St.AP;
        MP.MAX = S.S_MP + A.MP_A + St.MP;
        //ifs to counter hp/ap/mp overstats
        if(HP.MIN > HP.MAX){
            HP.MIN = HP.MAX;
        }

        if(AP.MIN > AP.MAX){
            AP.MIN = AP.MAX;
        }

        if(MP.MIN > MP.MAX){
            MP.MIN = MP.MAX;
        }
    }

    void character::RestoreStats(){
        HP.MIN = HP.MAX;
        AP.MIN = AP.MAX;
        MP.MIN = MP.MAX;
        cout << "\n[SUCSSEFULLY RECOVERED]\n" << endl;
    }

    void character::UpdateAtributes(){
    atributes& At = Atributes;
    stats& St = Stats;

        St.HP = At.RES * 10;
        St.ATK = RangeNum<int>(At.STR, 5);
        St.MAG = RangeNum<int>(At.WIS, 5);
        St.CURSE_DMG = RangeNum<int>(At.WIS, 2);
        St.CURSE_CHANCE = At.DEX * 2;
        St.DEF = RangeNum<int>(At.RES, 5);
        St.CRIT = At.DEX * 0.02;
        St.LUCK = At.DEX;
        St.PREC = RangeNum<int>(At.AGL, 5);
        St.DODGE = RangeNum<int>(At.AGL, 5);
        St.AP = At.STR * 10;
        St.MP = At.WIS * 10;

    }

    void character::LevelUp(){
    soul& S = Soul;
        while(S.S_EXP.MIN >= S.S_EXP.MAX){
            Atributes.POINTS += 5;
            float next = S.S_EXP.MAX * 1.30;
            S.S_EXP.MAX = static_cast<int>(ceil(next));
            ++LV;
            cout << "[LEVEL UP! ->" << LV << "]" << endl;
        }
    }

    void character::PointsDist(){
    atributes& At = Atributes;
        do{
        cout << "[POINTS DISTRIBUTION]" << endl;
        cout << "[POINTS:" << At.POINTS << "]" << endl;
        cout << "[1].[DISTRIBUTE]" << endl;
        cout << "[2].[LEAVE]" << endl;
        int dist = VerifyCin(1, 2);

        if(dist == 1){
            do{
            cout << "\n[POINTS:" << At.POINTS << "]" << endl;
            cout << "[1].[RES][" << At.RES << "]" << endl;
            cout << "[2].[STR][" << At.STR << "]" << endl;
            cout << "[3].[WIS][" << At.WIS << "]" << endl;
            cout << "[4].[DEX][" << At.DEX << "]" << endl;
            cout << "[5].[AGL][" << At.AGL << "]" << endl;
            cout << "[6].[LEAVE]" << endl;
            int pt = VerifyCin(1, 6);

            if(pt == 6){
                break;
            }

            if(At.POINTS > 0){
                switch(pt){
                    case 1:
                        ++At.RES;
                        --At.POINTS;
                        cout << "[RES UP!]" << endl;
                        break;
                    case 2:
                        ++At.STR;
                        --At.POINTS;
                        cout << "[STR UP!]" << endl;
                        break;
                    case 3:
                        ++At.WIS;
                        --At.POINTS;
                        cout << "[WIS UP!]" << endl;
                        break;
                    case 4:
                        ++At.DEX;
                        --At.POINTS;
                        cout << "[DEX UP!]" << endl;
                        break;
                    case 5:
                        ++At.AGL;
                        --At.POINTS;
                        cout << "[AGL UP!]" << endl;
                        break;
                }
            }
            else{
                cout << "\n[NOT ENOUGHT POINTS]\n" << endl;
                break;
            }

            }while(true);
        }
        else{
            enter("\n[LEAVING...]\n", 80);
            break;
        }

        }while(true);
        UpdateAtributes();
    }

    void character::WeaponInvDisplay(){
    int page, equip, repeat;

    do{
        cout << "\n[WEAPON INVENTORY]" << endl;
        for(int i = 0; i < 8; ++i){
        weapon& We = Equip.WeaponInv[i];
            if(We.Name_W != "Empty"){
                cout << "[" << i+1 << "][" << We.Name_W
                     << "][LV:" << We.LV_W << "]" << endl;
            }
            else{
                    cout << "[" << i+1 << "][EMPTY]" << endl;
            }
        }

        cout << "\n[1-8][SELECT WEAPON]" << endl;
        equip = VerifyCin(1, 8)-1;
        Equip.WeaponInv[equip].WeaponInfo();

        cout << "[SHOW ANOTHER WEAPON?]" << endl;
        cout << "1.[YES]" << endl;
        cout << "2.[NO]" << endl;
        repeat = VerifyCin(1, 2);

        if(repeat == 1){
            enter("\n[DISPLAING ANOTHER WEAPON...]", 80);
            continue;
        }

        }while(repeat != 2);

        return;
    }

    void character::ArmorInvDisplay(){
    int page, equip, repeat;
    do{
        cout << "\n[ARMOR INVENTORY]" << endl;
        for(int i = 0; i < 8; ++i){
        armor& Ar = Equip.ArmorInv[i];
            if(Ar.Name_A != "Empty"){
                cout << "[" << i+1 << "][" << Ar.Name_A
                     << "][LV:" << Ar.LV_A << "]" << endl;
            }
            else{
                    cout << "[" << i+1 << "][EMPTY]" << endl;
            }
        }

        cout << "\n[1-8][SELECT ARMOR]" << endl;
        equip = VerifyCin(1, 8)-1;
        Equip.ArmorInv[equip].ArmorInfo();

        cout << "[SHOW ANOTHER ARMOR?]" << endl;
        cout << "[1].[YES]" << endl;
        cout << "[2].[NO]" << endl;
        repeat = VerifyCin(1, 2);

        if(repeat == 1){
            enter("\n[DISPLAING ANOTHER ARMOR...]", 80);
            continue;
        }

        }while(repeat != 2);

        return;
    }

    void character::GlovesInvDisplay(){
    int page, equip, repeat;
    do{

        cout << "\n[GLOVES INVENTORY]" << endl;
        for(int i = 0; i < 8; ++i){
        gloves& Gl = Equip.GloveInv[i];
            if(Gl.Name_GL != "Empty"){
                cout << "[" << i+1 << "][" << Gl.Name_GL
                     << "][LV:" << Gl.LV_GL << "]" << endl;
            }
            else{
                    cout << "[" << i+1 << "][EMPTY]" << endl;
            }
        }

        cout << "\n[1-8][SELECT GLOVES]" << endl;
        equip = VerifyCin(1, 8)-1;
        Equip.GloveInv[equip].GlovesInfo();

        cout << "[SHOW ANOTHER GLOVES?]" << endl;
        cout << "[1].[YES]" << endl;
        cout << "[2].[NO]" << endl;
        repeat = VerifyCin(1, 2);

        if(repeat == 1){
            enter("\n[DISPLAING ANOTHER GLOVE...]", 80);
            continue;
        }

        }while(repeat != 2);

        return;
    }

    void character::NeckleInvDisplay(){
    int page, equip, repeat;
    do{

        cout << "\n[NECLKE INVENTORY]" << endl;
        for(int i = 0; i < 8; ++i){
        necklace& Ne = Equip.NeckInv[i];
            if(Ne.Name_N != "Empty"){
                cout << "[" << i+1 << "][" << Ne.Name_N
                     << "][LV:" << Ne.LV_N << "]" << endl;
            }
            else{
                    cout << "[" << i+1 << "][EMPTY]" << endl;
            }
        }

        cout << "\n[1-8][SELECT NECKLE]" << endl;
        equip = VerifyCin(1, 8)-1;
        Equip.NeckInv[equip].NeckleInfo();

        cout << "[SEE ANOTHER NECKLE?]" << endl;
        cout << "[1].[YES]" << endl;
        cout << "[2].[NO]" << endl;
        repeat = VerifyCin(1, 2);

        if(repeat == 1){
            enter("\n[DISPLAING ANOTHER NECKLE...]", 80);
            continue;
        }

        }while(repeat != 2);

        return;
    }

    void character::SwitchEquip(){
    int slot, type, inv;

        cout << "[SELECT EQUIPMENT]" << endl;
        type = VerifyCinEquip();

        if(type == 5){
            enter("[LEAVING EQUIPMENT CHANGE...]", 60);
            return;
        }

        switch(type){
            case 1:
                WeaponInvDisplay();
                break;
            case 2:
                ArmorInvDisplay();
                break;
            case 3:
                GlovesInvDisplay();
                break;
            case 4:
                NeckleInvDisplay();
                break;
        }

        cout << "\n[SELECT SLOT TO SWAP][1-8]" << endl;
        slot = VerifyCin(1, 8) - 1;

        switch(type){
            case 1:
                swap(Equip.Weapon, Equip.WeaponInv[slot]);
                enter("[" + Equip.Weapon.Name_W + "][EQUIPED]", 80);
                break;
            case 2:
                swap(Equip.Armor, Equip.ArmorInv[slot]);
                enter("[" + Equip.Armor.Name_A + "][EQUIPED]", 80);
                break;
            case 3:
                swap(Equip.Gloves, Equip.GloveInv[slot]);
                enter("[" + Equip.Gloves.Name_GL + "][EQUIPED]", 80);
                break;
            case 4:
                swap(Equip.Neckle, Equip.NeckInv[slot]);
                enter("[" + Equip.Neckle.Name_N + "][EQUIPED]", 80);
                break;

        }
        UpdateCharStatus();
    }


    void character::EquipManager(){
    equipment& E = Equip;
    weapon& w = E.Weapon;
    armor& a = E.Armor;
    gloves& g = E.Gloves;
    necklace& n = E.Neckle;
    do{
        cout << "\n[EQUIPMENT MANEGER]" << endl;
        cout << "[1].[SHOW INVENTORY]" << endl;
        cout << "[2].[SHOW EQUIPED ITENS]" << endl;
        cout << "[3].[CHANGE EQUIPMENT]" << endl;
        cout << "[4].[LEAVE MANAGER]" << endl;
        int equip = VerifyCin(1, 4);

        if(equip == 1){
            cout << "\n[SHOW INVENTORY]" << endl;
            int inv1 = VerifyCin(1, 4);

            switch(inv1){
                case 1:
                    WeaponInvDisplay();
                    break;
                case 2:
                    ArmorInvDisplay();
                    break;
                case 3:
                    GlovesInvDisplay();
                    break;
                case 4:
                    NeckleInvDisplay();
                    break;
                }
        }
        else if(equip == 2){
            cout << "\n[SHOW EQUIPED ITENS]" << endl;
            cout << "1.EQUIPED WEAPON:" << endl;
            cout << "[" << w.Name_W << "][" << w.LV_W << "]" << endl;
            cout << "2.EQUIPED ARMOR:" << endl;
            cout << "[" << a.Name_A << "][" << a.LV_A << "]" << endl;
            cout << "3.EQUIPED GLOVES:" << endl;
            cout << "[" << g.Name_GL << "][" << g.LV_GL << "]" << endl;
            cout << "4.EQUIPED NECKLACE:" << endl;
            cout << "[" << n.Name_N << "][" << n.LV_N << "]" << endl;
            cout << "5.GO BACK" << endl;
            int inv2 = VerifyCin(1, 5);

            switch(inv2){
                case 1:
                    enter("[CURRENT WEAPON]", 80);
                    w.WeaponInfo();
                    break;
                case 2:
                    enter("[CURRENT ARMOR]", 80);
                    a.ArmorInfo();
                    break;
                case 3:
                    enter("[CURRENT GLOVES]", 80);
                    g.GlovesInfo();
                    break;
                case 4:
                    enter("[CURRENT NECKLE]", 80);
                    n.NeckleInfo();
                    break;
                default:
                     enter("[LEAVING EQUIPMENT]", 80);
            }
        }
        else if(equip == 3){
            SwitchEquip();
            continue;
        }
        else{
            enter("\n[LEAVING MANAGER]\n", 80);
            break;
        }

        }while(true);
    return;
    }

    void character::CreateSoul(soul& CharSoul){

        struct SoulDet{

            string soul;
            int HP;
            int ATK;
            int MAG;
            int DMG;
            int CHA;
            int DEF;
            float CRIT;
            int LUCK;
            int PREC;
            int DODGE;
            int AP;
            int MP;

        };

        vector<vector<SoulDet>> PREFIX{
        {{"Meridian", 150, 30, 10, 5, 40, 10, 1.20, 30, 60, 60, 200, 120},
        {"Dark", 140, 35, 15, 10, 55, 5, 1.25, 35, 60, 50, 190, 100}},
        {{"Okasian", 160, 25, 25, 10, 50, 7, 1.15, 20, 60, 55, 180, 180},
        {"Mystic", 140, 10, 30, 20, 60, 5, 1.12, 35, 60, 60, 120, 200}},
        {{"Sinner", 150, 25, 10, 30, 45, 6, 1.15, 35, 65, 55, 180, 140},
        {"Saint", 140, 10, 25, 20, 60, 5, 1.10, 20, 70, 50, 100, 220}},
        {{"Feral", 160, 30, 5, 15, 65, 10, 1.08, 30, 60, 60, 220, 80},
        {"Magical", 130, 5, 30, 10, 70, 5, 1.15, 35, 70, 55, 80, 220}},
        {{"Faithless", 160, 35, 15, 5, 45, 10, 1.05, 25, 70, 55, 180, 120},
        {"Insane", 140, 15, 30, 15, 65, 5, 1.15, 35, 65, 55, 110, 200}}
        };
        //(soul, hp, atk, mag, dmg, chance, def, crit, luck, prec, dodge, ap, mp)

        vector<string> Race{"Angel", "Demon", "Human", "Creature", "Unknown"};
        vector<string> SoulRace{"Angelical", "Demoniac", "Artificial", "Feral", "Mysterious"};
        //(angel, demon, human, creature, unknown)
        vector<vector<SoulDet>> SUFIX{
        {{"of Seas", 100, 30, 10, 5, 20, 10, 0.20, 10, 30, 40, 100, 20},
        {"of Avalon", 90, 35, 15, 10, 25, 5, 0.25, 15, 30, 30, 90, 0}},
        {{"of Hell", 110, 25, 25, 10, 20, 7, 0.15, 5, 30, 35, 80, 80},
        {"of Hades", 90, 10, 30, 20, 20, 5, 0.12, 15, 40, 40, 20, 100}},
        {{"of Blaphemy", 100, 25, 10, 15, 25, 6, 0.15, 15, 35, 35, 80, 40},
        {"of Witches", 90, 10, 25, 20, 20, 5, 0.10, 5, 40, 30, 0, 120}},
        {{"of Wrath", 120, 30, 5, 15, 25, 10, 0.08, 10, 40, 40, 120, -20},
        {"of Nether", 80, 5, 30, 10, 25, 5, 0.15, 15, 40, 35, -20, 20}},
        {{"of Decay", 110, 35, 15, 5, 15, 5, 0.05, 5, 40, 35, 80, 20},
        {"of Wichery", 90, 15, 30, 15, 25, 5, 0.15, 15, 35, 35, 10, 100}}
        };
        //(soul, hp, atk, mag, dmg, chance, def, crit, luck, prec, dodge, ap, mp)

        int LVX = LvDet(LV);

        int RaceIndex = DistReal<int>(0, Race.size()-1);
        int PrefixIndex = DistReal<int>(0, PREFIX[RaceIndex].size()-1);
        int SufixIndex = DistReal<int>(0, SUFIX[RaceIndex].size()-1);
        string RaceX = Race[RaceIndex];
        string RaceName = SoulRace[RaceIndex];

        soul& S = CharSoul;
        SoulDet& Pr = PREFIX[RaceIndex][PrefixIndex];
        SoulDet& Su = SUFIX[RaceIndex][SufixIndex];

        string Detsoul = Pr.soul + " " + RaceName + " Soul " + Su.soul;
        int DetHP = Pr.HP + Su.HP;
        int DetATK = Pr.ATK + Su.ATK;
        int DetMAG = Pr.MAG + Su.MAG;
        int DetDMG = Pr.DMG + Su.DMG;
        int DetCHA = Pr.CHA + Su.CHA;
        int DetDEF = Pr.DEF + Su.DEF;
        float DetCRIT = Pr.CRIT + Su.CRIT;
        int DetLUCK = Pr.LUCK + Su.LUCK;
        int DetPREC = Pr.PREC + Su.PREC;
        int DetDODGE = Pr.DODGE + Su.DODGE;
        int DetAP = Pr.AP + Su.AP;
        int DetMP = Pr.MP + Su.MP;

        S.S_Soul = Detsoul;
        S.S_Race = RaceX;
        S.S_HP = SingleStats<int>(DetHP, 1, LVX);
        S.S_ATK = RangeStats<int>(DetATK, 1, LVX);
        S.S_MAG = RangeStats<int>(DetMAG, 1, LVX);
        S.S_CURSE_DMG = RangeStats<int>(DetDMG, 1, LVX);
        S.S_CURSE_CHANCE = SingleStats<int>(DetCHA, 1, LVX);
        S.S_DEF = RangeStats<int>(DetDEF, 1, LVX);
        S.S_CRIT = SingleStats<float>(DetCRIT, 1, LVX);
        S.S_LUCK = SingleStats<int>(DetLUCK, 1, LVX);
        S.S_PREC = RangeStats<int>(DetPREC, 1, LVX);
        S.S_DODGE = RangeStats<int>(DetDODGE, 1, LVX);
        S.S_AP = SingleStats<int>(DetAP, 1, LVX);
        S.S_MP = SingleStats<int>(DetMP, 1, LVX);
        S.S_LV = LVX;
        S.S_EXP.MIN = 0;
        S.S_EXP.MAX = 120 * (LVX * 1.30);
    }

    void character::CreateGun(gun& Firearm){

        struct gunset{

            string name;
            int burst;
            int ench;
            int dmg;
            float leth;
            int hand;
            int aim;

        };
        //(name, burst, ench, dmg, leth, hand, aim);

        struct calibers{

            string caliber;
            int dmg;
            int ammo;

        };

        vector<pair<string, int>> GunsVec{
        {"Pistol", 22},
        {"SMG", 18},
        {"Sniper", 5},
        {"Assault", 12},
        {"SemiAuto", 16},
        {"Shotgun", 14}
        };

        vector<vector<calibers>> Calibers{
        {{".40", 10, 15},//pistols
        {"9mm", 13, 13},
        {".45", 20, 8}},
        {{".32", 7, 30},//smgs
        {"9mm", 10, 30},
        {"5.7mm", 10, 60}},
        {{".50", 35, 5},//snipers
        {".388", 30, 5},
        {".223", 25, 10}},
        {{"5.56mm", 12, 30},//assault
        {"7.62mm", 17, 30},
        {".308", 23, 20}},
        {{".223", 15, 30},//semiautos
        {"7.62mm", 17, 30},
        {".308", 23, 20}},
        {{"12GA", 30, 8},//shotguns
        {"10GA", 40, 4},
        {"6GA", 50, 2}}
        };

        vector<vector<gunset>> Brand{
        {{"Negal", 1, 5, 25, 1.05, 45, 55},//pistols
        {"Algox", 1, 5, 30, 1.03, 30, 55},
        {"Techstein", 1, 10, 15, 1.04, 40, 50}},
        {{"Sumaru", 3, 3, 5, 1.04, 40, 35},//smgs
        {"Oroch", 3, 5, 4, 1.03, 30, 45},
        {"Ziang", 4, 1, 7, 1.03, 30, 35}},
        {{"Algox", 1, 0, 60, 1.05, 50, 70},//snipers
        {"Marter", 1, 10, 45, 1.07, 40, 70},
        {"Mitsui", 2, 5, 25, 1.08, 45, 65}},
        {{"Akshinov", 3, 0, 10, 1.04, 45, 50},//assault
        {"ARB", 4, 5, 5, 1.02, 25, 55},
        {"Vortex", 2, 5, 20, 1.03, 30, 50}},
        {{"AR-KS", 1, 0, 45, 1.05, 40, 60},//semiautos
        {"AR-EK", 1, 10, 30, 1.05, 35, 65},
        {"SERV", 1, 20, 20, 1.04, 40, 65}},
        {{"GALAD", 1, 15, 40, 1.03, 60, 25},//shotguns
        {"Akshinov", 1, 0, 65, 1.03, 65, 30},
        {"USAG", 3, 0, 25, 1.02, 70, 30}}
        };
         //(name, burst, ench, dmg, leth, hand, aim);

        vector<vector<gunset>> GunParts{
        {{"-AS", 0, 5, 5, 0.05, 20, 15},//pistols
        {"-ASG", 1, 0, 5, 0.04, 15, 20},
        {"-BLAKK", 2, 0, 0, 0.04, 10, 10}},
        {{"-STEIN", 0, 5, 10, 0.04, 15, 5},//smgs
        {"-RE", 1, 0, 10, 0.02, 5, 0},
        {"-REV", 3, 0, -5, 0.00, 10, -5}},
        {{"-BRKS", 0, 5, 10, 0.02, 0, 5},//snipers
        {"-REX", 1, 10, 10, -0.01, -5, 15},
        {"-VS", 0, 10, 15, 0.02, 10, 5}},
        {{"-IN", 0, 5, 10, 0.01, 5, 5},//assault
        {"-AC", 0, 10, 5, 0.01, 0, 15},
        {"-JKT", 1, 5, 15, -0.01, -5, 0}},
        {{"-SE7", 0, 5, 10, 0.04, 25, 10},//semiautos
        {"-REX", 0, 10, 0, 0.05, 20, 10},
        {"-JNG",0 , 0, 20, 0.02, 5, 5}},
        {{"-AUTO", 2, 0, 0, 0.03, 5, 5},//shotguns
        {"-RKS", 0, 5, 20, 0.03, 20, 15},
        {"-FZ", 0, 20, 10, 0.01, 15, 10}}
        };
         //(name, burst, ench, dmg, leth, hand, aim);

        vector<vector<gunset>> GunMod{
        {{"-67E", 1, 5, 5, 0.02, 5, 5},//pistols
        {"-49B", 0, 0, 15, 0.02, 5, 10},
        {"-31RK", 0, 5, 5, 0.04, 0, 0}},
        {{"-32J", 0, 0, 5, 0.03, 5, 10},//smgs
        {"-4AF", 1, 0, 0, 0.00, 5, -5},
        {"-78GX", 0, 0, 5, 0.02, 5, 5}},
        {{"-70JN", 0, 5, 10, 0.03, 5, 10},//snipers
        {"-2TWO", 1, 0, 0, -0.02, 0, -5},
        {"-3R", 0, 10, 10, 0.03, 10, 0}},
        {{"-47Au", 0, 5, 10, 0.03, 10, 5},//assault
        {"-UM7", 0, 0, 15, 0.04, 5, 5},
        {"-D34", 1, 0, 5, 0.01, 0, -5}},
        {{"-SEMI", 0, 0, 20, 0.04, 0, 10},//semiautos
        {"-78G", 0, 5, 20, 0.02, 5, 10},
        {"-21H", 0, 10, 10, 0.03, 10, 10}},
        {{"-14FF", 0, 10, 20, 0.00, 5, -5},//shotguns
        {"-3AXN", 1, 5, 10, -0.01, 0, 0},
        {"-58YF", 0, -5, 25, 0.01, 5, 5}}
        };
         //(name, burst, ench, dmg, leth, hand, aim);

        int LVX = LvDet(LV);

        auto [rarity, rarity_value] = RarityGen();
        auto [SealName, SealDmg, SealChance] = SealGen(LVX);
        auto [GunType, GunIndex] = DiscreteDist<string>(GunsVec);
        int selectPart = DistReal<int>(0, GunParts[GunIndex].size()-1);
        int selectBrand = DistReal<int>(0, Brand[GunIndex].size()-1);
        int selectMod = DistReal<int>(0, GunMod[GunIndex].size()-1);
        int selectCal = DistReal<int>(0, Calibers[GunIndex].size()-1);

        gun& FA = Firearm;
        gunset& bra = Brand[GunIndex][selectBrand];
        gunset& pts = GunParts[GunIndex][selectPart];
        gunset& mod = GunMod[GunIndex][selectMod];
        calibers& cal = Calibers[GunIndex][selectCal];

        string G_name = bra.name + pts.name + mod.name;
        string cal_g = cal.caliber;
        int ammo_g = cal.ammo;
        int G_burst = bra.burst + pts.burst + mod.burst;
        int G_ench = bra.ench + pts.ench + mod.ench;
        int G_dmg = bra.dmg + pts.dmg + mod.dmg + cal.dmg;
        float G_leth = bra.leth + pts.leth + mod.leth;
        int G_hand = bra.hand + pts.hand + mod.hand;
        int G_aim = bra.aim + pts.aim + mod.aim;
        float value = DistReal<int>(108, 144) * (1 + LVX * 0.05) * rarity_value;

        FA.Name_G = G_name;
        FA.Type_G = GunType;
        FA.Seal_G = SealName;
        FA.Caliber_G = cal_g;
        FA.Rarity_G = rarity;
        FA.BURST_G = G_burst;
        FA.DAMAGE_G = RangeStats<int>(G_dmg, rarity_value, LVX);
        FA.ENCHANT_G = RangeStats<int>(G_ench, rarity_value, LVX);
        FA.SEAL_DMG_G = SealDmg;
        FA.SEAL_CHANCE_G = SealChance;
        FA.LETHAL_G = SingleStats<float>(G_leth, rarity_value, LVX);
        FA.HANDLE_G = SingleStats<int>(G_hand, rarity_value, LVX);
        FA.AIM_G = RangeStats<int>(G_aim, rarity_value, LVX);
        FA.LV_G = LVX;
        FA.Value_G = static_cast<int>(ceil(value));
        FA.AMMO_G.MAX = ammo_g;
        FA.AMMO_G.MIN = ammo_g;
    }

    array<string, 2> SetType(int classIndex, int typeIndex) {

        vector<string> Classes = {"Melee", "Magic", "Enchanted"};

        vector<vector<string>> Types = {
        {"Blade", "Daggers", "Blunt", "Pierce", "None"},//Melee
        {"Wand", "Staff", "Idol", "Book", "None"},//Magic
        {"Magic Blade", "Magic Daggers", "Magic Blunt", "Orb", "None"}//Enchanted
        };

        if (classIndex >= 0 && classIndex < Classes.size() &&
            typeIndex >= 0 && typeIndex < Types[classIndex].size()){
            return {Classes[classIndex], Types[classIndex][typeIndex]};
        }
        else{
            return {"None", "None"};
        }
    }


    void character::CreateWeapon(weapon& RandWeapon){
    int x, y, z, LVX;

        struct stats{

            string name;
            int atk;//melee damage
            int mag;//magic damage
            float crit;//critic damage
            int luck;//critic chance
            int prec;//precision, chance of executing the ability

        };

        LVX = LvDet(LV);// randon level based on character's LV

        vector<vector<stats>> Prefix{//3 class indexs, 4 per classes
        {{"Sharp", 9, 0, 0.04, 4, 3},//melee
        {"Ocult", 10, 0, 0.03, 5, 3},
        {"Cursed", 9, 0, 0.04, 4, 3},
        {"Sinner's", 9, 0, 0.06, 3, 3}},
        {{"Infernal", 0, 10, 0.02, 4, 3},//magical
        {"Cursed", 0, 9, 0.04, 4, 3},
        {"Blazed", 0, 9, 0.03, 5, 3},
        {"Unsainted", 0, 10, 0.02, 3, 3}},
        {{"Flaming", 4, 5, 0.03, 4, 3},//enchanted
        {"Sparking", 4, 5, 0.03, 4, 3},
        {"Frozen", 5, 4, 0.04, 4, 3},
        {"Dead", 6, 3, 0.06, 3, 3}}
        };
        //(name, atk, mag, crit, luck, prec)
        vector<vector<vector<stats>>> Additional{//3 indexs per weapon, 4 weapon types per classes
        {{{"Agile", 8, 0, 0.04, 5, 6},//blade, melee begin
        {"Great", 13, 0, -0.02, 2, 3},
        {"Heavy", 10, 0, 0.02, 4, 4}},
        {{"Agile", 8, 0, 0.04, 5, 6},//dagger
        {"Bloody", 7, 0, 0.05, 5, 6},
        {"Sanguinare", 6, 0, 0.06, 6, 5}},
        {{"Heavy", 10, 0, 0.00, 4, 3},//blunt
        {"Great", 13, 0, -0.01, 2, 3},
        {"Spiked", 14, -2, -0.02, 2, 3}},
        {{"Pointed", 8, 0, 0.02, 5, 5},//perfuration
        {"Sharped", 7, 0, 0.03, 5, 5},
        {"Great", 10, 0, -0.02, 4, 4}}},//melee end
        {{{"Burning", 0, 8, 0.02, 4, 6},//wand, magic begin
        {"Deadly", 0, 7, 0.03, 5, 6},
        {"Crooked", 0, 6, 0.05, 6, 5}},
        {{"Rotten", 0, 13, 0.02, 3, 4},//staff
        {"Curved", -1, 13, 0.01, 3, 3},
        {"Satanic", -2, 14, 0.00, 2, 3}},
        {{"Wicked", 0, 7, 0.06, 5, 3},//idoll
        {"Negative", 0, 6, 0.07, 6, 3},
        {"Blasphemous", 0, 7, 0.05, 6, 3}},
        {{"Burt", 0, 7, 0.03, 5, 5},//Magic book
        {"Diabolic", 0, 9, 0.02, 5, 5},
        {"Unholy", 0, 8, 0.04, 5, 5}}},//magic end
        {{{"Angelical", 4, 5, 0.04, 2, 6},//magic blade, enchanted begin
        {"Curved", 5, 4, 0.03, 3, 6},
        {"Satanic", 5, 5, 0.02, 4, 6}},
        {{"Cursed", 4, 2, 0.05, 6, 8},//magic dagger
        {"Obscure", 4, 3, 0.06, 6, 8},
        {"Sacrificial", 5, 2, 0.05, 6, 8}},
        {{"Cracked", 9, 2, -0.01, 2, 4},//magic blunt
        {"Cruel", 8, 3, 0.00, 2, 4},
        {"Evil", 5, 6, -0.02, 1, 4}},
        {{"Burt", 2, 7, 0.04, 5, 6},//orb
        {"Biabolic", 4, 6, 0.03, 6, 6},
        {"Unholy", 5, 5, 0.02, 5, 6}}}
        };
        //(name, atk, mag, crit, luck, prec)
        vector<vector<vector<stats>>> Weapon{//3 indexs per weapon, 4 weapon types per classes
        {{{"Sword", 4, 0, 0.03, 3, 2},//blade, melee begin
        {"Schimitar", 4, 0, 0.02, 2, 3},
        {"Axe", 5, 0, 0.02, 3, 2}},
        {{"Daggers", 3, 0, 0.05, 3, 4},//dagger
        {"Knives", 4, 0, 0.03, 4, 4},
        {"Kunais", 4, 0, 0.04, 4, 4}},
        {{"Hammer", 6, -2, 0.02, 2, 3},//blunt
        {"Mace", 6, 0, 0.02, 2, 2},
        {"Zheihander", 7, -3, -0.01, 2, 2}},
        {{"Spear", 5, 0, 0.02, 3, 3},//perfuration
        {"Lance", 4, 0, 0.03, 3, 3},
        {"Long Sword", 6, -1, 0.02, 3, 3}}},//melee end
        {{{"Wand", 0, 4, 0.04, 3, 3},//wand, magic begin
        {"Bone", 0, 5, 0.03, 2, 3},
        {"Finger", 0, 6, 0.02, 2, 3}},
        {{"Staff", 0, 6, 0.01, 2, 2},//staff
        {"Guitar", 0, 7, 0.02, 3, 2},
        {"Pole", 0, 6, 0.01, 2, 2}},
        {{"Idoll", 0, 4, 0.04, 3, 5},//idoll
        {"Mandragora", 0, 3, 0.05, 2, 5},
        {"Doll", 0, 5, 0.03, 4, 5}},
        {{"Bible", 0, 6, -0.01, 5, 4},//Magic book
        {"Torav", 0, 7, -0.02, 5, 4},
        {"Alcooram", 0, 7, -0.02, 5, 4}}},//magic end
        {{{"Excalibur", 3, 4, 0.03, 4, 4},//magic blade, enchanted begin
        {"Sawazaki", 4, 3, 0.02, 4, 4},
        {"Hirudo", 6, 2, 0.02, 3, 4}},
        {{"Kamas", 4, 3, 0.04, 6, 5},//magic dagger
        {"Kunais", 3, 4, 0.05, 6, 6},
        {"Kaichis", 3, 5, 0.06, 6, 5}},
        {{"Thor Hammer", 6, 3, -0.01, 2, 2},//magic blunt
        {"Morning Star", 7, 2, 0.00, 3, 2},
        {"Elven Smasher", 5, 4, -0.02, 2, 3}},
        {{"Orb", 4, 6, 0.03, 3, 5},//orb
        {"Sphere", 3, 7, 0.03, 3, 6},
        {"Globe", 5, 5, 0.03, 3, 4}}}
        };
        //(name, atk, mag, crit, luck, prec)
        vector<vector<stats>> Sufix{
        {{"from North", 3, 0, 0.03, 3, 4},//melee
        {"of Depression", 4, 0, 0.02, 4, 3},
        {"of Pychos", 5, 0, 0.01, 3, 4},
        {"of Canibals", 2, 0, 0.04, 2, 4}},
        {{"from Hell", 0, 2, 0.04, 2, 3},//magical
        {"from the Valley", 0, 3, 0.02, 3, 4},
        {"of Sadists", 0, 4, 0.02, 3, 4},
        {"of Satan", 0, 5, 0.01, 3, 4}},
        {{"of Samael", 2, 3, 0.02, 3, 4},//enchanted
        {"of Lilith", 1, 3, 0.03, 4, 4},
        {"of Uriel", 3, 2, 0.01, 4, 4},
        {"of Baal", 2, 1, 0.04, 4, 3}}
        };
        //(name, atk, mag, crit, luck, prec)
        int WeaponClass = DistReal<int>(0, 2);
        int WeaponType = DistReal<int>(0, 3);

        int PrefixDist = DistReal<int>(0, Prefix[WeaponClass].size()-1);//prefix index
        int WeaponDist = DistReal<int>(0, Weapon[WeaponClass][WeaponType].size()-1);//weapon index
        int AddDist = DistReal<int>(0, Additional[WeaponClass][WeaponType].size()-1);//add index
        int SufixDist = DistReal<int>(0, Sufix[WeaponClass].size()-1);//sufix index

        weapon& W = RandWeapon;
        stats& pr = Prefix[WeaponClass][PrefixDist];
        stats& ad = Additional[WeaponClass][WeaponType][AddDist];
        stats& we = Weapon[WeaponClass][WeaponType][WeaponDist];
        stats& su = Sufix[WeaponClass][SufixDist];

        auto [curse, curse_value] = CurseGen(LVX);
        auto [rarity, rarity_value] = RarityGen();
        string WeaponName = pr.name + " " + ad.name + " " + we.name + " " + su.name;

        int atkWeapon = pr.atk + ad.atk + we.atk + su.atk;
        int magWeapon = pr.mag + ad.mag + we.mag + su.mag;
        float critWeapon = pr.crit + ad.crit + we.crit + su.crit;
        int luckWeapon = pr.luck + ad.luck + we.luck + su.luck;
        int precWeapon = pr.prec + ad.prec + we.prec + su.prec;
        float valueWeapon = DistReal<int>(108, 144) * (1 + (LVX * 0.2)) * rarity_value;

        W.Name_W = WeaponName;
        W.Type_W = SetType(WeaponClass, WeaponType);
        W.Curse_W = curse;
        W.Rarity_W = rarity;
        W.ATK_W = RangeStats<int>(atkWeapon, rarity_value, LVX);
        W.MAG_W = RangeStats<int>(magWeapon, rarity_value, LVX);
        W.CURSE_DMG_W = RangeStats<int>(curse_value, rarity_value, LVX);
        W.CRIT_W = SingleStats<float>(critWeapon, rarity_value, LVX);
        W.LUCK_W = SingleStats<int>(luckWeapon, rarity_value, LVX);
        W.PREC_W = RangeStats<int>(precWeapon, rarity_value, LVX);
        W.LV_W = LVX;
        W.Value_W = static_cast<int>(ceil(valueWeapon));
    }

    void character::CreateArmor(armor& RandArmor){
    int LVX;


        struct stats{

            string name;
            int atk;
            int mag;
            int hp;
            int def;
            int dodge;
            int ap;
            int mp;

        };

        LVX = LvDet(LV);// randon level based on character's LV

        vector<string> Types{"Light", "Medium", "Heavy", "Robes", "Corpse"};

        vector<vector<stats>> ARMORS{
        {{"Skin Armor", 0, 0, 5, 3, 9, 5, 5},//light
        {"Fur Armor", 0, 0, 6, 3, 9, 5, 5},
        {"Silk Armor", 0, 0, 5, 3, 9, 5, 5}},
        {{"Mithril Armor", 0, 0, 8, 6, 6, 6, 3},//medium
        {"Bones Armor", 0, 0, 7, 6, 6, 6, 3},
        {"Bronze Armor", 0, 0, 7, 6, 6, 7, 3}},
        {{"Scales Armor", 0, 0, 10, 9, 3, 10, 1},// heavy
        {"Demon Armor", 0, 0, 10, 9, 3, 9, 1},
        {"Drake Armor", 0, 0, 9, 9, 3, 9, 1}},
        {{"Cotton Robes", 0, 5, 2, 2, 9, 1, 9},//robes
        {"Guts Vest", 0, 6, 2, 2, 9, 1, 9},
        {"Human Skin Robes", 0, 6, 2, 2, 9, 2, 9}},
        {{"Goat Head", 6, 0, 4, 4, 6, 9, 2},//attack
        {"Teeth Bracelets", 7, 0, 5, 3, 6, 9, 2},
        {"Ribs Crown", 7, 0, 5, 4, 7, 9, 2}}
        };
        //(name, atk, mag, hp, def, dodge, ap, mp)
        vector<vector<stats>> SUFIX{
        {{"of Wolves", 0, 0, 3, 3, 10, 9, 2},//light
        {"of Fury", 0, 0, 3, 3, 10, 9, 2},
        {"of Imps", 0, 0, 3, 4, 9, 9, 2}},
        {{"From Depths", 0, 0, 6, 6, 10, 0},//medium
        {"of Cowards", 0, 0, 6, 6, 10, 0},
        {"of Torturers", 0, 0, 7, 6, 9, 0}},
        {{"of Dragons", 0, 0, 9, 9, 12, -3},//heavy
        {"From Meridia", 0, 0, 9, 8, 11, -2},
        {"From The Hole", 0, 0, 10, 9, 12, -3}},
        {{"of Satan", 0, 9, 1, 7, 0, 9},//robes
        {"of Massacre", 0, 9, 2, 7, 0, 9},
        {"From Umbral", 0, 10, 1, 8, 0, 9}},
        {{"of Ocultism", 4, 0, 5, 4, 9, 0},//attack
        {"of Cannibalism", 3, 0, 5, 5, 9, 0},
        {"of Sodomy", 5, 0, 5, 4, 9, 0}}
        };

        int ArmorType = DistReal<int>(0, Types.size()-1);
        int ArmorSelect = DistReal<int>(0, ARMORS[ArmorType].size()-1);
        int SufixSelect = DistReal<int>(0, SUFIX[ArmorType].size()-1);
        auto [rarity, rarity_value] = RarityGen();

        armor& AR = RandArmor;
        stats& arm = ARMORS[ArmorType][ArmorSelect];
        stats& su = SUFIX[ArmorType][SufixSelect];
        string ResName = Types[ArmorType];

        string armorName = arm.name + " " + su.name;
        int armorAtk = arm.atk + su.atk;
        int armorMag = arm.mag + su.mag;
        int armorHp  = arm.hp + su.hp;
        int armorDef = arm.def + su.def;
        int armorDodge = arm.dodge + su.dodge;
        int armorAp  = arm.ap + su.ap;
        int armorMp  = arm.mp + su.mp;
        float valueAr = DistReal<int>(108, 144) * (1 + (LVX * 0.2)) * rarity_value;

        AR.Name_A = armorName;
        AR.Type_A = ResName;
        AR.Rarity_A = rarity;
        AR.HP_A = SingleStats<int>(armorHp, rarity_value, LVX);
        AR.ATK_A = RangeStats<int>(armorAtk, rarity_value, LVX);
        AR.MAG_A = RangeStats<int>(armorMag, rarity_value, LVX);
        AR.DEF_A = RangeStats<int>(armorDef, rarity_value, LVX);
        AR.DODGE_A = RangeStats<int>(armorDodge, rarity_value, LVX);
        AR.AP_A = SingleStats<int>(armorAp, rarity_value, LVX);
        AR.MP_A = SingleStats<int>(armorMp, rarity_value, LVX);
        AR.LV_A = LVX;
        AR.Value_A = static_cast<int>(ceil(valueAr));

    }

    void character::CreateGloves(gloves& RandGloves){
    int LVX;


        struct stats{

            string name;
            int atk;
            int mag;
            int def;
            float crit;
            int prec;
            int dodge;

        };

        LVX = LvDet(LV);// randon level based on character's LV
        vector<vector<stats>> PREFIX{
        {{"Ragades", 1, 3, 4, 0.04, 5, 5},//light gloves
        {"Bloody", 3, 1, 4, 0.04, 5, 5}},
        {{"Hardened", 3, 1, 6, 0.02, 4, 4},//medium gloves
        {"Bones", 1, 3, 6, 0.02, 4, 4}},
        {{"Iron", 3, 1, 8, -0.01, 3, 3},//heavy gloves
        {"Steel", 1, 3, 8, -0.01, 3, 3}},
        {{"Sharped", 4, -1, 2, 0.04, 5, 4},//atack gloves
        {"Pointed", 5, -2, 2, 0.04, 4, 5}},
        {{"Enchanted", -1, 4, 2, 0.02, 4, 5},//magic gloves
        {"Cursed", -2, 5, 2, 0.02, 5, 4}}
        };
        //{name, atk, mag, def, crit, prec, dodge}
        vector<string> TYPES{
        "Light Gloves",
        "Medium Gloves",
        "Heavy Gloves",
        "Atack Gloves",
        "Magic Gloves"
        };
        //{name, atk, mag, def, crit, prec, dodge}
        vector<vector<stats>> MATERIAL{
        {{"Cotton", 0, 0, 5, 0.04, 5, 5},//light
        {"Skin", 0, 0, 7, 0.05, 5, 5},
        {"Silk", 0, 0, 6, 0.04, 5, 5}},
        {{"Mithril", 0, 0, 8, 0.03, 3, 3},//medium
        {"Chain Mail", 0, 0, 9, 0.03, 3, 3},
        {"Bronze", 0, 0, 10, 0.02, 3, 3}},
        {{"Scales", 0, 0, 13, 0.01, 1, 1},// heavy
        {"Demon", 0, 0, 14, 0.00, 1, 1},
        {"Drake", 0, 0, 15, -0.01, 1, 1}},
        {{"Abandoned", 7, 0, 2, 0.03, 4, 4},//atack
        {"Destruction", 6, 0, 4, 0.04, 4, 4},
        {"Stiched Skin", 8, 0, 3, 0.03, 4, 4}},
        {{"Goat Head", 0, 7, 3, 0.03, 4, 4},//magic
        {"Teeth", 0, 6, 4, 0.04, 4, 4},
        {"Ribs", 0, 8, 2, 0.03, 4, 4}}
        };

        vector<stats> EQUIPMENT{
        {"Gloves", 3, 3, 3, 0.02, 7, 7},
        {"Bracelets", 4, 2, 6, 0.02, 5, 5},
        {"Mitten", 5, 1, 9, 0.02, 3, 3},
        {"Knuckes", 7, 0, 2, 0.04, 4, 4},
        {"Magic Gloves", 0, 7, 2, 0.04, 4, 4}
        };

        vector<stats> SUFIX{
        {"of Slavery", 2, 3, 5, 0.01, 5, 4},
        {"of Obscuriry", 3, 2, 5, 0.01, 4, 5},
        {"of Polution", 3, 1, 3, 0.02, 4, 4},
        {"of Pilgrims", 3, 1, 3, 0.02, 4, 5},
        {"from Depths", 1, 5, 5, 0.01, 5, 5},
        {"of Satanism", 5, 2, 7, 0.00, 4, 4},
        {"of Depravation", 2, 3, 4, 0.02, 3, 3},
        {"of Plagues", 5, 1, 5, 0.01, 4, 6}
        };
        //(name, atk, mag, def, crit, prec, dodge)
        int GloveType = DistReal<int>(0, TYPES.size()-1);
        int GloveMat = DistReal<int>(0, MATERIAL[GloveType].size()-1);
        int GlovePrefix = DistReal<int>(0, PREFIX[GloveType].size()-1);
        int GloveSufix = DistReal<int>(0, SUFIX.size()-1);
        auto [rarity, rarity_value] = RarityGen();

        gloves& G = RandGloves;
        stats& pr = PREFIX[GloveType][GlovePrefix];
        stats& eq = EQUIPMENT[GloveType];
        stats& ma = MATERIAL[GloveType][GloveMat];
        stats& su = SUFIX[GloveSufix];

        string glovesName = pr.name + " " + ma.name + " " + eq.name + " " + su.name;
        int glovesATK = pr.atk + ma.atk + eq.atk + su.atk;
        int glovesMAG = pr.mag + ma.mag + eq.mag + su.mag;
        int glovesDEF = pr.def + ma.def + eq.def + su.def;
        float glovesCRIT = pr.crit + ma.crit + eq.crit + su.crit;
        int glovesPREC = pr.prec + ma.prec + eq.prec + su.prec;
        int glovesDODGE = pr.dodge + ma.dodge + eq.dodge + su.dodge;
        float valueArmor = DistReal<int>(108, 144) * (1 + (LVX * 0.2)) * rarity_value;

        G.Name_GL = glovesName;
        G.Type_GL = TYPES[GloveType];
        G.Rarity_GL = rarity;
        G.ATK_GL = RangeStats<int>(glovesATK, rarity_value, LVX);
        G.MAG_GL = RangeStats<int>(glovesMAG, rarity_value, LVX);
        G.DEF_GL = RangeStats<int>(glovesDEF, rarity_value, LVX);
        G.CRIT_GL = SingleStats<float>(glovesCRIT, rarity_value, LVX);
        G.PREC_GL = RangeStats<int>(glovesPREC, rarity_value, LVX);
        G.DODGE_GL = RangeStats<int>(glovesDODGE, rarity_value, LVX);
        G.LV_GL = LVX;
        G.Value_GL = static_cast<int>(ceil(valueArmor));

    }

    void character::CreateNeckle(necklace& RandNeckle){

        struct neckset{

            string name;
            int hp;
            int atk;
            int mag;
            int luck;

        };

        vector<neckset> Type{
        {"Health", 5, 0, 0, 2},
        {"Attack", 0, 5, 0, 2},
        {"Magic", 0, 0, 5, 2},
        {"Enchanted", 0, 3, 3, 2},
        {"Lucky", 0, 0, 0, 6}
        };

        vector<vector<neckset>> Material{
        {{"Obisidian", 6, 0, 0, 2},//Health
        {"Emerald", 5, 0, 0, 3},
        {"Diamond", 4, 0, 0, 4}},
        {{"Ruby", 0, 6, 0, 2},//Attack
        {"Garnet", 0, 5, 0, 3},
        {"Onyx", 0, 4, 0, 4}},
        {{"Amethyst", 0, 0, 6, 2},//Magic
        {"Aquamarine", 0, 0, 5, 3},
        {"Crystal", 0, 0, 4, 4}},
        {{"Blessed", 0, 3, 3, 2},//Enchanted
        {"Bloody", 0, 4, 2, 2},
        {"Curssed", 0, 2, 4, 2}},
        {{"Moonstone", 2, 0, 0, 5},//Lucky
        {"Topaz", 0, 2, 0, 5},
        {"Pearl", 0, 0, 2, 5}}
        };

        vector<neckset> EquipType{
        {"Neclke", 5, 1, 1, 2},//Health
        {"Pendant", 1, 5, 1, 2},//Attack
        {"Amulet", 1, 1, 5, 2},//Magic
        {"Collar", 1, 3, 3, 2},//Enchanted
        {"Choker", 2, 2, 2, 5}//Lucky
        };

        vector<vector<neckset>> Sufix{
        {{"of Mirai", 5, 2, 0, 2},//Health
        {"of Life", 5, 0, 2, 2},
        {"of Obssesion", 5, 1, 1, 2}},
        {{"of Wrath", 2, 5, 0, 2},//Attack
        {"of Fury", 0, 5, 2, 2},
        {"of Hate", 1, 5, 1, 2}},
        {{"of Ocultism", 2, 0, 5, 2},//Magic
        {"of Demons", 0, 2, 5, 2},
        {"of Angels", 1, 1, 5, 2}},
        {{"of Duality", 0, 3, 3, 2},//Enchanted
        {"of Sacrifice", 2, 2, 3, 2},
        {"of Leprosy", 2, 3, 2, 2}},
        {{"of Greed", 3, 1, 1, 5},//Lucky
        {"of Clauneck", 1, 3, 1, 5},
        {"of Barachiel", 1, 1, 3, 5}}
        };

        int TypeIndex = DistReal<int>(0, Type.size()-1);
        int MatIndex = DistReal<int>(0, Material[TypeIndex].size()-1);
        int EquipIndex = DistReal<int>(0, EquipType.size()-1);
        int SufixIndex = DistReal<int>(0, Sufix[TypeIndex].size()-1);
        int LVX = LvDet(LV);

        necklace& N = RandNeckle;
        neckset& Ty = Type[TypeIndex];
        neckset& Ma = Material[TypeIndex][MatIndex];
        neckset& Eqp = EquipType[EquipIndex];
        neckset& Su = Sufix[TypeIndex][SufixIndex];

        string NeckName = Ty.name + " " + Ma.name + " " + Eqp.name + " " + Su.name;
        string TypeN = Ty.name;
        int NeckHP = Ty.hp + Ma.hp + Eqp.hp + Su.hp;
        int NeckATK = Ty.atk + Ma.atk + Eqp.atk + Su.atk;
        int NeckMAG = Ty.mag + Ma.mag + Eqp.mag + Su.mag;
        int NeckLUCK = Ty.luck + Ma.luck + Eqp.luck + Su.luck;
        auto [rarity, RarityVal] = RarityGen();
        float valueNeck = DistReal<int>(108, 144) * (1 + (LVX * 0.2)) * RarityVal;

        N.Name_N = NeckName;
        N.Type_N = TypeN;
        N.Rarity_N = rarity;
        N.HP_N = SingleStats<int>(NeckHP, RarityVal, LVX);
        N.ATK_N = RangeStats<int>(NeckATK, RarityVal, LVX);
        N.MAG_N = RangeStats<int>(NeckMAG, RarityVal, LVX);
        N.LUCK_N = SingleStats<int>(NeckLUCK, RarityVal, LVX);
        N.LV_N = LVX;
        N.Value_N = static_cast<int>(ceil(valueNeck));

    }

    void character::EquipFound(){
    int choice1, slot, page, sacrifice, repeat;
    string equip;
    weapon WeapX;
    armor ArmorX;
    gloves GloveX;
    necklace NeckleX;

        int type = DistReal<int>(1, 4);

        switch(type){
            case 1:
                CreateWeapon(WeapX);
                equip = WeapX.Name_W;
                sacrifice = static_cast<int>(ceil(WeapX.Value_W/3));
                break;
            case 2:
                CreateArmor(ArmorX);
                equip = ArmorX.Name_A;
                sacrifice = static_cast<int>(ceil(ArmorX.Value_A/3));
                break;
            case 3:
                CreateGloves(GloveX);
                equip = GloveX.Name_GL;
                sacrifice = static_cast<int>(ceil(GloveX.Value_GL/3));
                break;
            case 4:
                CreateNeckle(NeckleX);
                equip = NeckleX.Name_N;
                sacrifice = static_cast<int>(ceil(NeckleX.Value_N/3));
                break;
        }

        enter("\n[" + equip + " -> FOUND]", 65);

        switch(type){
            case 1:
                WeapX.WeaponInfo();
                break;
            case 2:
                ArmorX.ArmorInfo();
                break;
            case 3:
                GloveX.GlovesInfo();
                break;
            case 4:
                NeckleX.NeckleInfo();
        }

        equipment& E = Equip;
        cout << "\n[KEEP OR SACRIFICE?]" << endl;
        cout << "[1].[KEEP IT]" << endl;
        cout << "[2].[SACRIFICE]:[" << sacrifice << "Souls]"<< endl;
        choice1 = VerifyCin(1, 2);

        if(choice1 == 1){

            switch(type){
                case 1:
                    for(int i = 0; i < E.WeaponInv.size(); ++i){
                        cout << "[" << i+1 << "][" << E.WeaponInv[i].Name_W << "]" << endl;
                    }
                    break;
                case 2:
                    for(int i = 0; i < E.ArmorInv.size(); ++i){
                        cout << "[" << i+1 << "][" << E.ArmorInv[i].Name_A << "]" << endl;
                    }
                    break;
                case 3:
                    for(int i = 0; i < E.GloveInv.size(); ++i){
                        cout << "[" << i+1 << "][" << E.GloveInv[i].Name_GL << "]" << endl;
                    }
                    break;
                case 4:
                    for(int i = 0; i < E.NeckInv.size(); ++i){
                        cout << "[" << i+1 << "][" << E.NeckInv[i].Name_N << "]" << endl;
                    }

                    break;
            }

            cout << "\n[SELECT SLOT[1-8] TO STORE THE EQUIPMENT]" << endl;
            slot = VerifyCin(1, 8) - 1;

                switch(type){
                    case 1:
                        Equip.WeaponInv[slot] = WeapX;
                        printSlowly("\n[" + WeapX.Name_W + "] placed on inventory", 60);
                        break;
                    case 2:
                        Equip.ArmorInv[slot] = ArmorX;
                        printSlowly("\n[" + ArmorX.Name_A + "] placed on inventory", 60);
                        break;
                    case 3:
                        Equip.GloveInv[slot] = GloveX;
                        printSlowly("\n[" + GloveX.Name_GL + "] placed on inventory", 60);
                        break;
                    case 4:
                        Equip.NeckInv[slot] = NeckleX;
                        printSlowly("\n[" + NeckleX.Name_N + "] placed on inventory", 60);
                        break;
                }
        }
        else{
            Itens.Souls += sacrifice;
            enter("[" + equip + "] sacificed to:[" + to_string(sacrifice) + " Souls]\n", 60);
            return;
        }
    return;
    };

    void GenEnemy(enemy& Enemy, int dificulty, int level){

        struct set{

            int hp;
            int atk;
            int mag;
            int def;
            float crit;
            int luck;
            int prec;
            int dodge;
            int ap;
            int mp;

        };

        vector<pair<string, set>> Soul{
        {"Angel", {300, 40, 25, 35, 1.30, 25, 40, 55, 240, 220}},//Angel
        {"Demon", {320, 50, 10, 40, 1.25, 20, 40, 50, 260, 180}},//Demon
        {"Human", {280, 45, 15, 35, 1.35, 30, 40, 50, 200, 320}},//Human
        {"Creature", {310, 40, 25, 30, 1.20, 25, 45, 55, 220, 220}},//Creature
        {"Unknown", {275, 25, 35, 35, 1.35, 35, 45, 55, 200, 240}}//Unknown
        };

        int Select = DistReal<int>(0, Soul.size()-1);
        float rar;
        switch(dificulty){
            case 1:
                rar = 0.8f;
                break;
            case 2:
                rar = 1.0f;
                break;
            case 3:
                rar = 1.2f;
                break;
            case 4:
                rar = 1.4f;
                break;
            case 5:
                rar = 1.8f;
                break;
        }
        float rarity = NumRound(rar);

        enemy& E = Enemy;
        set& S = Soul[Select].second;
        string Race = Soul[Select].first;

        E.Name_EN = EnemyNameGen(Race);
        E.Race_EN = Race;
        E.HP_EN.MAX = SingleStats<int>(S.hp, rarity, level);
        E.HP_EN.MIN = E.HP_EN.MAX;
        E.ATK_EN = RangeStats<int>(S.atk, rarity, level);
        E.MAG_EN = RangeStats<int>(S.mag, rarity, level);
        E.DEF_EN = RangeStats<int>(S.def, rarity, level);
        E.CRIT_EN = SingleStats<float>(S.crit, rarity, level);
        E.LUCK_EN = SingleStats<int>(S.luck, rarity, level);//Agility, crit chance %
        E.PREC_EN = RangeStats<int>(S.prec, rarity, level);//precision, chance of dealing damage %
        E.DODGE_EN = RangeStats<int>(S.dodge, rarity, level);
        E.AP_EN.MAX = SingleStats<int>(S.ap, rarity, level);
        E.AP_EN.MIN = E.AP_EN.MAX;
        E.MP_EN.MAX = SingleStats<int>(S.mp, rarity, level);
        E.MP_EN.MIN = E.MP_EN.MAX;
        E.LV_EN = level;

    };

    void character::CreateAbility(ability& RandAbility, int type){

        struct hability{

            string name;
            float dmg;
            float mag;
            float heal;
            int times;
            int ap;
            int mp;

        };

        vector<vector<vector<hability>>> Prefix{
        {{{"Sharp", 1.06, 0, 0, 1, 25, 0},//blade
        {"Quick", 1.03, 0, 0, 1, 20, 0},
        {"Strong", 1.09, 0, 0, 1, 30, 0},
        {"Fast", 1.12, 0, 0, 1, 40, 0}},
        {{"Lacerating", 0.26, 0, 0, 3, 30, 0},//dagger
        {"Furious", 0.23, 0, 0, 4, 35, 5},
        {"Stealth", 1.18, 0, 0, 1, 25, 0},
        {"Vampiric", 0.42, 0, 0.10, 2, 45, 5}},
        {{"Heavy", 1.18, 0, 0, 1, 40, 0},//blunt
        {"Brutal", 1.21, 0, 0, 1, 45, 0},
        {"Smasher", 1.12, 0, 0, 1, 30, 0},
        {"Soft", 1.03, 0, 0, 1, 20, 0}},
        {{"Pointed", 1.21, 0, 0, 1, 45, 0},//perfuration
        {"Sequencial", 0.32, 0, 0, 3, 35, 5},
        {"Sanguinare", 0.43, 0, 0.05, 2, 50, 10},
        {"Perfurating", 1.09, 0, 0, 1, 20, 0}}},
        {{{"Soul", 0, 1.03, 0, 1, 0, 20},//wand
        {"Ocult", 0, 1.09, 0, 1, 0, 30},
        {"Sparking", 0, 1.12, 0, 1, 0, 35},
        {"Healing", 0, 0.10, 0.30, 1, 0, 30}},
        {{"Flaming", 0, 1.12, 0, 1, 5, 30},//staff
        {"Aquatic", 0, 1.18, 0, 1, 5, 35},
        {"Obscure", 0, 1.21, 0, 1, 0, 25},
        {"Spiritual", 0, 0.40, 0, 2, 0, 30}},
        {{"Macabre", 0, 1.12, 0, 1, 5, 35},//idoll
        {"Satanic", 0, 1.09, 0, 1, 5, 30},
        {"Spook", 0, 0.40, 0, 2, 5, 30},
        {"Cannibal", 0, 0.70, 0.20, 1, 10, 25}},
        {{"Holy", 0, 1.12, 0, 1, 0, 25},//book
        {"Unholy", 0, 1.15, 0, 1, 0, 20},
        {"Saint", 0, 1.12, 0, 1, 0, 30},
        {"Blasphemous", 0, 0.80, 0.10, 1, 0, 30}}},
        {{{"Sparking", 1.06, 1.06, 0, 1, 20, 20},//magic blade
        {"Flaming", 1.09, 1.09, 0, 1, 30, 30},
        {"Spiritual", 1.06, 1.12, 0, 1, 30, 35},
        {"Destructive", 0.42, 0.42, 0, 2, 35, 35}},
        {{"Flashing", 0.26, 0.26, 0, 3, 30, 30},//magic dagger
        {"Slasher", 0.20, 0.20, 0, 4, 30, 30},
        {"Stealth", 1.18, 1.03, 0, 1, 30, 5},
        {"Vampiric", 0.38, 0.38, 0.03, 2, 25, 25}},
        {{"Holy", 1.18, 1.06, 0, 1, 40, 20},//magic blunt
        {"Soul", 1.09, 1.03, 0, 1, 10, 20},
        {"Sacred", 1.03, 1.09, 0, 1, 20, 10},
        {"Cursed", 1.12, 1.12, 0, 1, 30, 30}},
        {{"Fast", 1.06, 1.06, 0, 1, 15, 15},//orb
        {"Explosive", 1.03, 1.12, 0, 1, 20, 40},
        {"Angelical", 0.38, 0.38, 0, 2, 30, 20},
        {"Bloody", 0.22, 0.22, 0.03, 4, 10, 10}}}
        };

        vector<vector<vector<hability>>> Base{
        {{{"Cut", 0.05, 0.0, 0, 0, 5, 0},//blade
        {"Slash", 0.10, 0.0, 0, 0, 10, 0},
        {"Laceration", 0.13, 0.0, 0, 0, 10, 0},
        {"Double Strike", -0.50, 0.0, 0, 1, 20, 0}},
        {{"Slash", 0.10, 0.0, 0, 0, 5, 0},//dagger
        {"Strike", 0.20, 0.0, 0, 0, 10, 0},
        {"Stab", 0.20, 0, 0, 0, 10, 5},
        {"Drain", 0.05, 0, 0.05, 0, 15, 0}},
        {{"Strike", 0.10, 0, 0, 0, 20, 0},//blunt
        {"Hit", 0.05, 0, 0, 0, 10, 0},
        {"Smash", 0.15, 0, 0, 0, 20, 0},
        {"Butt", 0.10, 0, 0, 0, 15, 5}},
        {{"Hit", 0.10, 0, 0, 0, 15, 0},//perfuration
        {"Pierce", 0.15, 0, 0, 0, 15, 5},
        {"Needles", 0.10, 0, 0.05, 0, 20, 10},
        {"Perfuration", 0.15, 0.00, 0, 0, 20, 0}}},
        {{{"Spear", 0.00, 0.10, 0, 0, 0, 10},//wand
        {"Arrow", 0.00, 0.20, 0, 0, 0, 20},
        {"Ball", 0, 0.25, 0, 0, 0, 30},
        {"Heal", 0, 0.05, 0.10, 0, 0, 20}},
        {{"Great Arrow", 0, 0.20, 0, 0, 0, 20},//staff
        {"Spark", 0, 0.20, 0, 0, 5, 20},
        {"Chaos Ball", 0.00, 0.25, 0, 0, 0, 25},
        {"Earthquake", 0, 0.15, 0, 0, 10, 20}},
        {{"Ritual", 0.00, 0.30, 0, 0, 0, 25},//idoll
        {"Summon", 0.0, 0.20, 0, 0, 5, 20},
        {"Sacrifice", 0, 0.25, 0, 0, 10, 25},
        {"Heal", 0.00, 0.10, 0.15, 0, 5, 20}},
        {{"Spelling", 0.00, 0.25, 0, 0, 0, 25},//book
        {"Summon", 0.00, 0.15, 0, 0, 0, 15},
        {"Evoke", 0, 0.20, 0, 0, 5, 20},
        {"Manipulation", 0.00, 0.05, 0.7, 0, 5, 20}}},
        {{{"Cut", 0.05, 0.05, 0, 0, 5, 5},//magic blade
        {"Slash", 0.10, 0.05, 0, 0, 10, 5},
        {"Laceration", 0.13, 0.05, 0, 0, 13, 5},
        {"Strikes", -0.30, -0.30, 0, 1, 25, 25}},
        {{"Slash", 0.10, 0.05, 0, 0, 10, 5},//magic dagger
        {"Strike", 0.20, 0.10, 0, 0, 20, 10},
        {"Stab", 0.10, 0.20, 0, 0, 10, 20},
        {"Drain", 0.05, 0.05, 0.05, 0, 15, 15}},
        {{"Strike", 0.10, 0.0, 0, 0, 20, 0},//magic blunt
        {"Hit", 0.05, 0.5, 0, 0, 5, 5},
        {"Smash", 0.15, 0.15, 0, 0, 15, 15},
        {"Butt", 0.15, 0.05, 0, 0, 15, 5}},
        {{"Hit", 0.05, 0.10, 0, 0, 5, 10},//orb
        {"Spark", 0.05, 0.15, 0, 0, 5, 15},
        {"Moves", 0.20, 0.05, 0, 0, 20, 5},
        {"Dance", 0.10, 0.15, 0, 0, 10, 15}}}
        };

        vector<vector<hability>> Sufix{
        {{"of Warriors", 0.03, 0.0, 0, 0, 5, 0},//melee
        {"of Destruction", 0.06, 0.0, 0, 0, 10, 0},
        {"of Beasts", 0.09, 0, 0, 0, 15, 0},
        {"of Fury", 0.12, 0.00, 0, 0, 20, 0}},
        {{"of Wisdom", 0, 0.06, 0, 0, 0, 10},//magic
        {"of Serenity", 0.00, 0.09, 0, 0, 0, 15},
        {"of Chaos", 0, 0.03, 0, 0, 0, 5},
        {"of Cure", 0, 0.04, 0.10, 0, 0, 25}},
        {{"of Demons", 0.09, 0.03, 0, 0, 15, 5},//enchanted
        {"of Angels", 0.03, 0.09, 0, 0, 5, 15},
        {"of Ancients", 0.12, 0.12, 0, 0, 20, 20},
        {"of Distortion", 0.12, 0.06, 0, 0, 20, 10}}
        };

        int ClassIndex = type;//melee, magic, enchanted
        int TypeIndex = DistReal<int>(0, 3);//4 ability types per class
        auto [rarity, RarityVal] = RarityGen();

        int PreIndex = DistReal<int>(0, Prefix[ClassIndex][TypeIndex].size()-1);
        int BaseIndex = DistReal<int>(0, Base[ClassIndex][TypeIndex].size()-1);
        int SufixIndex = DistReal<int>(0, Sufix[ClassIndex].size()-1);

        ability& A = RandAbility;
        hability& Pr = Prefix[ClassIndex][TypeIndex][PreIndex];
        hability& Ba = Base[ClassIndex][TypeIndex][BaseIndex];
        hability& Su = Sufix[ClassIndex][SufixIndex];

        string Name = Pr.name + " " + Ba.name + " " + Su.name;
        float Dmg = Pr.dmg + Ba.dmg + Su.dmg;
        float Mag = Pr.mag + Ba.mag + Su.mag;
        float Heal = Pr.heal + Ba.heal + Su.heal;
        int Times = Pr.times + Ba.times + Su.times;
        int Ap = Pr.ap + Ba.ap + Su.ap;
        int Mp = Pr.mp + Ba.mp + Su.mp;

        A.Name_AB = Name;
        A.Type_AB = SetType(ClassIndex, TypeIndex);
        A.Rarity_AB = rarity;
        A.DMG_AB = SingleStats<float>(Dmg, RarityVal, 1);
        A.MAG_AB = SingleStats<float>(Mag, RarityVal, 1);
        A.Heal_AB = SingleStats<float>(Heal, RarityVal, 1);
        A.Times_AB = Times;
        A.AP_Cost = SingleStats<int>(Ap, RarityVal, 1);
        A.MP_Cost = SingleStats<int>(Mp, RarityVal, 1);
    }

    void character::ShowAbility(int abi){
        ability& AB = Equip.Abilities[abi];
        cout << "\n[" << (abi+1) << "][" << AB.Name_AB << "]" << endl;
        cout << "[CLASS][TYPE]:" << endl;
        cout << "[" << AB.Type_AB[0] << "][" << AB.Type_AB[1] << "]" << endl;
        cout << "[ATK/MAG/HEAL]:" << endl;
        cout << "[" << AB.DMG_AB << "/" << AB.MAG_AB << "/" << AB.Heal_AB << "]" << endl;
        cout << "[TIMES/AP/MP]" << endl;
        cout << "[" << AB.Times_AB << "/" << AB.AP_Cost << "/" << AB.MP_Cost << "]" << endl;
    }

    int character::CurseDamage(enemy& Enemy){
        int DodgeDist = DistReal<int>(Enemy.DODGE_EN.MIN, Enemy.DODGE_EN.MAX);
        if(CURSE_CHANCE > DodgeDist){
            int Damage = DistReal<int>(CURSE_DMG.MIN, CURSE_DMG.MAX);
            cout << "\n[YOU DID CURSE DAMAGE]:[" << Damage << "]" << endl;
            return Damage;
        }
        else{
            cout << "\n[CURSE MISS...]" << endl;
            return 0;
        }
    }

    bool character::Battle(enemy& E){
        int turn = 0;
        int abi, confirm, select, TotalDamage, TotalHeal, FinalDamage;
        string evoke;
        printSlowly("\n[BATTLE START!]\n", 120);
        do{
            ++turn;
            printSlowly("\n[TURN:" + to_string(turn) + "]", 80);
            cout << "\n[ENEMY HP:" << E.HP_EN.MIN << "/" << E.HP_EN.MAX << "]" << endl;
            cout << "[YOUR HP:" << HP.MIN << "/" << HP.MAX << "]" << endl;
            cout << "[YOUR AP:" << AP.MIN << "/" << AP.MAX << "]" << endl;
            cout << "[YOUR MP:" << MP.MIN << "/" << MP.MAX << "]\n" << endl;

            do{
                cout << "\n[YOUR ABILITIES]:" << endl;
                for(int i = 0; i < 4; ++i){
                ability& AB = Equip.Abilities[i];
                cout << "[" << (i+1) << "][" << AB.Name_AB << "]" << endl;
                }
                cout << "\n[SELECT ABILITY]:" << endl;
                abi = VerifyCin(1, 4)-1;
                ShowAbility(abi);
                cout << "\n[USE THIS ABILITY?]" << endl;
                cout << "[1].[USE ABILITY]" << endl;
                cout << "[2].[TRY ANOTHER]" << endl;
                confirm = VerifyCin(1, 2);

                if(confirm == 1){
                    cout << "\n[" << Equip.Abilities[abi].Name_AB << "]\n" << endl;
                    break;
                }
                else{
                    cout << "\n[SELECT ANOTHER ONE]\n" << endl;
                    continue;
                }

            }while(true);
            ability& A = Equip.Abilities[abi];
            weapon& W = Equip.Weapon;
            int TotalAtk = DistReal<int>(ATK.MIN, ATK.MAX) * A.DMG_AB;
            int TotalMag = DistReal<int>(MAG.MIN, MAG.MAX) * A.MAG_AB;
            TotalHeal = HP.MAX * A.Heal_AB;
            int CurseAtk = CurseDamage(E);
            int TotalDamage = 0;

            if(A.Type_AB[0] == "Melee"){
                TotalDamage = TotalAtk;
                cout << "[MELEE ATTACK:" << TotalDamage << "]" << endl;
                cout << "[HEAL:" << TotalHeal << "]" << endl;
            }
            else if(A.Type_AB[0] == "Magic"){
                TotalDamage = TotalMag;
                cout << "[MAGIC ATTACK:" << TotalDamage << "]" << endl;
                cout << "[HEAL:" << TotalHeal << "]" << endl;
            }
            else if(A.Type_AB[0] == "Enchanted"){
                TotalDamage = (TotalAtk + TotalMag)/2;
                cout << "[ENCHANTED ATTACK:" << TotalDamage << "]" << endl;
                cout << "[HEAL:" << TotalHeal << "]" << endl;
            }

            if(CurseAtk > 0){
                cout << "[CURSE ATTACK:" << CurseAtk << "]" << endl;
                TotalDamage += CurseAtk;
            }

            if(A.Type_AB[0] != W.Type_W[0]){
                printSlowly("\n[DIFERENT WEAPON CLASS, 10% OF PENALITY]\n", 60);
                TotalDamage *= 0.90;
                cout << "[ATTACK:" << TotalDamage << "]" << endl;
            }

            if(A.Type_AB[1] != W.Type_W[1]){
                printSlowly("\n[DIFERENT WEAPON TYPE, 10% OF PENALITY]\n", 60);
                TotalDamage *= 0.90;
                cout << "[ATTACK:" << TotalDamage << "]" << endl;
            }

            int DetLuck = DistReal<int>(1, 100);
            float CharCrit = CRIT;

            if(LUCK > 100){
                CharCrit += (LUCK - 100)/100;
            }

            if(LUCK > DetLuck){
                printSlowly("\n[CRITICAL HIT!][" + Percent(CharCrit) + "]\n", 60);
                TotalDamage *= CharCrit;
                cout << "[ATTACK:" << TotalDamage << "]" << endl;
            }

            int precision = DistReal<int>(PREC.MIN, PREC.MAX);
            enter("\nYOUR PRECISION:" + to_string(precision) + "\n", 90);

            if(precision < E.DODGE_EN.MIN){
                printSlowly("\n[ALMOST MISS HIT, 10% OF PENALITY]\n", 60);
                TotalDamage *= 0.90;
                cout << "[ATTACK:" << TotalDamage << "]" << endl;
            }
            else if(precision > E.DODGE_EN.MAX){
                printSlowly("\n[PERFECT HIT, 10% OF ADVANTAGE]\n", 60);
                TotalDamage *= 1.10;
                cout << "[ATTACK:" << TotalDamage << "]" << endl;
            }
            else{
                printSlowly("\n[NORMAL TARGET HIT]\n", 60);
                cout << "[ATTACK:" << TotalDamage << "]" << endl;
            }

            int EnemyDef = DistReal<int>(E.DEF_EN.MIN, E.DEF_EN.MAX);
            enter("\nENEMY DEFENSE IS:" + to_string(EnemyDef) + "\n", 90);
            FinalDamage = TotalDamage - EnemyDef;

            if(FinalDamage < 1){
                printSlowly("\n[ENEMY RESISTED YOUR ATTACK]\n", 60);
            }
            else if(AP.MIN < A.AP_Cost || MP.MIN < A.MP_Cost){
                printSlowly("\n[NOT ENOUGHT AP OR MP, RECOVERING 40%...]\n", 60);
                AP.MIN += ceil(AP.MAX * 0.40);
                MP.MIN += ceil(MP.MAX * 0.40);

                if(AP.MIN > AP.MAX){
                    AP.MIN = AP.MAX;
                }

                if(MP.MIN > MP.MAX){
                    MP.MIN = MP.MAX;
                }
            }
            else{
                for(int i = 1; i <= A.Times_AB; ++i){
                    printSlowly("\n[YOU HIT THE ENEMY WITH: " + A.Name_AB + "]", 30);
                    printSlowly("\n[DAMAGE DELT: " + to_string(FinalDamage) + "]\n", 30);
                    E.HP_EN.MIN -= FinalDamage;
                }

                if(TotalHeal > 0){
                    printSlowly("\n[YOU HEALED YOURSELF WITH: " + to_string(TotalHeal) + "%]\n", 60);
                    HP.MIN += HP.MAX * TotalHeal;
                }
                AP.MIN -= A.AP_Cost;
                MP.MIN -= A.MP_Cost;
            }

            if(E.HP_EN.MIN <= 0){
                printSlowly("\n[ENEMY DEFEATED!]\n", 60);
                int gain = ceil((E.HP_EN.MAX + E.ATK_EN.MAX + E.MAG_EN.MAX)/3);
                Soul.S_EXP.MIN += gain;
                Itens.Souls += gain;
                Itens.Bones += 3;
                enter("\n[GAINED EXP/SOULS:" + to_string(gain) + "]\n", 60);
                EquipFound();
                LevelUp();
                return true;
            }

            enter("\n[ENEMY TURN]\n", 80);
            cout << "[ENEMY HP:" << E.HP_EN.MIN << "/" << E.HP_EN.MAX << "]" << endl;
            cout << "[YOUR HP:" << HP.MIN << "/" << HP.MAX << "]" << endl;

            int TotalAtkEn = DistReal<int>(E.ATK_EN.MIN, E.ATK_EN.MAX);
            int TotalMagEn = DistReal<int>(E.MAG_EN.MIN, E.MAG_EN.MAX);
            int TotalDamageEn = TotalAtkEn + TotalMagEn;
            int precisionEn = DistReal<int>(E.PREC_EN.MIN, E.PREC_EN.MAX);

            int DetLuckEn = DistReal<int>(1, 100);
            float EnemyCrit = E.CRIT_EN;

            if(E.LUCK_EN > 100){
               EnemyCrit += (E.LUCK_EN - 100)/100;
            }

            if(E.LUCK_EN > DetLuckEn){
                printSlowly("\n[ENEMY CRITICAL HIT!][" + Percent(EnemyCrit) + "]\n", 60);
                TotalDamageEn *= EnemyCrit;
            }

            enter("\nENEMY PRECISION:" + to_string(precisionEn) + "\n", 90);

            if(precisionEn < DODGE.MIN){
                printSlowly("\n[ENEMY ALMOST MISS HIT, 10% OF PENALITY]\n", 60);
                TotalDamageEn *= 0.90;
            }
            else if(precisionEn > DODGE.MAX){
                printSlowly("\n[ENEMY DID A PERFECT HIT, 10% OF ADVANTAGE]\n", 60);
                TotalDamageEn *= 1.10;
            }
            else{
                printSlowly("\n[ENEMY HIT YOU]\n", 60);
            }

            int Defense = DistReal<int>(DEF.MIN, DEF.MAX);
            enter("\nYOUR DEFENSE IS:" + to_string(Defense) + "\n", 90);
            int FinalDamageEn = TotalDamageEn - Defense;

            if(FinalDamageEn < 1){
                printSlowly("\n[YOU RESISTED ENEMY'S ATTACK]\n", 60);
            }
            else{
                printSlowly("\n[ENEMY HIT YOU WITH: " + HabNameEnemy(E.Race_EN) + "]\n", 60);
                printSlowly("[DAMAGE: " + to_string(FinalDamageEn) + "]\n", 60);
                HP.MIN -= FinalDamageEn;
            }

            if(HP.MIN <= 0){
                printSlowly("\n[YOU WERE DEFEATED!]\n\n", 60);
                return false;
            }

        }while(true);

    }

    void character::DeadReset(){
        equipment& E = Equip;
        atributes& At = Atributes;
        E.Weapon = weapon();
        E.Armor = armor();
        E.Gloves = gloves();
        E.Neckle = necklace();
        for(int i = 0; i < E.Abilities.size(); ++i){
            E.Abilities[i] = ability();
        }
        At.POINTS = 0;
        At.RES = 0;
        At.STR = 0;
        At.WIS = 0;
        At.DEX = 0;
        At.AGL = 0;
        UpdateAtributes();
        UpdateCharStatus();
    }

    void character::FoundEnemy(){

        enemy FOE;
        cout << "\n[HOLE LEVEL:" << Places.Hole << "]" << endl;
        cout << "[BATTLE DIFICULTY]" << endl;
        cout << "1.[EASY]" << endl;
        cout << "2.[NORMAL]" << endl;
        cout << "3.[HARD]" << endl;
        cout << "4.[EXTREME]" << endl;
        cout << "5.[LEGENDARY]" << endl;
        int dificulty = VerifyCin(1, 5);

        GenEnemy(FOE, dificulty, Places.Hole);

        FOE.EnemyInfo();

        printSlowly("[START BATTLE?\n", 40);
        cout << "[1].[FUCK YEAH]" << endl;
        cout << "[2].[HELL NO]" << endl;
        int ch1 = VerifyCin(1, 2);

        if(ch1 == 1){
            bool win = Battle(FOE);

            if(win){
                ++Places.Hole;
                cout << "\nBelial: let's go deeper...\n" << endl;
            }
            else{
                Places.Hole = 1;
                DeadReset();
                cout << "\nBelial: If you die in the Hole" << endl;
                cout << "\nBelial: You lose all execept inventory" << endl;
                printSlowly("\nBelial: Again, who are you?\n", 100);
                StartCharacter();
            }
        }
        else{
            printSlowly("\n[RUNNIG...]\n", 80);
        }

    return;
    }

    void character::StartCharacter(){
        Name = "KEITH";

        do{

        cout << "\nKeith: I'm a..." << endl;
        cout << "[1].[WARRIOR]" << endl;
        cout << "[2].[MAGE]" << endl;
        cout << "[3].[SPELLSWORD]" << endl;
        int CharClass = VerifyCin(1, 3);

        switch(CharClass){
            case 1:
                Soul = soul("Human", "Warrior", 300, {60, 70}, {30, 40}, {25, 35}, 30, {20, 35}, 1.25, 45, {40, 55}, {40, 55}, 200, 80, 1, {0, 140});
                Equip.Weapon = weapon("Battle Knife", {"Melee", "Daggers"}, "Sin", "[C][COMMON]", {15, 20}, {0, 0}, 5, 0.05, 5, {5, 10}, 1, 100);
                Equip.Abilities[0] = ability("Deep Cut", {"Melee", "Daggers"}, "[R][RARE]", 1.15, 0.0, 0.0, 1, 30, 5);
                break;
            case 2:
                Soul = soul("Human", "Mage", 280, {30, 40}, {65, 70}, {20, 30}, 30, {20, 35}, 1.20, 45, {40, 55}, {40, 55}, 80, 200, 1, {0, 140});
                Equip.Weapon = weapon("Old Staff", {"Magic", "Staff"}, "Sin", "[C][COMMON]", {0, 0}, {15, 20}, 5, 0.05, 5, {5, 10}, 1, 100);
                Equip.Abilities[0] = ability("Soul Arrow", {"Magic", "Staff"}, "[R][RARE]", 0.0, 1.15, 0.0, 1, 5, 30);
                break;
            case 3:
                Soul = soul("Human", "Spellsword", 290, {40, 50}, {40, 50}, {25, 30}, 30, {20, 35}, 1.10, 55, {40, 55}, {40, 55}, 160, 160, 1, {0, 140});
                Equip.Weapon = weapon("Flaming Sword", {"Enchanted", "Magic Blade"}, "Sin", "[C][COMMON]", {7, 10}, {7, 10}, 5, 0.05, 5, {5, 10}, 1, 100);
                Equip.Abilities[0] = ability("Ignated Slash", {"Enchanted", "Magic Blade"}, "[R][RARE]", 1.03, 1.03, 0.0, 1, 20, 20);
                break;
        }
        UpdateCharStatus();
        RestoreStats();
        CharInfo();
        cout << "\nBelial: Are you sure?" << endl;
        cout << "[1].[Not Sure...]" << endl;
        cout << "[2].[I'm Sure...]" << endl;
        int sure = VerifyCin(1, 2);

        if(sure == 1){
            continue;
        }
        else{
            break;
        }

        }while(true);
    }

    void character::Merchant(){
        do{
        cout << "\n[MERCHANT CENTER]" << endl;
        cout << "[SOULS:" << Itens.Souls << "]" << endl;
        cout << "[1].[WEAPON]" << endl;
        cout << "[2].[ARMOR]" << endl;
        cout << "[3].[GLOVES]" << endl;
        cout << "[4].[NECKLE]" << endl;
        cout << "[5].[LEAVE]" << endl;
        int merch = VerifyCin(1, 5);

        if(merch == 1){
            weapon W;
            CreateWeapon(W);
            int price = W.Value_W * 2.1;
            W.WeaponInfo();
            cout << "[1].[BUY WEAPON:" << price << "]" << endl;
            cout << "[2].[LEAVE IT]" << endl;
            int merc = VerifyCin(1, 2);

            if(merc == 1){
                if(Itens.Souls >= W.Value_W){
                    WeaponInvDisplay();
                    cout << "\n[SELECT SLOT]" << endl;
                    int slot = VerifyCin(1, 8)-1;
                    Equip.WeaponInv[slot] = W;
                    Itens.Souls -= W.Value_W;
                    cout << "\n[WEAPON PURCHATED]\n" << endl;
                }
                else{
                    printSlowly("\n[NOT ENOUGHT SOULS...]\n", 80);
                    continue;
                }
            }
            else{
                printSlowly("\n[LEAVING WEAPON SHOP...]\n", 80);
                continue;
            }
        }
        else if(merch == 2){
            armor A;
            CreateArmor(A);
            int price = A.Value_A * 2.1;
            A.ArmorInfo();
            cout << "[1].[BUY ARMOR:" << price << "]" << endl;
            cout << "[2].[LEAVE IT]" << endl;
            int merc = VerifyCin(1, 2);

            if(merc == 1){
                if(Itens.Souls >= price){
                    ArmorInvDisplay();
                    cout << "\n[SELECT SLOT]" << endl;
                    int slot = VerifyCin(1, 8)-1;
                    Equip.ArmorInv[slot] = A;
                    Itens.Souls -= price;
                    cout << "\n[ARMOR PURCHATED]\n" << endl;
                }
                else{
                    printSlowly("\n[NOT ENOUGHT SOULS...]\n", 80);
                    continue;
                }
            }
            else{
                printSlowly("\n[LEAVING ARMOR SHOP...]\n", 80);
                continue;
            }
        }
        else if(merch == 3){
            gloves G;
            CreateGloves(G);
            int price = G.Value_GL * 2.1;
            G.GlovesInfo();
            cout << "[1].[BUY GLOVES:" << price << "]" << endl;
            cout << "[2].[LEAVE IT]" << endl;
            int merc = VerifyCin(1, 2);

            if(merc == 1){
                if(Itens.Souls >= price){
                    GlovesInvDisplay();
                    cout << "\n[SELECT SLOT]" << endl;
                    int slot = VerifyCin(1, 8)-1;
                    Equip.GloveInv[slot] = G;
                    Itens.Souls -= price;
                    cout << "\n[GLOVES PURCHATED]\n" << endl;
                }
                else{
                    printSlowly("\n[NOT ENOUGHT SOULS...]\n", 80);
                    continue;
                }
            }
            else{
                printSlowly("\n[LEAVING GLOVES SHOP...]\n", 80);
                continue;
            }
        }
        else if(merch == 4){
            necklace N;
            CreateNeckle(N);
            int price = N.Value_N * 2.1;
            N.NeckleInfo();
            cout << "[1].[BUY NECKLE:" << price << "]" << endl;
            cout << "[2].[LEAVE IT]" << endl;
            int merc = VerifyCin(1, 2);

            if(merc == 1){
                if(Itens.Souls >= price){
                    NeckleInvDisplay();
                    cout << "\n[SELECT SLOT]" << endl;
                    int slot = VerifyCin(1, 8)-1;
                    Equip.NeckInv[slot] = N;
                    Itens.Souls -= price;
                    cout << "\n[NECKLE PURCHATED]\n" << endl;
                }
                else{
                    printSlowly("\n[NOT ENOUGHT SOULS...]\n", 80);
                    continue;
                }
            }
            else{
                printSlowly("\n[LEAVING NECKLE SHOP...]\n", 80);
                continue;
            }
        }
        else{
            enter("\n[LEAVING MERCHANT...]\n", 80);
            break;
        }

        }while(true);
    }

    void character::AbilityManeger(){
        do{
        printSlowly("\n\n[ABILITY MENEGER]\n", 80);
        cout << "[1].[SHOW ABILITIES]" << endl;
        cout << "[2].[ABILITIES INVENTORY]" << endl;
        cout << "[3].[CHANGE ABILITIES]" << endl;
        cout << "[4].[BUY ABILITY]" << endl;
        cout << "[5].[LEAVE]" << endl;
        int menu = VerifyCin(1, 5);

        if(menu == 1){
            do{
            cout << "[YOUR ABILITIES]:" << endl;
            for(int i = 0; i < 4; ++i){
                ability& AB = Equip.Abilities[i];
                cout << "\n[" << (i+1) << "].[" << AB.Name_AB << "]" << endl;
            }
            cout << "[5].[LEAVE]" << endl;
            cout << "[SELECT ABILITY]:" << endl;
            int abi = VerifyCin(1, 5)-1;

            if(abi >= 0 && abi < 4){
                Equip.Abilities[abi].AbilityInfo();
            }
            else if(abi == 4){
                printSlowly("\nLEAVING...\n", 60);
                break;
            }

            }while(true);

        }
        else if(menu == 2){
        do{
            cout << "[INVENTORY ABILITIES]:" << endl;
            for(int i = 0; i < 8; ++i){
                ability& AB = Equip.AbilityInv[i];
                cout << "\n[" << (i+1) << "].[" << AB.Name_AB << "]" << endl;
            }
            cout << "[9].[LEAVE]" << endl;
            cout << "[SELECT ABILITY]:" << endl;
            int abi = VerifyCin(1, 9)-1;

            if(abi >= 0 && abi < 8){
                Equip.AbilityInv[abi].AbilityInfo();
            }
            else if(abi == 8){
                printSlowly("\nLEAVING...\n", 60);
                break;
            }

            }while(true);

        }else if(menu == 3){
            cout << "[YOUR ABILITIES]:" << endl;
            for(int i = 0; i < 4; ++i){
                ability& AB = Equip.Abilities[i];
                cout << "\n[" << (i+1) << "].[" << AB.Name_AB << "]" << endl;
            }
            cout << "[INVENTORY ABILITIES]:" << endl;
            for(int i = 0; i < 8; ++i){
                ability& AB = Equip.AbilityInv[i];
                cout << "\n[" << (i+1) << "].[" << AB.Name_AB << "]" << endl;
            }
            cout << "[SELECT ABILITY]:" << endl;
            int abi = VerifyCin(1, 4)-1;
            cout << "[SELECT INVENTORY]:" << endl;
            int inv = VerifyCin(1, 8)-1;
            swap(Equip.Abilities[abi], Equip.AbilityInv[inv]);
            enter("[ABILITIES CHANGED]", 80);
            continue;
        }
        else if(menu == 4){
            do{
            printSlowly("\n[ABILITY SHOP]\n", 60);
            cout << "[YOUR BONES" << Itens.Bones << "]" << endl;
            cout << "[1].[BUY]" << endl;
            cout << "[2].[EXIT]" << endl;
            int buy = VerifyCin(1, 2);

            if(buy == 1){
                do{
                if(Itens.Bones > 0){
                    ability Hab;
                    --Itens.Bones;
                    printSlowly("\n[GENERATING ABILITY...]\n", 60);
                    printSlowly("\n[WHAT TYPE?]\n", 80);
                    cout << "[1].[MELEE]" << endl;
                    cout << "[2].[MAGIC]" << endl;
                    cout << "[3].[ENCHANTED]" << endl;
                    int type = VerifyCin(1, 3)-1;
                    CreateAbility(Hab, type);
                    Hab.AbilityInfo();
                    printSlowly("\n[KEEP ON INVENTORY?]\n", 80);
                    cout << "[1].[KEEP IT]" << endl;
                    cout << "[2].[BUY AGAIN]" << endl;
                    cout << "[3].[EXIT]" << endl;
                    int ab = VerifyCin(1, 3);

                    if(ab == 1){
                        cout << "[SELECT INVENTORY]:" << endl;
                        int inv = VerifyCin(1, 8)-1;
                        Equip.AbilityInv[inv] = Hab;
                        printSlowly("\n[KEEPT...]\n", 80);
                        break;
                    }
                    else if(ab == 2){
                        printSlowly("\n[BUY ANOTHER...]\n", 80);
                        continue;
                    }
                    else{
                        printSlowly("\n[LEAVING SHOP...]\n", 80);
                        break;
                    }

                }
                else{
                    printSlowly("\n[YOU HAVE NO BONES...]\n", 80);
                    break;
                }

                }while(true);

            }
            else{
                printSlowly("\n[LEAVING SHOP...]\n", 80);
                break;
            }

            }while(true);

        }
        else{
            printSlowly("\n[LEAVING MANAGER]\n", 60);
            break;
        }

        }while(true);
    }

    void character::CharacterMenu(){
        do{
        printSlowly("\n\n[CHARACTER MENU]\n", 80);
        cout << "[1].[EQUIPMENT MANEGER]" << endl;
        cout << "[2].[CHARACTER INFO]" << endl;
        cout << "[3].[ABILITY MANAGER]" << endl;
        cout << "[4].[DISTRIBUTE POINTS]" << endl;
        cout << "[5].[LEAVE MENU]" << endl;
        int menu = VerifyCin(1, 5);

        if(menu == 1){
            EquipManager();
            continue;
        }
        else if(menu == 2){
            CharInfo();
            continue;
        }
        else if(menu == 3){
            AbilityManeger();
            continue;
        }
        else if(menu == 4){
            PointsDist();
            continue;
        }
        else{
            printSlowly("\n[LEAVING MENU]\n", 60);
            break;
        }

        }while(true);

    }



    void character::TheWorld(){

        do{
        printSlowly("\n\n[THE WORLD OF SOULS]\n", 80);
        cout << "[1].[CHARACTER MENU]" << endl;
        cout << "[2].[MERCHANT CENTER]" << endl;
        cout << "[3].[HOSPITAL]" << endl;
        cout << "[4].[THE HOLE]" << endl;
        cout << "[5].[QUIT GAME]" << endl;
        int worldt = VerifyCin(1, 5);

        if(worldt == 1){
            CharacterMenu();
            continue;
        }
        else if(worldt == 2){
            Merchant();
            continue;
        }
        else if(worldt == 3){
            RestoreStats();
            continue;
        }
        else if(worldt == 4){
            do{
            printSlowly("\n\n[THE HOLE][DEPTH:" + to_string(Places.Hole) + "]\n", 80);
            cout << "[1].[FIGHT]" << endl;
            cout << "[2].[EXIT HOLE]" << endl;
            int depth = VerifyCin(1, 2);

            if(depth == 1){
                FoundEnemy();
                continue;
            }
            else if(depth == 2){
                printSlowly("\n[LEAVING THE HOLE...]\n", 80);
                break;
            }

            }while(true);

        }
        else{
            printSlowly("\n[LEAVING GAME...]\n", 80);
            break;
        }


        }while(true);
    }

int main(){

character YME;

    srand(static_cast<unsigned int>(time(0)));

    do{

    printSlowly("\nBLACK RO$ES: Lies VOL.1\n",100);
    cout << "[1].[LOAD SAVE]" << endl;
    cout << "[2].[NEW SAVE]" << endl;
    int play = VerifyCin(1, 2);

    if(play == 1){
        enter("Belial: Not now...", 60);
        continue;
    }
    else if(play == 2){
        printSlowly("\nBelial: Well, here there are, the human.", 60);
        printSlowly("\nBelial: How did you get here?", 60);
        printSlowly("\nBelial: Nevermind, tell me, who are you?", 60);
        YME.StartCharacter();
        enter("\nBelial: Don't die too fast...", 100);
        break;
    }

    }while(true);


    while(true){

        YME.TheWorld();
        break;

    }

return 0;
};
