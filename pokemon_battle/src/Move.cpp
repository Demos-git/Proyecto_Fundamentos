#include "Move.h"

using namespace std;

Move::Move(string n, Type t, MoveCategory cat, int pow, int acc, int pp_)
    : name(move(n)), type(t), category(cat),
      power(pow), accuracy(acc), pp(pp_), currentPP(pp_) {}
