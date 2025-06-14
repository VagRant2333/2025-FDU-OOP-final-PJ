// src/scene/Scene.h
#ifndef SCENE_H
#define SCENE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> // For sf::Music access

// Forward declare Game to avoid circular dependency if Scene needs Game members
class Game;

class Scene
{
public:
    virtual ~Scene() = default;

    // Called when scene is set as current, for loading specific assets
    virtual void loadAssets() = 0;

    // Handle SFML events
    virtual void handleInput(sf::Event &event, sf::RenderWindow &window) = 0;

    // Update scene logic
    virtual void update(sf::Time deltaTime) = 0;

    // Render scene to window
    virtual void render(sf::RenderWindow &window) = 0;

    // Called when global volume changes, so scene can update its sounds
    virtual void onVolumeChanged() = 0;

    // Optional: to get scene's music for global controls (e.g. stopping it)
    virtual sf::Music *getMusic() { return nullptr; }

protected:
    Game &m_game; // Reference to the main game object for changing scenes, accessing global state

    explicit Scene(Game &game) : m_game(game) {}
};

#endif // SCENE_H