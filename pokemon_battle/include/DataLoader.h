#pragma once
#include <vector>
#include <string>
#include "Pokemon.h"
#include "Move.h"
using namespace std;

class DataLoader {
public:
    static vector<Move>    loadMoves(const string& path);
    static vector<Pokemon> loadPokemon(const string& path,
                                             const vector<Move>& movePool);
private:
    static Move    parseMove(const string& line);
    static Pokemon parsePokemon(const string& line,
                                const vector<Move>& movePool);
};
