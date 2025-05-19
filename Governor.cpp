#include "Governor.h"
#include "Game.h"
void Governor::tax(Game& game) {
    if (isBlocked()) throw std::runtime_error("Governor under sanction");
    addCoins(3);
}
