#include "Trainer.h"
#include <iostream>

using namespace std;

bool Trainer::hasUsablePokemon() const {
    for (const auto& p : team) if (!p.isFainted()) return true;
    return false;
}

bool Trainer::switchTo(int index) {
    if (index < 0 || index >= static_cast<int>(team.size())) return false;
    if (team[index].isFainted()) return false;
    if (index == activePokemon) return false;
    activePokemon = index;
    return true;
}

int Trainer::chooseSwitch() const {
    cout << "\nElige un Pokemon:\n";
    for (int i = 0; i < static_cast<int>(team.size()); ++i) {
        const auto& p = team[i];
        cout << "  " << (i + 1) << ". " << p.name;
        if (i == activePokemon)  cout << " (en campo)";
        else if (p.isFainted())  cout << " (debilitado)";
        else                     cout << "  HP:" << p.currentHP << "/" << p.maxHP;
        cout << "\n";
    }
    int choice;
    while (true) {
        cout << "Opcion: ";
        cin >> choice;
        choice--;
        if (choice >= 0 && choice < static_cast<int>(team.size())
            && !team[choice].isFainted() && choice != activePokemon)
            return choice;
        cout << "Seleccion invalida, intenta de nuevo.\n";
    }
}

void Trainer::displayTeam() const {
    cout << "=== Equipo de " << name << " ===\n";
    for (int i = 0; i < static_cast<int>(team.size()); ++i) {
        const auto& p = team[i];
        cout << (i + 1) << ". " << p.name << " " << p.statusString()
                  << "  HP: " << p.currentHP << "/" << p.maxHP;
        if (p.isFainted()) cout << "  [DEBILITADO]";
        cout << "\n";
    }
}

void Trainer::displayActive() const {
    cout << name << " tiene en campo a: ";
    active().displayInfo();
}
