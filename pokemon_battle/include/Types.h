#pragma once
#include <string>

using namespace std;

enum class Type {
    NORMAL, FIRE, WATER, GRASS, ELECTRIC,
    ICE, FIGHTING, POISON, GROUND, FLYING,
    PSYCHIC, BUG, ROCK, GHOST, DRAGON,
    DARK, STEEL, NONE
};

string typeToString(Type t);
