// src/entities/Player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include <SFML/Graphics.hpp>

class Player : public Entity {
public:
    Player(sf::Vector2f startPosition, sf::Texture& texture);

    void update(sf::Time deltaTime) override; // Mostly for internal logic if any, physics engine moves it
    void render(sf::RenderWindow& window) override;

    sf::FloatRect getBounds() const override;
    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& pos) override;

    void setVelocity(const sf::Vector2f& vel);
    sf::Vector2f getVelocity() const;

    // Charge controls
    void increaseCharge();
    void decreaseCharge(); // amount is now internal via CHARGE_STEP
    void toggleChargeSign();
    float getCharge() const;
    void setCharge(float charge);


private:
    sf::Sprite m_sprite;
    sf::Vector2f m_velocity;
    float m_charge; // Player's electric charge (e.g., in Coulombs or arbitrary units)
    
    static constexpr float MAX_CHARGE = 10.0f;
    static constexpr float MIN_CHARGE_MAGNITUDE = 0.1f; // Minimum magnitude, can be negative
    static constexpr float CHARGE_STEP = 0.5f;
};

#endif // PLAYER_H