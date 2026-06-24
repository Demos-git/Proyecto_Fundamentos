#pragma once
#include "Trainer.h"
#include "TypeChart.h"

using namespace std;

enum class BattleResult { PLAYER_WIN, ENEMY_WIN, FLED };
enum class Action { ATTACK, SWITCH, FLEE };

struct TurnAction {
    Action action;
    int    choice;
};

class Battle {
public:
    Battle(Trainer& player, Trainer& enemy);
    BattleResult run();

private:
    Trainer& player;
    Trainer& enemy;
    int      turnNumber;

    TurnAction getPlayerAction();
    TurnAction getEnemyAction();

    void executeTurn(TurnAction playerAct, TurnAction enemyAct);
    void executeAttack(Trainer& attacker, Trainer& defender, int moveIndex);
    void executeSwitch(Trainer& trainer, int newIndex);

    int  calculateDamage(const Pokemon& atk, const Pokemon& def, const Move& move);
    bool hitCheck(int accuracy);

    void applyEndOfTurn(Trainer& trainer);
    void applyStatusDamage(Pokemon& poke);
    bool isParalyzed(Pokemon& poke);

    bool checkFainted(Trainer& trainer);
    bool isBattleOver();

    void displayBattleState() const;
    void displayHP(const Pokemon& p) const;
    void printSeparator() const;
};
