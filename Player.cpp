#include "Player.h"
#include "Game.h"

Player::Player(const std::string& name, int id)
    : name(name), id(id), coins(0), alive(true), economicBlocked(false), lastArrestedTarget(-1) {}

const std::string& Player::getName() const { return name; }
int Player::getID() const { return id; }
int Player::getCoins() const { return coins; }
bool Player::isAlive() const { return alive; }
bool Player::isBlocked() const { return economicBlocked; }
void Player::setEconomicBlocked(bool blocked) { economicBlocked = blocked; }
void Player::setLastArrested(int targetID) { lastArrestedTarget = targetID; }
int Player::getLastArrested() const { return lastArrestedTarget; }

void Player::addCoins(int amount) { coins += amount; }
void Player::removeCoins(int amount) {
    if (coins < amount) throw std::runtime_error("Not enough coins");
    coins -= amount;
}
void Player::kill() { alive = false; }

void Player::gather(Game& game) {
    if (economicBlocked) throw std::runtime_error("Player is under economic sanction");
    addCoins(1);
}

void Player::tax(Game& game) {
    if (economicBlocked) throw std::runtime_error("Player is under economic sanction");
    addCoins(2);
}

void Player::bribe(Game& game) {
    removeCoins(4);
}

void Player::arrest(Game& game, Player& target) {
    if (target.getID() == lastArrestedTarget)
        throw std::runtime_error("Cannot arrest same player twice in a row");
    target.removeCoins(1);
    addCoins(1);
    lastArrestedTarget = target.getID();
}

void Player::sanction(Game& game, Player& target) {
    removeCoins(3);
    target.setEconomicBlocked(true);
}

void Player::coup(Game& game, Player& target) {
    if (getCoins() < 7) throw std::runtime_error("Not enough coins for coup");
    removeCoins(7);
    target.kill();
}
