// src/physics/PhysicsEngine.h
#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

class Player;

struct FieldProperties
{
    sf::Vector2f electricField;  // E_x, E_y
    float magneticField_Z = 0.f; // B_z in/out

    FieldProperties() : electricField(0.f, 0.f), magneticField_Z(0.f) {}
};
class PhysicsEngine
{
public:
    PhysicsEngine();

    void setPlayerMass(float mass);

    // update velocity and position
    void updatePlayer(Player &player, sf::Time deltaTime, const FieldProperties &fields, const sf::Vector2u &windowSize);

private:
    float m_playerMass = 1.f;      // mass
    const float MAX_SPEED = 500.f; // max speed
};

#endif // PHYSICSENGINE_H