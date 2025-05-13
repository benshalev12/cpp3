#pragma once
#include "Player.h"
#include <iostream>

class Governor : public Player {
public:
    using Player::Player;
    std::string getRole() const override { return "Governor"; }
    bool canBlockTax() const override { return true; }
};

class Spy : public Player {
public:
    using Player::Player;
    std::string getRole() const override { return "Spy"; }
    bool canBlockArrest() const override { return true; }
    bool canSpy() const override { return true; }

    void spyOn(Player& other) override {
        std::cout << "Spy sees that " << other.getName() << " has " << other.getCoins() << " coins.\n";
        other.setArrestProtected(true);
    }
};

class Baron : public Player {
public:
    using Player::Player;
    std::string getRole() const override { return "Baron"; }
    void onSanctioned() override { addCoins(1); }
    bool canInvest() const override { return true; }
    void invest() override {
        if (coins < 3) throw std::runtime_error("Not enough coins to invest.");
        removeCoins(3);
        addCoins(6);
    }
};

class General : public Player {
public:
    using Player::Player;
    std::string getRole() const override { return "General"; }
    void onArrested() override { addCoins(1); }
    bool canBlockCoup() const override { return true; }
    void blockNextCoup() override {
        if (coins < 5) throw std::runtime_error("Not enough coins to block coup.");
        removeCoins(5);
        std::cout << "Coup has been blocked by the General!\n";
    }
};

class Judge : public Player {
public:
    using Player::Player;
    std::string getRole() const override { return "Judge"; }
    bool canBlockBribe() const override { return true; }
};

class Merchant : public Player {
public:
    using Player::Player;
    std::string getRole() const override { return "Merchant"; }
    void onArrested() override { removeCoins(2); }
    void startTurnBonus() override { if (coins >= 3) addCoins(1); }
};
