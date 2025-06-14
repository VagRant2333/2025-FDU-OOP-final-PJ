// src/core/Game.h
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <memory>
#include <vector>
#include "../scene/Scene.h" // Forward declare if Scene.h includes Game.h, but here it's fine

// Forward declarations
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
    GameOver,      // Potentially for showing score/scroll before returning to menu
    ViewingScroll, // When clicking a scroll in the collection
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
    void setMasterVolume(float volume); // Volume 0.0 to 100.0

    GameState getCurrentState() const { return m_currentState; }

    // Scroll management
    void collectScroll(int scrollId);
    bool isScrollCollected(int scrollId) const;
    const std::vector<bool> &getCollectedScrollsStatus() const { return m_collectedScrolls; }
    const std::string &getScrollContent(int scrollId) const;
    int getTotalScrolls() const { return m_totalScrolls; }
    int getCollectedScrollsCount() const;

    void playerDied(float distance);
    void checkWinCondition(float distance);

    // For displaying scroll after death/win
    // int lastCollectedScrollId = -1;

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void loadAssets(); // Central place to load common assets or trigger scene asset loading

    sf::RenderWindow m_window;
    std::unique_ptr<Scene> m_currentScene;
    GameState m_currentState;

    float m_masterVolume = 50.0f; // Default volume 0-100

    // Scroll data
    const int m_totalScrolls = 5; // Example: 5 scrolls in the game
    std::vector<bool> m_collectedScrolls;
    std::vector<std::string> m_scrollContents; // Placeholder for scroll texts
    void prepareNextScrollForDisplay();

    // Game win condition
    const float m_requiredDistanceForWin = 100.0f; // Example distance
    bool m_gameWonMessageDisplayed = false;

    sf::Text m_winText;
    sf::Text m_deathScrollText;
    sf::Font m_font; // For game-over/win messages displayed by Game class itself

    std::vector<int> m_newlyCollectedScrolls;

public: // Public for easy access from scenes, or use getters
    bool m_exitGame = false;
};

#endif // GAME_H