#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Trainer.h"
#include "Battle.h"
#include "DataLoader.h"

using namespace std;

static Trainer buildPlayerTeam(const vector<Pokemon>& pool) {
    Trainer player("Jugador");
    cout << "=== Elige tu equipo (hasta 3 Pokemon) ===\n";
    for (size_t i = 0; i < pool.size(); ++i)
        cout << "  " << (i + 1) << ". " << pool[i].name
                  << " [" << typeToString(pool[i].type1) << "]\n";

    int count = 0;
    vector<bool> taken(pool.size(), false);
    while (count < 3) {
        cout << "Elige Pokemon #" << (count + 1) << ": ";
        int choice;
        cin >> choice;
        choice--;
        if (choice < 0 || choice >= static_cast<int>(pool.size()) || taken[choice]) {
            cout << "Seleccion invalida.\n";
            continue;
        }
        taken[choice] = true;
        player.team.push_back(pool[choice]);
        ++count;
    }
    return player;
}

static Trainer buildEnemyTeam(const vector<Pokemon>& pool) {
    Trainer enemy("Rival");
    vector<int> indices;
    for (size_t i = 0; i < pool.size(); ++i) indices.push_back(static_cast<int>(i));

    for (int i = 0; i < 3 && !indices.empty(); ++i) {
        int r = rand() % indices.size();
        enemy.team.push_back(pool[indices[r]]);
        indices.erase(indices.begin() + r);
    }
    return enemy;
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    cout << "============================================\n";
    cout << "       POKEMON BATTLE - EDICION CONSOLA\n";
    cout << "============================================\n\n";

    auto movePool = DataLoader::loadMoves("data/moves/moves.txt");
    if (movePool.empty()) {
        cerr << "Error: no se pudieron cargar los movimientos.\n";
        return 1;
    }

    auto pokePool = DataLoader::loadPokemon("data/pokemon/pokemon.txt", movePool);
    if (pokePool.empty()) {
        cerr << "Error: no se pudieron cargar los Pokemon.\n";
        return 1;
    }

    bool playAgain = true;
    while (playAgain) {
        Trainer player = buildPlayerTeam(pokePool);
        Trainer enemy  = buildEnemyTeam(pokePool);

        cout << "\nTu equipo:\n";
        player.displayTeam();
        cout << "\nEquipo rival:\n";
        enemy.displayTeam();

        Battle battle(player, enemy);
        battle.run();

        cout << "\nQuieres jugar de nuevo? (1 = si, 0 = no): ";
        int again;
        cin >> again;
        playAgain = (again == 1);
    }

    cout << "\nGracias por jugar!\n";
    return 0;
}
