#pragma once
#include "Player.h"
#include <vector>

class ActionManager {
public:
    void gather(Player& player);
    void tax(Player& player, const std::vector<Player*>& others);
    void bribe(Player& player, const std::vector<Player*>& others);
    void arrest(Player& player, Player& target);
    void sanction(Player& player, Player& target);
    void coup(Player& player, Player& target);
};
