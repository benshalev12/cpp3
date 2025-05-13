#include "Player.h"
#include <stdexcept>

Player::Player(const std::string& name) : name(name), coins(0), is_active(true) {}
Player::~Player() = default;

std::string Player::getName() const { return name; }
int Player::getCoins() const { return coins; }
bool Player::isActive() const { return is_active; }

void Player::deactivate() { is_active = false; }

void Player::addCoins(int amount) { coins += amount; }
void Player::removeCoins(int amount) {
    if (coins < amount) throw std::runtime_error("Not enough coins.");
    coins -= amount;
}

void Player::blockTax() { blocked_from_tax = true; }
void Player::blockEconomy() { blocked_from_economy = true; }
void Player::resetBlocks() {
    blocked_from_tax = false;
    blocked_from_arrest = false;
    blocked_from_economy = false;
}

bool Player::isBlockedFromTax() const { return blocked_from_tax; }
bool Player::isBlockedFromArrest() const { return blocked_from_arrest; }
bool Player::isBlockedFromEconomy() const { return blocked_from_economy; }

void Player::setLastArrestTarget(const std::string& name) { last_arrest_target = name; }
std::string Player::getLastArrestTarget() const { return last_arrest_target; }

void Player::onSanctioned() {}
void Player::onArrested() {}
void Player::startTurnBonus() {}

bool Player::canBlockTax() const { return false; }
bool Player::canBlockBribe() const { return false; }
bool Player::canBlockArrest() const { return false; }
bool Player::canBlockCoup() const { return false; }
bool Player::canInvest() const { return false; }
bool Player::canSpy() const { return false; }

void Player::invest() {}
void Player::spyOn(Player& other) { (void)other; }
void Player::blockNextCoup() {}
void Player::setArrestProtected(bool val) { blocked_from_arrest = val; }
