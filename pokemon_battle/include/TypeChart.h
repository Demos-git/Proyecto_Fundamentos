#pragma once
#include "Types.h"

using namespace std;

// Tabla de efectividades entre tipos
// getMultiplier(atacante, defensor) -> 0.0, 0.5, 1.0, 2.0
class TypeChart {
public:
    static TypeChart& getInstance();
    float getMultiplier(Type attacker, Type defender) const;

private:
    TypeChart();
    float chart[18][18];
    void loadDefaults();
    int typeIndex(Type t) const;
};
