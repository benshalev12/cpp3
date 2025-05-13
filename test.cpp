#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Game.h"
#include "Player.h"
#include "Roles.h"

TEST_CASE("Basic Actions & Turn Order") {
    Game game;
    game.addPlayer("Alice");
    game.addPlayer("Bob");
    game.addPlayer("Caro");

    CHECK(game.currentPlayer()->getName() == "Alice");
    game.currentPlayer()->addCoins(1);
    CHECK(game.currentPlayer()->getCoins() == 1);
    game.nextTurn();

    CHECK(game.currentPlayer()->getName() == "Bob");
    game.currentPlayer()->addCoins(1);
    CHECK(game.currentPlayer()->getCoins() == 1);
    game.nextTurn();

    CHECK(game.currentPlayer()->getName() == "Caro");
    game.currentPlayer()->addCoins(1);
    CHECK(game.currentPlayer()->getCoins() == 1);
    game.nextTurn();

    CHECK(game.currentPlayer()->getName() == "Alice");
}

TEST_CASE("Coin Accumulation and Removal") {
    Game game;
    game.addPlayer("Player1");
    Player* p = game.getPlayerByName("Player1");

    p->addCoins(5);
    CHECK(p->getCoins() == 5);
    p->removeCoins(2);
    CHECK(p->getCoins() == 3);
}

TEST_CASE("Block Flags Set and Reset") {
    Game game;
    game.addPlayer("P1");
    Player* p = game.getPlayerByName("P1");

    p->blockTax();
    p->blockEconomy();
    CHECK(p->isBlockedFromTax());
    CHECK(p->isBlockedFromEconomy());

    p->resetBlocks();
    CHECK_FALSE(p->isBlockedFromTax());
    CHECK_FALSE(p->isBlockedFromEconomy());
}

TEST_CASE("Deactivate Player") {
    Game game;
    game.addPlayer("Inactive");
    Player* p = game.getPlayerByName("Inactive");

    CHECK(p->isActive());
    p->deactivate();
    CHECK_FALSE(p->isActive());
}

TEST_CASE("Arrest Target Memory") {
    Game game;
    game.addPlayer("Spy1");
    Player* p = game.getPlayerByName("Spy1");

    p->setLastArrestTarget("Eitan");
    CHECK(p->getLastArrestTarget() == "Eitan");
    p->setLastArrestTarget("Nevo");
    CHECK(p->getLastArrestTarget() == "Nevo");
}

TEST_CASE("Game Turn Rotation") {
    Game game;
    game.addPlayer("A");
    game.addPlayer("B");
    game.addPlayer("C");
    game.addPlayer("D");

    std::vector<std::string> expected = {"A", "B", "C", "D", "A", "B"};
    for (const std::string& name : expected) {
        CHECK(game.currentPlayer()->getName() == name);
        game.nextTurn();
    }
}

TEST_CASE("Get Players List") {
    Game game;
    game.addPlayer("P1");
    game.addPlayer("P2");
    game.addPlayer("P3");

    const auto& players = game.getPlayers();
    CHECK(players.size() == 3);
    CHECK(players[0]->getName() == "P1");
    CHECK(players[1]->getName() == "P2");
    CHECK(players[2]->getName() == "P3");
}

TEST_CASE("Winner Declaration") {
    Game game;
    game.addPlayer("One");
    game.addPlayer("Two");
    Player* p1 = game.getPlayerByName("One");
    Player* p2 = game.getPlayerByName("Two");

    p2->deactivate();
    CHECK(game.getWinner() == "One");
}

TEST_CASE("Multiple Blocks in Sequence") {
    Game game;
    game.addPlayer("Blocker");
    Player* p = game.getPlayerByName("Blocker");

    p->blockTax();
    p->blockEconomy();
    p->blockTax();

    CHECK(p->isBlockedFromTax());
    CHECK(p->isBlockedFromEconomy());
    p->resetBlocks();
    CHECK_FALSE(p->isBlockedFromTax());
    CHECK_FALSE(p->isBlockedFromEconomy());
}