#include "General.h"
#include "Game.h"
void General::blockCoup(Game& game, Player& victim) {
    removeCoins(5);
    game.cancelCoupOn(victim);
}
void General::onArrested() {
    addCoins(1);
}
