#include "Battle.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

Battle::Battle(Trainer& p, Trainer& e) : player(p), enemy(e), turnNumber(0) {
    srand(static_cast<unsigned>(time(nullptr)));
}

BattleResult Battle::run() {
    cout << "\nComienza la batalla entre " << player.name
              << " y " << enemy.name << "!\n\n";

    while (!isBattleOver()) {
        ++turnNumber;
        printSeparator();
        cout << "  TURNO " << turnNumber << "\n";
        printSeparator();
        displayBattleState();

        TurnAction playerAct = getPlayerAction();
        if (playerAct.action == Action::FLEE) {
            cout << "\n" << player.name << " ha huido de la batalla.\n";
            return BattleResult::FLED;
        }
        TurnAction enemyAct = getEnemyAction();
        executeTurn(playerAct, enemyAct);
    }

    if (!player.hasUsablePokemon()) {
        cout << "\n" << player.name << " no tiene mas Pokemon. Has perdido!\n";
        return BattleResult::ENEMY_WIN;
    }
    cout << "\n" << player.name << " ha ganado la batalla!\n";
    return BattleResult::PLAYER_WIN;
}

TurnAction Battle::getPlayerAction() {
    cout << "\nQue hara " << player.active().name << "?\n";
    cout << "  1. Atacar\n  2. Cambiar Pokemon\n  3. Huir\nOpcion: ";
    int opt;
    cin >> opt;

    if (opt == 3) return {Action::FLEE, 0};
    if (opt == 2) return {Action::SWITCH, player.chooseSwitch()};

    const auto& moves = player.active().moves;
    cout << "\nMovimientos:\n";
    for (int i = 0; i < static_cast<int>(moves.size()); ++i) {
        const auto& m = moves[i];
        cout << "  " << (i + 1) << ". " << m.name
                  << "  [" << typeToString(m.type) << "]"
                  << "  Poder:" << m.power << "  PP:" << m.currentPP << "/" << m.pp;
        if (!m.isUsable()) cout << " (sin PP)";
        cout << "\n";
    }
    int moveChoice;
    while (true) {
        cout << "Movimiento: ";
        cin >> moveChoice;
        moveChoice--;
        if (moveChoice >= 0 && moveChoice < static_cast<int>(moves.size())
            && moves[moveChoice].isUsable())
            return {Action::ATTACK, moveChoice};
        cout << "Seleccion invalida.\n";
    }
}

TurnAction Battle::getEnemyAction() {
    const Pokemon& atkPoke = enemy.active();
    const Pokemon& defPoke = player.active();
    int bestMove = 0, bestScore = -1;
    for (int i = 0; i < static_cast<int>(atkPoke.moves.size()); ++i) {
        const auto& m = atkPoke.moves[i];
        if (!m.isUsable()) continue;
        float eff = defPoke.typeMultiplierReceived(m.type);
        int score = static_cast<int>(m.power * eff);
        if (score > bestScore) { bestScore = score; bestMove = i; }
    }
    return {Action::ATTACK, bestMove};
}

void Battle::executeTurn(TurnAction playerAct, TurnAction enemyAct) {
    if (playerAct.action == Action::SWITCH) executeSwitch(player, playerAct.choice);
    if (enemyAct.action == Action::SWITCH)  executeSwitch(enemy, enemyAct.choice);

    bool playerFirst = player.active().base.speed >= enemy.active().base.speed;
    Trainer* first  = playerFirst ? &player : &enemy;
    Trainer* second = playerFirst ? &enemy  : &player;
    TurnAction firstAct  = playerFirst ? playerAct : enemyAct;
    TurnAction secondAct = playerFirst ? enemyAct  : playerAct;

    if (firstAct.action == Action::ATTACK) {
        executeAttack(*first, *second, firstAct.choice);
        if (checkFainted(*second)) return;
    }
    if (secondAct.action == Action::ATTACK) {
        executeAttack(*second, *first, secondAct.choice);
        checkFainted(*first);
    }

    applyEndOfTurn(player);
    applyEndOfTurn(enemy);
}

void Battle::executeSwitch(Trainer& trainer, int newIndex) {
    cout << "\n" << trainer.name << " retira a " << trainer.active().name << ".\n";
    trainer.switchTo(newIndex);
    cout << trainer.name << " envia a " << trainer.active().name << "!\n";
    trainer.active().displayArt();
}

