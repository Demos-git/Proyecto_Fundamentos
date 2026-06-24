#include "Types.h"

using namespace std;

string typeToString(Type t) {
    switch (t) {
        case Type::NORMAL:   return "Normal";
        case Type::FIRE:     return "Fuego";
        case Type::WATER:    return "Agua";
        case Type::GRASS:    return "Planta";
        case Type::ELECTRIC: return "Electrico";
        case Type::ICE:      return "Hielo";
        case Type::FIGHTING: return "Lucha";
        case Type::POISON:   return "Veneno";
        case Type::GROUND:   return "Tierra";
        case Type::FLYING:   return "Volador";
        case Type::PSYCHIC:  return "Psiquico";
        case Type::BUG:      return "Bicho";
        case Type::ROCK:     return "Roca";
        case Type::GHOST:    return "Fantasma";
        case Type::DRAGON:   return "Dragon";
        case Type::DARK:     return "Siniestro";
        case Type::STEEL:    return "Acero";
        default:             return "---";
    }
}
