#pragma once
#include "Player.h"
class Judge : public Player {
public:
    using Player::Player;
    std::string getRole() const override { return "Judge"; }
    void blockBribe(Game& game, Player& briber);
    void onSanctioned(Game& game, Player& attacker);
};
