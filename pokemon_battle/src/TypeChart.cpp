#include "TypeChart.h"

using namespace std;

// Indices: 0 Normal 1 Fuego 2 Agua 3 Planta 4 Electrico 5 Hielo 6 Lucha
// 7 Veneno 8 Tierra 9 Volador 10 Psiquico 11 Bicho 12 Roca 13 Fantasma
// 14 Dragon 15 Siniestro 16 Acero 17 None

TypeChart& TypeChart::getInstance() {
    static TypeChart instance;
    return instance;
}

TypeChart::TypeChart() {
    for (auto& row : chart)
        for (auto& cell : row)
            cell = 1.0f;
    loadDefaults();
}

int TypeChart::typeIndex(Type t) const { return static_cast<int>(t); }

float TypeChart::getMultiplier(Type attacker, Type defender) const {
    if (attacker == Type::NONE || defender == Type::NONE) return 1.0f;
    return chart[typeIndex(attacker)][typeIndex(defender)];
}

void TypeChart::loadDefaults() {
    auto set = [&](Type atk, Type def, float val) {
        chart[typeIndex(atk)][typeIndex(def)] = val;
    };

    set(Type::FIRE, Type::GRASS, 2.0f);  set(Type::FIRE, Type::ICE, 2.0f);
    set(Type::FIRE, Type::BUG, 2.0f);    set(Type::FIRE, Type::STEEL, 2.0f);
    set(Type::FIRE, Type::WATER, 0.5f);  set(Type::FIRE, Type::ROCK, 0.5f);
    set(Type::FIRE, Type::FIRE, 0.5f);   set(Type::FIRE, Type::DRAGON, 0.5f);

    set(Type::WATER, Type::FIRE, 2.0f);   set(Type::WATER, Type::GROUND, 2.0f);
    set(Type::WATER, Type::ROCK, 2.0f);   set(Type::WATER, Type::WATER, 0.5f);
    set(Type::WATER, Type::GRASS, 0.5f);  set(Type::WATER, Type::DRAGON, 0.5f);

    set(Type::GRASS, Type::WATER, 2.0f);  set(Type::GRASS, Type::GROUND, 2.0f);
    set(Type::GRASS, Type::ROCK, 2.0f);   set(Type::GRASS, Type::FIRE, 0.5f);
    set(Type::GRASS, Type::GRASS, 0.5f);  set(Type::GRASS, Type::POISON, 0.5f);
    set(Type::GRASS, Type::FLYING, 0.5f); set(Type::GRASS, Type::BUG, 0.5f);
    set(Type::GRASS, Type::DRAGON, 0.5f); set(Type::GRASS, Type::STEEL, 0.5f);

    set(Type::ELECTRIC, Type::WATER, 2.0f);  set(Type::ELECTRIC, Type::FLYING, 2.0f);
    set(Type::ELECTRIC, Type::GRASS, 0.5f);  set(Type::ELECTRIC, Type::ELECTRIC, 0.5f);
    set(Type::ELECTRIC, Type::DRAGON, 0.5f); set(Type::ELECTRIC, Type::GROUND, 0.0f);

    set(Type::ICE, Type::GRASS, 2.0f);   set(Type::ICE, Type::GROUND, 2.0f);
    set(Type::ICE, Type::FLYING, 2.0f);  set(Type::ICE, Type::DRAGON, 2.0f);
    set(Type::ICE, Type::WATER, 0.5f);   set(Type::ICE, Type::ICE, 0.5f);
    set(Type::ICE, Type::STEEL, 0.5f);   set(Type::ICE, Type::FIRE, 0.5f);

    set(Type::FIGHTING, Type::NORMAL, 2.0f); set(Type::FIGHTING, Type::ICE, 2.0f);
    set(Type::FIGHTING, Type::ROCK, 2.0f);   set(Type::FIGHTING, Type::DARK, 2.0f);
    set(Type::FIGHTING, Type::STEEL, 2.0f);  set(Type::FIGHTING, Type::POISON, 0.5f);
    set(Type::FIGHTING, Type::FLYING, 0.5f); set(Type::FIGHTING, Type::PSYCHIC, 0.5f);
    set(Type::FIGHTING, Type::BUG, 0.5f);    set(Type::FIGHTING, Type::GHOST, 0.0f);

    set(Type::POISON, Type::GRASS, 2.0f);  set(Type::POISON, Type::ROCK, 0.5f);
    set(Type::POISON, Type::GHOST, 0.5f);  set(Type::POISON, Type::GROUND, 0.5f);
    set(Type::POISON, Type::POISON, 0.5f); set(Type::POISON, Type::STEEL, 0.0f);

    set(Type::GROUND, Type::FIRE, 2.0f);     set(Type::GROUND, Type::ELECTRIC, 2.0f);
    set(Type::GROUND, Type::POISON, 2.0f);   set(Type::GROUND, Type::ROCK, 2.0f);
    set(Type::GROUND, Type::STEEL, 2.0f);    set(Type::GROUND, Type::GRASS, 0.5f);
    set(Type::GROUND, Type::BUG, 0.5f);      set(Type::GROUND, Type::FLYING, 0.0f);

    set(Type::FLYING, Type::GRASS, 2.0f);    set(Type::FLYING, Type::FIGHTING, 2.0f);
    set(Type::FLYING, Type::BUG, 2.0f);      set(Type::FLYING, Type::ROCK, 0.5f);
    set(Type::FLYING, Type::ELECTRIC, 0.5f); set(Type::FLYING, Type::STEEL, 0.5f);

    set(Type::PSYCHIC, Type::FIGHTING, 2.0f); set(Type::PSYCHIC, Type::POISON, 2.0f);
    set(Type::PSYCHIC, Type::PSYCHIC, 0.5f);  set(Type::PSYCHIC, Type::STEEL, 0.5f);
    set(Type::PSYCHIC, Type::DARK, 0.0f);

    set(Type::BUG, Type::GRASS, 2.0f);   set(Type::BUG, Type::PSYCHIC, 2.0f);
    set(Type::BUG, Type::DARK, 2.0f);    set(Type::BUG, Type::FIRE, 0.5f);
    set(Type::BUG, Type::FIGHTING, 0.5f); set(Type::BUG, Type::POISON, 0.5f);
    set(Type::BUG, Type::FLYING, 0.5f);  set(Type::BUG, Type::GHOST, 0.5f);
    set(Type::BUG, Type::STEEL, 0.5f);

    set(Type::ROCK, Type::FIRE, 2.0f);   set(Type::ROCK, Type::ICE, 2.0f);
    set(Type::ROCK, Type::FLYING, 2.0f); set(Type::ROCK, Type::BUG, 2.0f);
    set(Type::ROCK, Type::FIGHTING, 0.5f); set(Type::ROCK, Type::GROUND, 0.5f);
    set(Type::ROCK, Type::STEEL, 0.5f);

    set(Type::GHOST, Type::PSYCHIC, 2.0f); set(Type::GHOST, Type::GHOST, 2.0f);
    set(Type::GHOST, Type::DARK, 0.5f);    set(Type::GHOST, Type::NORMAL, 0.0f);

    set(Type::DRAGON, Type::DRAGON, 2.0f); set(Type::DRAGON, Type::STEEL, 0.5f);

    set(Type::DARK, Type::PSYCHIC, 2.0f);  set(Type::DARK, Type::GHOST, 2.0f);
    set(Type::DARK, Type::FIGHTING, 0.5f); set(Type::DARK, Type::DARK, 0.5f);

    set(Type::STEEL, Type::ICE, 2.0f);  set(Type::STEEL, Type::ROCK, 2.0f);
    set(Type::STEEL, Type::FIRE, 0.5f); set(Type::STEEL, Type::WATER, 0.5f);
    set(Type::STEEL, Type::ELECTRIC, 0.5f); set(Type::STEEL, Type::STEEL, 0.5f);
}
