#include "Judge.h"
#include "Game.h"
void Judge::blockBribe(Game& game, Player& briber) {
    game.cancelBribe(briber);
}
void Judge::onSanctioned(Game& game, Player& attacker) {
    attacker.removeCoins(1);
    game.addToBank(1);
}
