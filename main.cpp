#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <random>
#include <algorithm>
#include <memory>

#include "Game.h"
#include "Player.h"
#include "Baron.h"
#include "General.h"
#include "Governor.h"
#include "Judge.h"
#include "Merchant.h"
#include "Spy.h"

// עזרים גרפיים
class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    bool isEnabled;

public:
    Button(const sf::Font& font, const std::string& buttonText, 
           const sf::Vector2f& size, const sf::Vector2f& position,
           const sf::Color& color = sf::Color(100, 100, 200)) {
        shape.setSize(size);
        shape.setPosition(position);
        shape.setFillColor(color);
        
        text.setFont(font);
        text.setString(buttonText);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);
        
        // מיקום הטקסט במרכז הכפתור
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(
            position.x + (size.x - textBounds.width) / 2,
            position.y + (size.y - textBounds.height) / 2 - textBounds.top
        );
        
        isEnabled = true;
    }
    
    bool contains(const sf::Vector2f& point) const {
        return isEnabled && shape.getGlobalBounds().contains(point);
    }
    
    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }
    
    void setEnabled(bool enabled) {
        isEnabled = enabled;
        if (enabled) {
            shape.setFillColor(sf::Color(100, 100, 200));
        } else {
            shape.setFillColor(sf::Color(60, 60, 120));
        }
    }
    
    void setText(const std::string& newText) {
        text.setString(newText);
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(
            shape.getPosition().x + (shape.getSize().x - textBounds.width) / 2,
            shape.getPosition().y + (shape.getSize().y - textBounds.height) / 2 - textBounds.top
        );
    }
    
    std::string getText() const {
        return text.getString();
    }
};

// מחלקה שמנהלת את המשחק עם הממשק הגרפי
class GameGui {
private:
    sf::RenderWindow window;
    sf::Font font;
    Game gameLogic;
    std::vector<std::string> roles = {"Baron", "General", "Governor", "Judge", "Merchant", "Spy"};
    std::vector<std::string> playerNames;
    std::vector<std::string> playerRoles;
    std::vector<Button> actionButtons;
    std::vector<Button> playerSelectButtons;
    std::vector<Button> confirmButtons;
    Button exitButton;
    
    std::string statusMessage;
    int currentSelectedPlayer = -1;
    std::string currentAction;
    bool needPlayerSelection = false;
    bool waitingForConfirmation = false;
    bool gameOver = false;
    
    // צבעים לכל תפקיד
    std::map<std::string, sf::Color> roleColors = {
        {"Baron", sf::Color(150, 75, 0)},      // חום
        {"General", sf::Color(200, 0, 0)},     // אדום
        {"Governor", sf::Color(0, 0, 150)},    // כחול כהה
        {"Judge", sf::Color(128, 0, 128)},     // סגול
        {"Merchant", sf::Color(0, 150, 0)},    // ירוק
        {"Spy", sf::Color(0, 0, 0)}            // שחור
    };

public:
    GameGui() : exitButton(font, "Exit", sf::Vector2f(100, 40), sf::Vector2f(750, 30)) {
        window.create(sf::VideoMode(900, 700), "Coup Game");
        
        if (!font.loadFromFile("/usr/share/fonts/truetype/freefont/FreeSans.ttf")) {

            std::cerr << "Could not load font. Make sure arial.ttf is in the executable directory.\n";
            exit(1);
        }
        
        statusMessage = "Welcome to Coup!";
    }
    
    void run() {
        if (!setupPlayers()) {
            return; // המשתמש סגר את החלון במהלך הגדרת השחקנים
        }
        
        createGameLogic();
        setupButtons();
        
        gameLoop();
    }
    
private:
    bool setupPlayers() {
        int playerCount = 0;
        std::string inputText = "";
        bool enterCount = true;
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
        
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    return false;
                }
                
