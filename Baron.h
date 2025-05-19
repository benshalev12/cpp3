#pragma once
#include "Player.h"
class Baron : public Player {
public:
    using Player::Player;
    std::string getRole() const override { return "Baron"; }
    void invest();
    void onSanctioned();
};
