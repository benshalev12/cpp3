#include "Game.h"
#include <iostream>

Game::Game() : currentTurnIndex(0), bank(50), gameEnded(false) {}

Game::~Game() {
    for (auto p : players) delete p;
}

void Game::addPlayer(Player* player) {
    players.push_back(player);
}

std::string Game::turn() const {
    if (gameEnded) throw std::runtime_error("Game has ended");
    return players[currentTurnIndex]->getName();
}

std::vector<std::string> Game::playersList() const {
    std::vector<std::string> result;
    for (auto p : players)
        if (p->isAlive()) result.push_back(p->getName());
    return result;
}

std::string Game::winner() const {
    int aliveCount = 0;
    std::string lastAlive;
    for (auto p : players) {
        if (p->isAlive()) {
            aliveCount++;
            lastAlive = p->getName();
        }
    }
    if (aliveCount > 1) throw std::runtime_error("Game is still ongoing");
    return lastAlive;
}

void Game::blockArrestFor(Player& target) {
    target.setLastArrested(target.getID()); // חוסם מעצר
}

void Game::cancelCoupOn(Player& victim) {
    victim.kill(); // ניתן לשנות לפי הצורך
}

void Game::cancelBribe(Player& briber) {
    std::cout << "Bribe by " << briber.getName() << " was blocked.\n";
}

void Game::addToBank(int amount) {
    bank += amount;
}

int Game::getBank() const {
    return bank;
}

Player* Game::getCurrentPlayer() {
    return players[currentTurnIndex];
}

void Game::nextTurn() {
    do {
        currentTurnIndex = (currentTurnIndex + 1) % players.size();
    } while (!players[currentTurnIndex]->isAlive());
    players[currentTurnIndex]->setEconomicBlocked(false);
    if (players[currentTurnIndex]->getRole() == "Merchant") {
        static_cast<Merchant*>(players[currentTurnIndex])->startTurnBonus();
    }
    if (players[currentTurnIndex]->getCoins() >= 10) {
        throw std::runtime_error("Must perform coup due to 10+ coins");
    }
}
