// src/core/Game.h
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <memory>
#include <vector>
#include "../scene/Scene.h"

namespace sf
{
    class RenderWindow;
}
class Scene;
class MenuScene;
class GameScene;

enum class GameState
{
    MainMenu,
    Playing,
    GameOver,      // for show score/scroll before returning to menu
    ViewingScroll, // When clicking a scroll
    GameWon
};

class Game
{
public:
    Game(unsigned int width, unsigned int height, const std::string &title);
    ~Game();

    void run();

    sf::RenderWindow &getWindow() { return m_window; }
    void changeScene(GameState newState);

    float getMasterVolume() const { return m_masterVolume; }
    void setMasterVolume(float volume); // volume 0.0 to 100.0

    GameState getCurrentState() const { return m_currentState; }

    // scroll management
    void collectScroll(int scrollId);
    bool isScrollCollected(int scrollId) const;
    const std::vector<bool> &getCollectedScrollsStatus() const { return m_collectedScrolls; }
    const std::string &getScrollContent(int scrollId) const;
    int getTotalScrolls() const { return m_totalScrolls; }
    int getCollectedScrollsCount() const;

    void playerDied(float distance);
    void checkWinCondition(float distance);

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void loadAssets(); // load common assets or trigger scene asset loading

    sf::RenderWindow m_window;
    std::unique_ptr<Scene> m_currentScene;
    GameState m_currentState;

    float m_masterVolume = 50.0f; // default vol

    // scroll data
    const int m_totalScrolls = 5;
    std::vector<bool> m_collectedScrolls;
    std::vector<std::string> m_scrollContents; // holder for scroll texts
    void prepareNextScrollForDisplay();

    // win condition
    const float m_requiredDistanceForWin = 100.0f;
    bool m_gameWonMessageDisplayed = false;

    sf::Text m_winText;
    sf::Text m_deathScrollText;
    sf::Font m_font; // win message displayed by game class

    std::vector<int> m_newlyCollectedScrolls;

public:
    bool m_exitGame = false;
};

#endif // GAME_H