void Battle::executeAttack(Trainer& attacker, Trainer& defender, int moveIndex) {
    Pokemon& atkPoke = attacker.active();
    Pokemon& defPoke = defender.active();
    Move&    move    = atkPoke.moves[moveIndex];

    cout << "\n" << atkPoke.name << " usa " << move.name << "!\n";
    move.use();

    if (isParalyzed(atkPoke)) {
        cout << atkPoke.name << " esta paralizado y no puede moverse.\n";
        return;
    }
    if (!hitCheck(move.accuracy)) {
        cout << "El ataque fallo!\n";
        return;
    }
    if (move.power == 0) {
        cout << "(Movimiento de estado, sin efecto por ahora)\n";
        return;
    }

    int dmg = calculateDamage(atkPoke, defPoke, move);
    float eff = defPoke.typeMultiplierReceived(move.type);

    if (eff == 0.0f) { cout << "No afecta a " << defPoke.name << "...\n"; return; }
    if (eff > 1.5f)      cout << "Es muy eficaz!\n";
    else if (eff < 0.5f) cout << "No es muy eficaz...\n";

    defPoke.takeDamage(dmg);
    cout << defPoke.name << " recibio " << dmg << " de dano. ";
    displayHP(defPoke);
}

int Battle::calculateDamage(const Pokemon& atk, const Pokemon& def, const Move& move) {
    int atkStat = (move.category == MoveCategory::PHYSICAL) ? atk.base.attack : atk.base.spAtk;
    int defStat = (move.category == MoveCategory::PHYSICAL) ? def.base.defense : def.base.spDef;

    float base = ((2.0f * 50 / 5 + 2) * move.power * atkStat / defStat) / 50 + 2;
    float stab = (move.type == atk.type1 || move.type == atk.type2) ? 1.5f : 1.0f;
    float eff  = def.typeMultiplierReceived(move.type);
    float rnd  = (85 + rand() % 16) / 100.0f;

    return static_cast<int>(base * stab * eff * rnd);
}

bool Battle::hitCheck(int accuracy) {
    if (accuracy == 0) return true;
    return (rand() % 100) < accuracy;
}

void Battle::applyEndOfTurn(Trainer& trainer) {
    if (!trainer.hasUsablePokemon()) return;
    applyStatusDamage(trainer.active());
}

void Battle::applyStatusDamage(Pokemon& poke) {
    switch (poke.status) {
        case StatusEffect::BURN:
            poke.takeDamage(poke.maxHP / 16);
            cout << poke.name << " sufre dano por quemadura. ";
            displayHP(poke);
            break;
        case StatusEffect::POISON:
            poke.takeDamage(poke.maxHP / 8);
            cout << poke.name << " sufre dano por envenenamiento. ";
            displayHP(poke);
            break;
        default: break;
    }
}

bool Battle::isParalyzed(Pokemon& poke) {
    if (poke.status != StatusEffect::PARALYSIS) return false;
    return (rand() % 4) == 0;
}

bool Battle::checkFainted(Trainer& trainer) {
    if (!trainer.active().isFainted()) return false;
    cout << "\n" << trainer.active().name << " se ha debilitado!\n";
    if (!trainer.hasUsablePokemon()) return true;
    cout << trainer.name << ", elige tu siguiente Pokemon.\n";
    int sw = trainer.chooseSwitch();
    executeSwitch(trainer, sw);
    return false;
}

bool Battle::isBattleOver() {
    return !player.hasUsablePokemon() || !enemy.hasUsablePokemon();
}

void Battle::displayBattleState() const {
    cout << "Rival  -> "; enemy.displayActive();
    cout << "Tu     -> "; player.displayActive();
    cout << "\n";
}

void Battle::displayHP(const Pokemon& p) const {
    int filled = (p.currentHP * 20) / p.maxHP;
    cout << "HP [";
    for (int i = 0; i < 20; ++i) cout << (i < filled ? "=" : " ");
    cout << "] " << p.currentHP << "/" << p.maxHP << "\n";
}

void Battle::printSeparator() const {
    cout << "------------------------------------------\n";
}
