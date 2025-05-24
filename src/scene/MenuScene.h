// src/scene/MenuScene.h
#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "Scene.h"
#include "../core/Game.h" // For GameState and Game reference
#include "../render/ResourceManager.h"
#include "../ui/UIManager.h" // Use the Button struct
#include <SFML/Audio.hpp>
#include <vector>

class MenuScene : public Scene {
public:
    explicit MenuScene(Game& game);
    ~MenuScene() override;

    void loadAssets() override;
    void handleInput(sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void onVolumeChanged() override;
    sf::Music* getMusic() override { return &m_menuMusic; }


private:
    void setupUI();
    void displayScrollContent(int scrollId);

    sf::Sprite m_backgroundSprite;
    sf::Text m_gameTitleText;
    sf::Font m_font; // Font for this scene

    Button m_startButton;
    Button m_exitButton;
    
    // Sidebar UI
    sf::RectangleShape m_sidebarBackground;
    sf::Text m_settingsTitle;
    Button m_volumeUpButton;
    Button m_volumeDownButton;
    sf::Text m_volumeValueText;

    sf::Text m_collectionTitle;
    std::vector<Button> m_scrollButtons; // Buttons to view collected scrolls
    // ASSET_PATH: Example scroll_item.png for each collected scroll icon
    sf::Texture m_scrollIconTexture; 

    sf::Music m_menuMusic;

    // Scroll viewing state
    bool m_isViewingScroll = false;
    sf::Text m_scrollDisplayContentText;
    sf::RectangleShape m_scrollDisplayBackground;
    Button m_closeScrollViewButton;

};

#endif // MENUSCENE_H