// src/entities/Player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include <SFML/Graphics.hpp>

class Player : public Entity
{
public:
    Player(sf::Vector2f startPosition, sf::Texture &texture);

    void update(sf::Time deltaTime) override; // for internal logic ,physics engine moves it
    void render(sf::RenderWindow &window) override;

    sf::FloatRect getBounds() const override;
    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f &pos) override;

    void setVelocity(const sf::Vector2f &vel);
    sf::Vector2f getVelocity() const;

    // charge controls
    void increaseCharge();
    void decreaseCharge();
    void toggleChargeSign();
    float getCharge() const;
    void setCharge(float charge);

    // skill
    void dash(const sf::Vector2f &direction);
    void resetDashCharges();
    int getDashCharges() const;

private:
    sf::Sprite m_sprite;
    sf::Vector2f m_velocity;
    float m_charge; // electric charge

    static constexpr float MAX_CHARGE = 30.0f;
    static constexpr float MIN_CHARGE_MAGNITUDE = 0.1f; // min magnitude, can be negative
    static constexpr float CHARGE_STEP = 5.f;

    int m_dashCharges;
    static constexpr int MAX_DASH_CHARGES = 10;
    static constexpr float DASH_DISTANCE = 100.f;
};

#endif // PLAYER_H