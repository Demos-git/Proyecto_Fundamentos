#pragma once
#include <string>
#include "Types.h"

using namespace std;

enum class MoveCategory { PHYSICAL, SPECIAL, STATUS };

struct Move {
    string name;
    Type        type;
    MoveCategory category;
    int         power;
    int         accuracy;
    int         pp;
    int         currentPP;

    Move() = default;
    Move(string n, Type t, MoveCategory cat, int pow, int acc, int pp);

    bool isUsable() const { return currentPP > 0; }
    void use()            { if (currentPP > 0) --currentPP; }
    void restore()        { currentPP = pp; }
};
