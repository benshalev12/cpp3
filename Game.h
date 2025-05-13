#pragma once
#include <vector>
#include <string>
#include "Player.h"
#include "ActionManager.h"

class Game {
private:
    std::vector<Player*> players;
    ActionManager actions;
    size_t turnIndex = 0;
    bool started = false;

public:
    ~Game();
    void addPlayer(const std::string& name);
    void start();
    bool isOver() const;
    Player* currentPlayer();
    const std::vector<Player*>& getPlayers() const;
    void nextTurn();
    std::string getWinner() const;
    ActionManager& getActions();
    Player* getPlayerByName(const std::string& name);
};
