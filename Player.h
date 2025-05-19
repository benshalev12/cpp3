#pragma once
#include <string>
#include <stdexcept>

class Game;

class Player {
protected:
    std::string name;
    int id;
    int coins;
    bool alive;
    bool economicBlocked;
    int lastArrestedTarget;

public:
    Player(const std::string& name, int id);
    virtual ~Player() = default;

    const std::string& getName() const;
    int getID() const;
    int getCoins() const;
    bool isAlive() const;
    bool isBlocked() const;
    void setEconomicBlocked(bool blocked);
    void setLastArrested(int targetID);
    int getLastArrested() const;

    void addCoins(int amount);
    void removeCoins(int amount);
    void kill();

    virtual std::string getRole() const = 0;

    virtual void gather(Game& game);
    virtual void tax(Game& game);
    virtual void bribe(Game& game);
    virtual void arrest(Game& game, Player& target);
    virtual void sanction(Game& game, Player& target);
    virtual void coup(Game& game, Player& target);
};
