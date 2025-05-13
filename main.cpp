
#include <iostream>
#include <vector>
#include <string>
#include "Game.h"
using namespace std;

int getIntInput(const string& prompt, int minVal = 1, int maxVal = 1000) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x && x >= minVal && x <= maxVal) return x;
        else {
            cout << "Invalid input. Try again.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
}

void playerTurn(Game& game, Player* current, const vector<Player*>& allPlayers) {
    cout << "\n=== " << current->getName() << "'s turn (" << current->getRole() << ") ===\n";
    cout << "Coins: " << current->getCoins() << "\n";

    if (current->getCoins() >= 10) {
        cout << "Forced to perform COUP due to 10+ coins.\n";
        for (Player* p : allPlayers) {
            if (p->isActive() && p != current) {
                game.getActions().coup(*current, *p);
                cout << p->getName() << " was eliminated.\n";
                return;
            }
        }
    }

    auto& act = game.getActions();
    while (true) {
        cout << "\nChoose an action:\n";
        vector<pair<int, string>> options;
        int optNum = 1;
        options.emplace_back(optNum++, "Gather");
        options.emplace_back(optNum++, "Tax");
        options.emplace_back(optNum++, "Bribe");
        options.emplace_back(optNum++, "Arrest");
        options.emplace_back(optNum++, "Sanction");
        options.emplace_back(optNum++, "Coup");
        if (current->canInvest()) options.emplace_back(optNum++, "Invest");
        if (current->canSpy()) options.emplace_back(optNum++, "Spy");
        if (current->canBlockCoup()) options.emplace_back(optNum++, "Block Coup");

        
        options.emplace_back(optNum++, "Exit Game");
        for (auto& opt : options) cout << opt.first << ". " << opt.second << "\n";

        int choice = getIntInput("> ");
        bool matched = false;

        for (auto& opt : options) {

        if (opt.second == "Exit Game") {
            cout << "Exiting game.\n";
            exit(0);
        }

            if (choice == opt.first) {
                matched = true;
                try {
                    if (opt.second == "Gather") act.gather(*current);
                    else if (opt.second == "Tax") act.tax(*current, allPlayers);
                    else if (opt.second == "Bribe") act.bribe(*current, allPlayers);
                    else if (opt.second == "Invest") current->invest();
                    else if (opt.second == "Spy") {
                        vector<Player*> targets;
                        for (Player* p : allPlayers) {
                            if (p->isActive() && p != current) {
                                cout << targets.size() + 1 << ". " << p->getName() << "\n";
                                targets.push_back(p);
                            }
                        }
                        int t = getIntInput("> ", 1, targets.size());
                        current->spyOn(*targets[t - 1]);
                    }
                    else if (opt.second == "Block Coup") {
                        current->blockNextCoup();
                    }
                    else if (opt.second == "Arrest" || opt.second == "Sanction" || opt.second == "Coup") {
                        vector<Player*> targets;
                        for (Player* p : allPlayers) {
                            if (p->isActive() && p != current) {
                                cout << targets.size() + 1 << ". " << p->getName() << "\n";
                                targets.push_back(p);
                            }
                        }
                        int t = getIntInput("> ", 1, targets.size());
                        Player* target = targets[t - 1];
                        if (opt.second == "Arrest") act.arrest(*current, *target);
                        else if (opt.second == "Sanction") act.sanction(*current, *target);
                        else if (opt.second == "Coup") act.coup(*current, *target);
                    }
                } catch (const exception& e) {
                    cout << "Error: " << e.what() << "\n";
                    continue;
                }
                break;
            }
        }

        if (!matched) cout << "Invalid choice. Try again.\n";
        else break;
    }
}

int main() {
    Game game;
    int n = getIntInput("Enter number of players (2–6): ", 2, 6);
    for (int i = 0; i < n; ++i) {
        string name;
        cout << "Enter name for player " << i + 1 << ": ";
        cin >> ws;
        getline(cin, name);
        int duplicateCount = 1;
        string original = name;
        for (const auto& p : game.getPlayers()) {
            if (p->getName() == name) {
                name = original + std::to_string(++duplicateCount);
            }
        }
        name = "Player " + std::to_string(i+1) + " - " + name;
        game.addPlayer(name);
    }

    game.start();

    while (!game.isOver()) {
        Player* current = game.currentPlayer();
        playerTurn(game, current, game.getPlayers());
        game.nextTurn();
    }

    cout << "\nThe winner is: " << game.getWinner() << "\n";
    return 0;
}