// src/scene/Scene.h
#ifndef SCENE_H
#define SCENE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Game;

class Scene
{
public:
    virtual ~Scene() = default;

    // when scene set as current
    virtual void loadAssets() = 0;

    // SFML events
    virtual void handleInput(sf::Event &event, sf::RenderWindow &window) = 0;

    // update scene
    virtual void update(sf::Time deltaTime) = 0;

    virtual void render(sf::RenderWindow &window) = 0;

    virtual void onVolumeChanged() = 0;

    virtual sf::Music *getMusic() { return nullptr; }

protected:
    Game &m_game;

    explicit Scene(Game &game) : m_game(game) {}
};

#endif // SCENE_H