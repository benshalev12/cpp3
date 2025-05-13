
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <ctime>

using namespace sf;
using namespace std;

struct Player {
    string name;
    string role;
    int coins;
    bool alive;

    Player(const string& n, const string& r) : name(n), role(r), coins(2), alive(true) {}
};

vector<string> roles = {"Duke", "Assassin", "Ambassador", "Captain", "Contessa"};

int main() {
    RenderWindow window(VideoMode(800, 600), "Coup Game GUI");
    Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        cout << "Could not load font!" << endl;
        return -1;
    }

    int playerCount = 0;
    vector<string> playerNames;
    vector<Player> players;
    string inputBuffer = "";
    bool collectingNames = false;
    int nameIndex = 0;
    bool gameStarted = false;
    int currentPlayer = 0;
    string statusMessage = "Enter number of players (2-6): ";

    Text status(statusMessage, font, 20);
    status.setPosition(10, 10);

    Text inputText("", font, 20);
    inputText.setPosition(10, 40);

    RectangleShape buttons[3];
    Text labels[3];
    string actions[3] = {"Income", "Foreign Aid", "Coup"};

    for (int i = 0; i < 3; ++i) {
        buttons[i].setSize(Vector2f(150, 40));
        buttons[i].setPosition(600, 100 + i * 60);
        buttons[i].setFillColor(Color(200, 200, 255));

        labels[i].setFont(font);
        labels[i].setString(actions[i]);
        labels[i].setCharacterSize(18);
        labels[i].setPosition(610, 110 + i * 60);
        labels[i].setFillColor(Color::Black);
    }

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (!gameStarted && event.type == Event::TextEntered) {
                if (event.text.unicode == 13) { // Enter
                    if (!collectingNames) {
                        playerCount = stoi(inputBuffer);
                        if (playerCount < 2 || playerCount > 6) {
                            inputBuffer = "";
                            statusMessage = "Invalid number. Enter between 2 and 6:";
                        } else {
                            collectingNames = true;
                            statusMessage = "Enter name for player 1:";
                            inputBuffer = "";
                        }
                    } else {
                        playerNames.push_back(inputBuffer);
                        inputBuffer = "";
                        nameIndex++;
                        if (nameIndex < playerCount) {
                            statusMessage = "Enter name for player " + to_string(nameIndex + 1) + ":";
                        } else {
                            shuffle(roles.begin(), roles.end(), default_random_engine(time(0)));
                            for (int i = 0; i < playerCount; ++i)
                                players.emplace_back(playerNames[i], roles[i % roles.size()]);
                            gameStarted = true;
                            statusMessage = "Game started! Player " + players[0].name + "'s turn.";
                        }
                    }
                } else if (event.text.unicode == 8 && !inputBuffer.empty()) {
                    inputBuffer.pop_back();
                } else if (event.text.unicode < 128) {
                    inputBuffer += static_cast<char>(event.text.unicode);
                }
            } else if (gameStarted && event.type == Event::MouseButtonPressed) {
                Vector2f mouse(Mouse::getPosition(window));
                for (int i = 0; i < 3; ++i) {
                    if (buttons[i].getGlobalBounds().contains(mouse)) {
                        Player& p = players[currentPlayer];
                        if (!p.alive) continue;
                        if (actions[i] == "Income") p.coins += 1;
                        else if (actions[i] == "Foreign Aid") p.coins += 2;
                        else if (actions[i] == "Coup" && p.coins >= 7) {
                            int target = (currentPlayer + 1) % players.size();
                            while (!players[target].alive) target = (target + 1) % players.size();
                            players[target].alive = false;
                            p.coins -= 7;
                        }
                        statusMessage = "Player " + p.name + " played " + actions[i];
                        int aliveCount = 0;
                        int lastAlive = 0;
                        for (int i = 0; i < players.size(); ++i) {
                            if (players[i].alive) {
                                aliveCount++;
                                lastAlive = i;
                            }
                        }
                        if (aliveCount == 1) {
                            statusMessage = "Player " + players[lastAlive].name + " wins!";
                            gameStarted = false;
                        } else {
                            do {
                                currentPlayer = (currentPlayer + 1) % players.size();
                            } while (!players[currentPlayer].alive);
                            statusMessage += " Now it's " + players[currentPlayer].name + "'s turn.";
                        }
                        break;
                    }
                }
            }
        }

        window.clear(Color(80, 120, 180));

        status.setString(statusMessage);
        window.draw(status);

        if (!gameStarted) {
            inputText.setString(inputBuffer);
            window.draw(inputText);
        } else {
            for (int i = 0; i < 3; ++i) {
                window.draw(buttons[i]);
                window.draw(labels[i]);
            }

            for (int i = 0; i < players.size(); ++i) {
                Text t;
                t.setFont(font);
                string text = players[i].name + " (" + players[i].role + "): ";
                text += players[i].alive ? to_string(players[i].coins) + " coins" : "DEAD";
                t.setString(text);
                t.setCharacterSize(20);
                t.setPosition(20, 100 + i * 40);
                t.setFillColor(i == currentPlayer ? Color::Yellow : Color::White);
                window.draw(t);
            }
        }

        window.display();
    }

    return 0;
}
