#include "DataLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

using namespace std;

static Type parseType(const    string& s) {
    static const   unordered_map<string, Type> table = {
        {"Normal", Type::NORMAL}, {"Fuego", Type::FIRE}, {"Agua", Type::WATER},
        {"Planta", Type::GRASS}, {"Electrico", Type::ELECTRIC}, {"Hielo", Type::ICE},
        {"Lucha", Type::FIGHTING}, {"Veneno", Type::POISON}, {"Tierra", Type::GROUND},
        {"Volador", Type::FLYING}, {"Psiquico", Type::PSYCHIC}, {"Bicho", Type::BUG},
        {"Roca", Type::ROCK}, {"Fantasma", Type::GHOST}, {"Dragon", Type::DRAGON},
        {"Siniestro", Type::DARK}, {"Acero", Type::STEEL}, {"Ninguno", Type::NONE}
    };
    auto it = table.find(s);
    return it != table.end() ? it->second : Type::NONE;
}

static MoveCategory parseCategory(const    string& s) {
    if (s == "Fisico")  return MoveCategory::PHYSICAL;
    if (s == "Especial") return MoveCategory::SPECIAL;
    return MoveCategory::STATUS;
}

// Formato esperado por linea en moves.txt:
// Nombre;Tipo;Categoria;Poder;Precision;PP
Move DataLoader::parseMove(const string& line) {
    stringstream ss(line);
    string name, typeStr, catStr, powerStr, accStr, ppStr;
    getline(ss, name, ';');
    getline(ss, typeStr, ';');
    getline(ss, catStr, ';');
    getline(ss, powerStr, ';');
    getline(ss, accStr, ';');
    getline(ss, ppStr, ';');

    return Move(name, parseType(typeStr), parseCategory(catStr),
                stoi(powerStr),    stoi(accStr),  stoi(ppStr));
}

vector<Move> DataLoader::loadMoves(const   string& path) {
    vector<Move> moves;
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "No se pudo abrir " << path << "\n";
        return moves;
    }
    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        moves.push_back(parseMove(line));
    }
    return moves;
}

// Formato esperado por linea en pokemon.txt:
// Nombre;Tipo1;Tipo2;HP;Atk;Def;SpAtk;SpDef;Spd;Move1,Move2,Move3,Move4;ArtFile
Pokemon DataLoader::parsePokemon(const string& line, const std::vector<Move>& movePool) {
    stringstream ss(line);
    string name, t1, t2, hp, atk, def, spa, spd, spe, movesStr, artFile;
    getline(ss, name, ';');
    getline(ss, t1, ';');
    getline(ss, t2, ';');
    getline(ss, hp, ';');
    getline(ss, atk, ';');
    getline(ss, def, ';');
    getline(ss, spa, ';');
    getline(ss, spd, ';');
    getline(ss, spe, ';');
    getline(ss, movesStr, ';');
    getline(ss, artFile, ';');

    Stats stats{   stoi(hp),  stoi(atk),   stoi(def),
                stoi(spa), stoi(spd),  stoi(spe)};

    vector<Move> chosenMoves;
    stringstream msplit(movesStr);
    string moveName;
    while (getline(msplit, moveName, ',')) {
        for (const auto& m : movePool) {
            if (m.name == moveName) { chosenMoves.push_back(m); break; }
        }
    }

    return Pokemon(name, parseType(t1), parseType(t2), stats, chosenMoves, artFile);
}

vector<Pokemon> DataLoader::loadPokemon(const  string& path,
                                              const    vector<Move>& movePool) {
    vector<Pokemon> list;
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "No se pudo abrir " << path << "\n";
        return list;
    }
    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        list.push_back(parsePokemon(line, movePool));
    }
    return list;
}
