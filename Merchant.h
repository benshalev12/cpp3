#pragma once
#include "Player.h"
class Merchant : public Player {
public:
    using Player::Player;
    std::string getRole() const override { return "Merchant"; }
    void startTurnBonus();
    void onArrested(Game& game);
};
