#include "Merchant.h"
#include "Game.h"
void Merchant::startTurnBonus() {
    if (coins >= 3) addCoins(1);
}
void Merchant::onArrested(Game& game) {
    removeCoins(2);
    game.addToBank(2);
}
