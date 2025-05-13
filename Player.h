#pragma once
#include <string>

class Player {
protected:
    std::string name;
    int coins;
    bool is_active;
    bool blocked_from_tax = false;
    bool blocked_from_arrest = false;
    bool blocked_from_economy = false;
    std::string last_arrest_target;

public:
    Player(const std::string& name);
    virtual ~Player();

    std::string getName() const;
    int getCoins() const;
    bool isActive() const;

    void deactivate();
    void addCoins(int amount);
    void removeCoins(int amount);

    void blockTax();
    void blockEconomy();
    void resetBlocks();

    bool isBlockedFromTax() const;
    bool isBlockedFromArrest() const;
    bool isBlockedFromEconomy() const;

    void setLastArrestTarget(const std::string& name);
    std::string getLastArrestTarget() const;

    virtual std::string getRole() const = 0;
    virtual void onSanctioned();
    virtual void onArrested();
    virtual void startTurnBonus();

    // Extended virtual interface
    virtual bool canBlockTax() const;
    virtual bool canBlockBribe() const;
    virtual bool canBlockArrest() const;
    virtual bool canBlockCoup() const;
    virtual bool canInvest() const;
    virtual bool canSpy() const;

    virtual void invest();
    virtual void spyOn(Player& other);
    virtual void blockNextCoup();
    virtual void setArrestProtected(bool val);
};
