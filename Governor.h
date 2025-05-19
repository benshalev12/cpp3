#pragma once
#include "Player.h"
class Governor : public Player {
public:
    using Player::Player;
    std::string getRole() const override { return "Governor"; }
    void tax(Game& game) override;
};
