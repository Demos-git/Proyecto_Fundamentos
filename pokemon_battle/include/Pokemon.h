#pragma once
#include <string>
#include <vector>
#include "Move.h"
#include "Types.h"

using namespace std;

enum class StatusEffect { NONE, BURN, POISON, PARALYSIS, SLEEP, FREEZE };

string statusToString(StatusEffect s);

struct Stats {
    int hp, attack, defense, spAtk, spDef, speed;
};

class Pokemon {
public:
    string  name;
    Type         type1;
    Type         type2;
    Stats        base;
    int          maxHP;
    int          currentHP;
    StatusEffect status;
    int          statusTurns;
    vector<Move> moves;
    string  art;

    Pokemon() = default;
    Pokemon(const string& name, Type t1, Type t2,
            Stats stats, vector<Move> moves, const string& artFile);

    bool  isFainted() const { return currentHP <= 0; }
    void  takeDamage(int dmg);
    void  heal(int amount);
    float typeMultiplierReceived(Type attackType) const;

    void  displayInfo() const;
    void  displayArt()  const;
    string statusString() const;
};