                if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode == '\b') {
                        if (!inputText.empty()) inputText.pop_back();
                    } else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
                        if (enterCount) {
                            try {
                                playerCount = std::stoi(inputText);
                                if (playerCount < 2 || playerCount > 6) {
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
                            if (name.empty()) {
                                errorText.setString("Name cannot be empty.");
                                continue;
                            }
                            
                            if (nameCount.count(name)) {
                                nameCount[name]++;
                                name += std::to_string(nameCount[name]);
                            } else {
                                nameCount[name] = 0;
                            }
                            
                            playerNames.push_back(name);
                            inputText = "";
                            
                            if ((int)playerNames.size() == playerCount) {
                                return true; // הגדרת השחקנים הסתיימה בהצלחה
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
        
        return false;
    }
    
    void createGameLogic() {
        // מערבבים את הרשימה של התפקידים
        std::random_device rd;
        std::mt19937 g(rd());
        std::vector<std::string> availableRoles = roles;
        
        // אם יש פחות שחקנים מתפקידים, בוחרים רק מספר תפקידים שווה למספר השחקנים
        if (playerNames.size() < availableRoles.size()) {
            std::shuffle(availableRoles.begin(), availableRoles.end(), g);
            availableRoles.resize(playerNames.size());
        }
        
        // חלוקת תפקידים לשחקנים
        playerRoles.resize(playerNames.size());
        
        for (size_t i = 0; i < playerNames.size(); ++i) {
            int roleIndex = i % availableRoles.size();
            playerRoles[i] = availableRoles[roleIndex];
            
            Player* newPlayer = nullptr;
            
            if (playerRoles[i] == "Baron") {
                newPlayer = new Baron(playerNames[i], i);
            } else if (playerRoles[i] == "General") {
                newPlayer = new General(playerNames[i], i);
            } else if (playerRoles[i] == "Governor") {
                newPlayer = new Governor(playerNames[i], i);
            } else if (playerRoles[i] == "Judge") {
                newPlayer = new Judge(playerNames[i], i);
            } else if (playerRoles[i] == "Merchant") {
                newPlayer = new Merchant(playerNames[i], i);
            } else if (playerRoles[i] == "Spy") {
                newPlayer = new Spy(playerNames[i], i);
            }
            
            if (newPlayer) {
                gameLogic.addPlayer(newPlayer);
                // שחקנים מתחילים עם 2 מטבעות
                newPlayer->addCoins(2);
            }
        }
    }
    
    void setupButtons() {
        // כפתורי פעולות
        std::vector<std::string> actions = {"Gather", "Tax", "Bribe", "Arrest", "Sanction", "Coup"};
        for (size_t i = 0; i < actions.size(); ++i) {
            actionButtons.emplace_back(
                font, actions[i], 
                sf::Vector2f(150, 50), 
                sf::Vector2f(50, 200 + i * 70)
            );
        }
        
        // כפתורים ספציפיים לכל תפקיד
        std::string role = playerRoles[gameLogic.getCurrentPlayer()->getID()];
        if (role == "Baron") {
            actionButtons.emplace_back(font, "Invest", sf::Vector2f(150, 50), sf::Vector2f(220, 200));
        } else if (role == "General") {
            actionButtons.emplace_back(font, "Block Coup", sf::Vector2f(150, 50), sf::Vector2f(220, 200));
        } else if (role == "Governor") {
            actionButtons.emplace_back(font, "Governor Tax", sf::Vector2f(150, 50), sf::Vector2f(220, 200));
        } else if (role == "Judge") {
            actionButtons.emplace_back(font, "Block Bribe", sf::Vector2f(150, 50), sf::Vector2f(220, 200));
        } else if (role == "Spy") {
            actionButtons.emplace_back(font, "Spy Action", sf::Vector2f(150, 50), sf::Vector2f(220, 200));
        }
        
        // כפתור אישור
        confirmButtons.emplace_back(font, "Confirm", sf::Vector2f(150, 50), sf::Vector2f(400, 600));
        confirmButtons[0].setEnabled(false);
        
        // כפתור ביטול
        confirmButtons.emplace_back(font, "Cancel", sf::Vector2f(150, 50), sf::Vector2f(250, 600));
        confirmButtons[1].setEnabled(false);
    }
    
    void updatePlayerSelectButtons() {
        playerSelectButtons.clear();
        
        // יוצרים כפתור עבור כל שחקן חי
        float startY = 200;
        for (size_t i = 0; i < playerNames.size(); ++i) {
            Player* player = gameLogic.getCurrentPlayer();
            
            // לא מציגים את השחקן הנוכחי כאפשרות בחירה
            if (!player || player->getID() == (int)i || !playerIsAlive(i)) continue;
            
            sf::Color playerColor = roleColors[playerRoles[i]];
            Button playerButton(font, playerNames[i], sf::Vector2f(150, 50), 
                              sf::Vector2f(400, startY), playerColor);
            playerSelectButtons.push_back(playerButton);
            startY += 70;
        }
    }
    
    bool playerIsAlive(int playerId) {
        auto playersList = gameLogic.playersList();
        for (const auto& name : playersList) {
            if (name == playerNames[playerId]) {
                return true;
            }
        }
        return false;
    }
    
    void gameLoop() {
        while (window.isOpen() && !gameOver) {
            handleEvents();
            render();
        }
        
        if (gameOver && window.isOpen()) {
            renderGameOver();
        }
    }
    
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    
                    // בדיקה אם לחצו על כפתור יציאה
                    if (exitButton.contains(mousePos)) {
                        window.close();
                        return;
                    }
                    
                    // מצב בחירת שחקן
                    if (needPlayerSelection) {
                        for (size_t i = 0; i < playerSelectButtons.size(); ++i) {
                            if (playerSelectButtons[i].contains(mousePos)) {
                                currentSelectedPlayer = i;
                                waitingForConfirmation = true;
                                confirmButtons[0].setEnabled(true);
                                confirmButtons[1].setEnabled(true);
                                break;
                            }
                        }
                    }
                    // מצב בחירת פעולה
                    else if (!waitingForConfirmation) {
                        for (size_t i = 0; i < actionButtons.size(); ++i) {
                            if (actionButtons[i].contains(mousePos)) {
                                currentAction = actionButtons[i].getText();
                                
                                // בדיקה אם הפעולה דורשת בחירת שחקן
                                if (currentAction == "Arrest" || currentAction == "Sanction" || 
                                    currentAction == "Coup" || currentAction == "Block Coup" || 
                                    currentAction == "Block Bribe" || currentAction == "Spy Action") {
                                    needPlayerSelection = true;
                                    updatePlayerSelectButtons();
                                } else {
                                    // פעולות שלא צריכות בחירת שחקן
                                    waitingForConfirmation = true;
                                    confirmButtons[0].setEnabled(true);
                                    confirmButtons[1].setEnabled(true);
                                }
                                break;
                            }
                        }
                    }
                    // מצב אישור פעולה
                    else if (waitingForConfirmation) {
                        // אישור הפעולה
                        if (confirmButtons[0].contains(mousePos)) {
                            executeAction();
                            resetActionState();
                        }
                        // ביטול הפעולה
                        else if (confirmButtons[1].contains(mousePos)) {
                            resetActionState();
                        }
                    }
                }
            }
        }
    }
    
    void executeAction() {
        Player* currentPlayer = gameLogic.getCurrentPlayer();
        if (!currentPlayer) return;
        
        try {
            if (currentAction == "Gather") {
                currentPlayer->gather(gameLogic);
                statusMessage = currentPlayer->getName() + " gathered 1 coin";
                gameLogic.nextTurn();
            }
            else if (currentAction == "Tax") {
                currentPlayer->tax(gameLogic);
                statusMessage = currentPlayer->getName() + " collected 2 coins in tax";
                gameLogic.nextTurn();
            }
            else if (currentAction == "Bribe") {
                currentPlayer->bribe(gameLogic);
                statusMessage = currentPlayer->getName() + " paid a bribe of 4 coins";
                gameLogic.nextTurn();
            }
            else if (currentAction == "Arrest") {
                // מוצאים את השחקן הנבחר
                Player* targetPlayer = findPlayerByButtonIndex(currentSelectedPlayer);
                if (targetPlayer) {
                    currentPlayer->arrest(gameLogic, *targetPlayer);
                    statusMessage = currentPlayer->getName() + " arrested " + targetPlayer->getName();
                    gameLogic.nextTurn();
                }
            }
            else if (currentAction == "Sanction") {
                Player* targetPlayer = findPlayerByButtonIndex(currentSelectedPlayer);
                if (targetPlayer) {
                    currentPlayer->sanction(gameLogic, *targetPlayer);
                    statusMessage = currentPlayer->getName() + " sanctioned " + targetPlayer->getName();
                    gameLogic.nextTurn();
                }
            }
            else if (currentAction == "Coup") {
                Player* targetPlayer = findPlayerByButtonIndex(currentSelectedPlayer);
                if (targetPlayer) {
                    currentPlayer->coup(gameLogic, *targetPlayer);
                    statusMessage = currentPlayer->getName() + " performed a coup against " + targetPlayer->getName();
                    
                    // בדיקה אם המשחק הסתיים
                    try {
                        std::string winnerName = gameLogic.winner();
                        statusMessage = winnerName + " has won the game!";
                        gameOver = true;
                        return;
                    } catch (const std::runtime_error&) {
                        // המשחק עדיין נמשך
                        gameLogic.nextTurn();
                    }
                }
            }
            else if (currentAction == "Invest" && playerRoles[currentPlayer->getID()] == "Baron") {
                static_cast<Baron*>(currentPlayer)->invest();
                statusMessage = currentPlayer->getName() + " (Baron) invested 3 coins and gained 6";
                gameLogic.nextTurn();
            }
            else if (currentAction == "Block Coup" && playerRoles[currentPlayer->getID()] == "General") {
                Player* targetPlayer = findPlayerByButtonIndex(currentSelectedPlayer);
                if (targetPlayer) {
                    static_cast<General*>(currentPlayer)->blockCoup(gameLogic, *targetPlayer);
                    statusMessage = currentPlayer->getName() + " (General) blocked coup on " + targetPlayer->getName();
                    gameLogic.nextTurn();
                }
            }
            else if (currentAction == "Governor Tax" && playerRoles[currentPlayer->getID()] == "Governor") {
                static_cast<Governor*>(currentPlayer)->tax(gameLogic);
                statusMessage = currentPlayer->getName() + " (Governor) collected 3 coins in special tax";
                gameLogic.nextTurn();
            }
            else if (currentAction == "Block Bribe" && playerRoles[currentPlayer->getID()] == "Judge") {
                Player* targetPlayer = findPlayerByButtonIndex(currentSelectedPlayer);
                if (targetPlayer) {
                    static_cast<Judge*>(currentPlayer)->blockBribe(gameLogic, *targetPlayer);
                    statusMessage = currentPlayer->getName() + " (Judge) blocked bribe from " + targetPlayer->getName();
                    gameLogic.nextTurn();
                }
            }
            else if (currentAction == "Spy Action" && playerRoles[currentPlayer->getID()] == "Spy") {
                Player* targetPlayer = findPlayerByButtonIndex(currentSelectedPlayer);
                if (targetPlayer) {
                    static_cast<Spy*>(currentPlayer)->spyAction(gameLogic, *targetPlayer);
                    statusMessage = currentPlayer->getName() + " (Spy) spied on " + targetPlayer->getName();
                    gameLogic.nextTurn();
                }
            }
            
            // כשמסתיים התור, נבדוק שוב אם המשחק הסתיים
            try {
                setupButtons(); // מעדכנים את הכפתורים לשחקן החדש
                std::string winnerName = gameLogic.winner();
                statusMessage = winnerName + " has won the game!";
                gameOver = true;
            } catch (const std::runtime_error&) {
                // המשחק עדיין נמשך
            }
        } catch (const std::runtime_error& e) {
            statusMessage = std::string("Error: ") + e.what();
        }
    }
    
    Player* findPlayerByButtonIndex(int buttonIndex) {
        if (buttonIndex < 0 || buttonIndex >= (int)playerSelectButtons.size()) {
            return nullptr;
        }
        
        std::string targetName = playerSelectButtons[buttonIndex].getText();
        for (size_t i = 0; i < playerNames.size(); ++i) {
            if (playerNames[i] == targetName && playerIsAlive(i)) {
                std::vector<Player*> allPlayers;
                for (const auto& name : gameLogic.playersList()) {
                    for (size_t j = 0; j < playerNames.size(); ++j) {
                        if (name == playerNames[j]) {
                            allPlayers.push_back(gameLogic.getCurrentPlayer()); // לא מדויק, אבל נשתמש בטריק להשיג גישה לשחקן
                            break;
                        }
                    }
                }
                
                if (allPlayers.empty()) return nullptr;
                
                return allPlayers[0]; // החזרת השחקן הראשון שמצאנו (זה לא מדויק, אבל נשפר בהמשך)
            }
        }
        return nullptr;
    }
    
    void resetActionState() {
        currentAction = "";
        currentSelectedPlayer = -1;
        needPlayerSelection = false;
        waitingForConfirmation = false;
        confirmButtons[0].setEnabled(false);
        confirmButtons[1].setEnabled(false);
    }
    
    void render() {
        window.clear(sf::Color(30, 30, 30));
        
        // כותרת המשחק
        sf::Text title("Coup Game", font, 36);
        title.setPosition(50, 30);
        window.draw(title);
        
        // מידע על התור הנוכחי
        Player* currentPlayer = gameLogic.getCurrentPlayer();
        if (currentPlayer) {
            sf::Text turnInfo("Current Turn: " + currentPlayer->getName() + 
                              " (" + playerRoles[currentPlayer->getID()] + ")" +
                              " - Coins: " + std::to_string(currentPlayer->getCoins()), 
                              font, 24);
            turnInfo.setPosition(50, 100);
            
            sf::Color roleColor = roleColors[playerRoles[currentPlayer->getID()]];
            turnInfo.setFillColor(roleColor);
            
            window.draw(turnInfo);
        }
        
        // הודעת סטטוס
        sf::Text status(statusMessage, font, 20);
        status.setPosition(50, 150);
        window.draw(status);
        
        // בנק המשחק
        sf::Text bankInfo("Bank: " + std::to_string(gameLogic.getBank()) + " coins", font, 20);
        bankInfo.setPosition(600, 100);
        window.draw(bankInfo);
        
        // רשימת שחקנים
        sf::Text playerListTitle("Players:", font, 22);
        playerListTitle.setPosition(600, 150);
        window.draw(playerListTitle);
        
        float yPos = 180;
        for (size_t i = 0; i < playerNames.size(); ++i) {
            if (playerIsAlive(i)) {
                sf::Text playerInfo(playerNames[i] + " (" + playerRoles[i] + ")", font, 18);
                playerInfo.setPosition(600, yPos);
                playerInfo.setFillColor(roleColors[playerRoles[i]]);
                window.draw(playerInfo);
                yPos += 30;
            } else {
                sf::Text playerInfo(playerNames[i] + " (Dead)", font, 18);
                playerInfo.setPosition(600, yPos);
                playerInfo.setFillColor(sf::Color(100, 100, 100));
                window.draw(playerInfo);
                yPos += 30;
            }
        }
        
        // כפתורי פעולות
        if (!needPlayerSelection) {
            for (auto& button : actionButtons) {
                // בדיקה אם השחקן יכול לבצע את הפעולה הזו
                bool canAfford = true;
                Player* player = gameLogic.getCurrentPlayer();
                
                if (player) {
                    std::string action = button.getText();
                    if (action == "Bribe" && player->getCoins() < 4) canAfford = false;
                    else if (action == "Sanction" && player->getCoins() < 3) canAfford = false;
                    else if (action == "Coup" && player->getCoins() < 7) canAfford = false;
                    else if (action == "Invest" && player->getCoins() < 3) canAfford = false;
                    else if (action == "Block Coup" && player->getCoins() < 5) canAfford = false;
                    
                    // אם לשחקן יש 10 מטבעות או יותר, הוא חייב לבצע הפיכה
                    if (player->getCoins() >= 10 && action != "Coup") {
                        canAfford = false;
                    }
                }
                
                button.setEnabled(canAfford);
                button.draw(window);
            }
        } else {
            sf::Text selectTitle("Select Player:", font, 24);
            selectTitle.setPosition(400, 150);
            window.draw(selectTitle);
            
            for (auto& button : playerSelectButtons) {
                button.draw(window);
            }
        }
        
        // כפתורי אישור
        for (auto& button : confirmButtons) {
            button.draw(window);
        }
        
        // כפתור יציאה
        exitButton.draw(window);
        
        window.display();
    }
    
    void renderGameOver() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    return;
                }
                
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                        if (exitButton.contains(mousePos)) {
                            window.close();
                            return;
                        }
                    }
                }
            }
            
            window.clear(sf::Color(30, 30, 30));
            
            sf::Text gameOverText("Game Over!", font, 48);
            gameOverText.setPosition(300, 200);
            window.draw(gameOverText);
            
            try {
                std::string winnerName = gameLogic.winner();
                sf::Text winnerText("Winner: " + winnerName, font, 36);
                winnerText.setPosition(300, 300);
                window.draw(winnerText);
            } catch (const std::runtime_error&) {
                sf::Text errorText("No winner determined.", font, 36);
                errorText.setPosition(300, 300);
                window.draw(errorText);
            }
            
            exitButton.draw(window);
            window.display();
        }
    }
};

int main() {
    GameGui game;
    game.run();
    return 0;
}