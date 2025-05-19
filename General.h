#pragma once
#include "Player.h"
class General : public Player {
public:
    using Player::Player;
    std::string getRole() const override { return "General"; }
    void blockCoup(Game& game, Player& victim);
    void onArrested();
};
