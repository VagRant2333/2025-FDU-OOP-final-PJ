// src/scene/GameScene.h
#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"
#include "../core/Game.h"
#include "../entities/Player.h"
#include "../physics/PhysicsEngine.h"
#include "../render/ResourceManager.h"
#include <SFML/Audio.hpp>
#include <vector>
#include <random>

struct Laser
{
    // sf::RectangleShape shape;
    sf::Sprite sprite;
    sf::Vector2f velocity;
    bool isActive = true;

    Laser(sf::Texture &texture, sf::Vector2f pos, sf::Vector2f vel, float initialRotation = 0.f)
        : velocity(vel)
    {
        sprite.setTexture(texture);
        sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
        sprite.setPosition(pos);
        // shape.setSize(size);
        // shape.setFillColor(color);
        sprite.setRotation(initialRotation);

        float desiredHeight = 15.f;
        float scale = desiredHeight / sprite.getLocalBounds().height;
        sprite.setScale(scale, scale);
    }

    void update(sf::Time dt)
    {
        sprite.move(velocity * dt.asSeconds());
    }
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
};

struct ScrollItem
{
    sf::Sprite sprite;
    int id;
    bool isActive = true;
    sf::Texture *texturePtr; // avoid copying texture

    ScrollItem(int scrollId, sf::Texture &tex, sf::Vector2f pos) : id(scrollId)
    {
        texturePtr = &tex;
        sprite.setTexture(*texturePtr);
        sprite.setOrigin(tex.getSize().x / 2.f, tex.getSize().y / 2.f);
        sprite.setPosition(pos);
        // sprite.setScale(0.2f, 0.2f);

        float desiredWidth = 48.f;
        float scale = desiredWidth / sprite.getLocalBounds().width;
        sprite.setScale(scale, scale);
    }
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
};

class GameScene : public Scene
{
public:
    explicit GameScene(Game &game);
    ~GameScene() override;

    void loadAssets() override;
    void handleInput(sf::Event &event, sf::RenderWindow &window) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow &window) override;
    void onVolumeChanged() override;
    sf::Music *getMusic() override { return &m_gameMusic; }

private:
    void setupInitialState();
    void updateHUD();
    void spawnLaser();
    void spawnScroll();
    void updateLasers(sf::Time deltaTime);
    void updateScrolls(sf::Time deltaTime);
    void updateBackground(sf::Time deltaTime);
    void updateFieldVisuals();
    void randomizeFields();

    sf::Sprite m_backgroundSprite1;
    sf::Sprite m_backgroundSprite2; // better scroll
    float m_bgScrollSpeed = 100.f;
    sf::Texture m_gameBgTexture;

    // Player m_player;
    std::unique_ptr<Player> m_player;
    PhysicsEngine m_physicsEngine;
    FieldProperties m_currentFields;

    sf::Music m_gameMusic;
    sf::Sound m_laserSound;

    // HUD
    sf::Font m_hudFont;
    sf::Text m_distanceText;
    sf::Text m_chargeText;
    sf::Text m_dashChargesText;
    float m_distanceTraveled = 0.f;

    // Game
    // sf::RectangleShape m_bottomLaser;
    std::vector<Laser> m_lasers;
    std::vector<ScrollItem> m_scrollsInScene;

    // laserspawn
    sf::Time m_laserSpawnTimer;
    sf::Time m_timeBetweenLaserSpawns = sf::seconds(2.f); // random this later
    sf::Time m_scrollSpawnTimer;
    sf::Time m_timeBetweenScrollSpawns = sf::seconds(10.f);
    int m_maxScrollsOnScreen = 1;

    std::vector<sf::Text> m_bFieldSymbols;
    std::vector<sf::VertexArray> m_eFieldLines;

    std::mt19937 m_rng;

    bool m_isGameOver = false;

    // B field
    static constexpr float B_FIELD_DENSITY_SCALE_FACTOR = 10.0f;
    static constexpr int MAX_B_FIELD_SYMBOLS_PER_ROW_COL = 15;

    sf::Texture m_playerTexture;
    sf::Texture m_scrollItemTexture;
    sf::Texture m_laserTexture;
};

#endif // GAMESCENE_H