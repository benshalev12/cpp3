#pragma once
#include "Player.h"
class Spy : public Player {
public:
    using Player::Player;
    std::string getRole() const override { return "Spy"; }
    void spyAction(Game& game, Player& target);
};
