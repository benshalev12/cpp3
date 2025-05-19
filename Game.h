#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include "Player.h"
#include "Merchant.h"


class Game {
private:
    std::vector<Player*> players;
    int currentTurnIndex;
    int bank;
    bool gameEnded;

public:
    Game();
    ~Game();

    void addPlayer(Player* player);
    void playTurn();
    std::string turn() const;
    std::vector<std::string> playersList() const;
    std::string winner() const;

    void blockArrestFor(Player& target);
    void cancelCoupOn(Player& victim);
    void cancelBribe(Player& briber);
    void addToBank(int amount);

    int getBank() const;
    Player* getCurrentPlayer();
    void nextTurn();
};
