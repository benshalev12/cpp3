#include "ActionManager.h"
#include <iostream>
#include <stdexcept>

void ActionManager::gather(Player& player) {
    if (player.isBlockedFromEconomy()) throw std::runtime_error("Gather blocked by sanction.");
    player.addCoins(1);
}

void ActionManager::tax(Player& player, const std::vector<Player*>& others) {
    for (const auto& p : others) {
        if (p->canBlockTax() && p->isActive()) {
            player.blockTax();
            throw std::runtime_error("Tax blocked by Governor.");
        }
    }
    if (player.getRole() == "Governor") player.addCoins(3);
    else player.addCoins(2);
}

void ActionManager::bribe(Player& player, const std::vector<Player*>& others) {
    for (const auto& p : others) {
        if (p->canBlockBribe() && p->isActive()) {
            throw std::runtime_error("Bribe blocked by Judge.");
        }
    }
    player.removeCoins(4);
}

void ActionManager::arrest(Player& player, Player& target) {
    if (player.getLastArrestTarget() == target.getName())
        throw std::runtime_error("Cannot arrest same player twice in a row.");
    if (target.isBlockedFromArrest() || target.canBlockArrest())
        throw std::runtime_error("Target is protected from arrest.");

    player.setLastArrestTarget(target.getName());

    if (target.getRole() == "Merchant") target.onArrested();
    else {
        target.removeCoins(1);
        player.addCoins(1);
        target.onArrested();
    }
}

void ActionManager::sanction(Player& player, Player& target) {
    player.removeCoins(3);
    target.blockEconomy();
    target.onSanctioned();
    if (target.getRole() == "Judge") player.removeCoins(1);
}

void ActionManager::coup(Player& player, Player& target) {
    try {
        if (target.canBlockCoup()) {
            target.blockNextCoup();
            return; // Coup blocked
        }
    } catch (...) {
        // Block failed or not enough coins – continue coup
    }

    player.removeCoins(7);
    target.deactivate();
}