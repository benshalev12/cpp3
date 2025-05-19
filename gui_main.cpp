#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <string>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Coup - Player Setup");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Could not load font. Make sure arial.ttf is in the executable directory.\n";
        return 1;
    }

    int maxPlayers = 6;
    int minPlayers = 2;
    int playerCount = 0;
    std::vector<std::string> playerNames;
    std::string inputText = "";
    std::map<std::string, int> nameCount;

    sf::Text title("Enter number of players (2-6):", font, 30);
    title.setPosition(50, 30);
    sf::Text input("", font, 30);
    input.setPosition(50, 80);
    sf::Text errorText("", font, 24);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(50, 500);
    sf::Text prompt("", font, 30);
    prompt.setPosition(50, 150);

    bool enterCount = true;
    bool gameReady = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!gameReady && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b') {
                    if (!inputText.empty()) inputText.pop_back();
                } else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
                    if (enterCount) {
                        try {
                            playerCount = std::stoi(inputText);
                            if (playerCount < minPlayers || playerCount > maxPlayers) {
                                errorText.setString("Player count must be between 2 and 6.");
                            } else {
                                enterCount = false;
                                prompt.setString("Enter name for player 1:");
                                inputText = "";
                                errorText.setString("");
                            }
                        } catch (...) {
                            errorText.setString("Invalid number.");
                        }
                    } else {
                        std::string name = inputText;
                        if (nameCount.count(name)) {
                            nameCount[name]++;
                            name += std::to_string(nameCount[name]);
                        } else {
                            nameCount[name] = 0;
                        }
                        playerNames.push_back(name);
                        inputText = "";
                        if ((int)playerNames.size() == playerCount) {
                            gameReady = true;
                            errorText.setFillColor(sf::Color::Green);
                            errorText.setString("Game is ready! Launch game logic here.");
                        } else {
                            prompt.setString("Enter name for player " + std::to_string(playerNames.size() + 1) + ":");
                        }
                    }
                } else if (event.text.unicode < 128) {
                    inputText += static_cast<char>(event.text.unicode);
                }
            }
        }

        input.setString(inputText);

        window.clear(sf::Color(30, 30, 30));
        window.draw(title);
        window.draw(input);
        window.draw(prompt);
        window.draw(errorText);
        window.display();
    }

    // Output final names to console for now
    if (gameReady) {
        std::cout << "Players entered:\n";
        for (const auto& name : playerNames) {
            std::cout << name << std::endl;
        }
    }

    return 0;
}
