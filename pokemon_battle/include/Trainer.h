#pragma once
#include <string>
#include <vector>
#include "Pokemon.h"

using namespace std;

class Trainer {
public:
    string name;
    vector<Pokemon> team;
    int activePokemon;

    Trainer() : activePokemon(0) {}
    explicit Trainer(const string& name) : name(name), activePokemon(0) {}

    Pokemon&       active()       { return team[activePokemon]; }
    const Pokemon& active() const { return team[activePokemon]; }

    bool hasUsablePokemon() const;
    bool switchTo(int index);
    int  chooseSwitch() const;

    void displayTeam()   const;
    void displayActive() const;
};
