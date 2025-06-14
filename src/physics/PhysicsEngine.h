// src/physics/PhysicsEngine.h
#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

class Player; // Forward declaration

struct FieldProperties
{
    sf::Vector2f electricField;  // E_x, E_y
    float magneticField_Z = 0.f; // B_z (into/out of screen)

    FieldProperties() : electricField(0.f, 0.f), magneticField_Z(0.f) {} // Explicit constructor
};

class PhysicsEngine
{
public:
    PhysicsEngine();

    // Player's mass in arbitrary units (e.g., kg)
    // Must be > 0
    void setPlayerMass(float mass);

    // Updates player's velocity and position based on forces
    void updatePlayer(Player &player, sf::Time deltaTime, const FieldProperties &fields, const sf::Vector2u &windowSize);

private:
    float m_playerMass = 1.f;     // Default mass
    const float MAX_SPEED = 500.f; // Max speed for player
};

#endif // PHYSICSENGINE_H