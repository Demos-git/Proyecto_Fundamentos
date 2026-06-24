#include "Pokemon.h"
#include "TypeChart.h"
#include <iostream>
#include <algorithm>
#include <fstream>

using namespace std;

string statusToString(StatusEffect s) {
    switch (s) {
        case StatusEffect::BURN:      return "[QMD]";
        case StatusEffect::POISON:    return "[VEN]";
        case StatusEffect::PARALYSIS: return "[PAR]";
        case StatusEffect::SLEEP:     return "[DRM]";
        case StatusEffect::FREEZE:    return "[HLD]";
        default:                      return "";
    }
}

Pokemon::Pokemon(const string& n, Type t1, Type t2,
                 Stats stats, vector<Move> mv, const string& artFile)
    : name(n), type1(t1), type2(t2), base(stats),
      maxHP(stats.hp), currentHP(stats.hp),
      status(StatusEffect::NONE), statusTurns(0),
      moves(move(mv)), art(artFile) {}

void Pokemon::takeDamage(int dmg) { currentHP = max(0, currentHP - dmg); }
void Pokemon::heal(int amount)    { currentHP = min(maxHP, currentHP + amount); }

float Pokemon::typeMultiplierReceived(Type attackType) const {
    auto& tc = TypeChart::getInstance();
    float mult = tc.getMultiplier(attackType, type1);
    if (type2 != Type::NONE) mult *= tc.getMultiplier(attackType, type2);
    return mult;
}

string Pokemon::statusString() const { return statusToString(status); }

void Pokemon::displayInfo() const {
    cout << name << " " << statusString()
              << "  HP: " << currentHP << "/" << maxHP << "\n";
}

void Pokemon::displayArt() const {
    if (art.empty()) return;
    ifstream f(art);
    if (!f.is_open()) return;
    string line;
    while (getline(f, line)) cout << line << "\n";
}
