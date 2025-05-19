#include "Spy.h"
#include "Game.h"
#include <iostream>
void Spy::spyAction(Game& game, Player& target) {
    std::cout << target.getName() << " has " << target.getCoins() << " coins.\n";
    game.blockArrestFor(target);
}
