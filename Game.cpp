#include "Game.h"
#include "Roles.h"
#include <stdexcept>
#include <random>
#include <ctime>
#include <algorithm>

Game::~Game() {
    for (Player* p : players) delete p;
}

void Game::addPlayer(const std::string& name) {
    std::vector<Player*> roles = {
        new Governor(name), new Spy(name), new Baron(name),
        new General(name), new Judge(name), new Merchant(name)
    };
    static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::shuffle(roles.begin(), roles.end(), rng);
    players.push_back(roles.front());
    for (size_t i = 1; i < roles.size(); ++i) delete roles[i];
}

void Game::start() {
    if (players.size() < 2) throw std::runtime_error("Need at least 2 players.");
    started = true;
}

bool Game::isOver() const {
    int count = 0;
    for (auto p : players)
        if (p->isActive()) ++count;
    return count <= 1;
}

Player* Game::currentPlayer() {
    while (!players[turnIndex % players.size()]->isActive())
        turnIndex++;
    return players[turnIndex % players.size()];
}

const std::vector<Player*>& Game::getPlayers() const {
    return players;
}

void Game::nextTurn() {
    turnIndex++;
}

std::string Game::getWinner() const {
    for (auto p : players)
        if (p->isActive()) return p->getName();
    throw std::runtime_error("No winner yet.");
}

ActionManager& Game::getActions() {
    return actions;
}

Player* Game::getPlayerByName(const std::string& name) {
    for (Player* p : players) {
        if (p->getName() == name) return p;
    }
    throw std::runtime_error("Player not found.");
}
