// src/physics/PhysicsEngine.cpp
#include "PhysicsEngine.h"
#include "../entities/Player.h" // For Player's properties like charge, velocity
#include <cmath>                // For std::sqrt, std::min/max if capping speed
#include <iostream>             // For debugging

PhysicsEngine::PhysicsEngine() {}

void PhysicsEngine::setPlayerMass(float mass)
{
    if (mass > 0)
    {
        m_playerMass = mass;
    }
}

void PhysicsEngine::updatePlayer(Player &player, sf::Time dt, const FieldProperties &fields, const sf::Vector2u &windowSize)
{
    // std::cout << "update player./ PhysicsEngine.cpp" << std::endl;
    if (m_playerMass <= 0)
        return; // Invalid mass

    float charge = player.getCharge();
    sf::Vector2f velocity = player.getVelocity();
    sf::Vector2f position = player.getPosition();

    // Calculate forces
    // Electric Force: F_e = qE
    // sf::Vector2f forceElectric = charge * fields.electricField;
    sf::Vector2f forceElectric(charge * fields.electricField.x, charge * fields.electricField.y);

    // Magnetic Force: F_m = q(v x B)
    // v = (vx, vy, 0), B = (0, 0, Bz)
    // v x B = (vy*Bz - 0, 0 - vx*Bz, 0) = (vy*Bz, -vx*Bz, 0)
    sf::Vector2f forceMagnetic = {
        charge * velocity.y * fields.magneticField_Z,
        charge * -velocity.x * fields.magneticField_Z};

    sf::Vector2f totalForce = forceElectric + forceMagnetic;

    // Apply a damping/friction force proportional to velocity to prevent infinite speed gain
    // This is a simple way to make controls feel a bit more stable.
    // You can adjust DAMPING_FACTOR or remove if not desired.
    const float DAMPING_FACTOR = 0.1f;
    // sf::Vector2f dampingForce = -velocity * DAMPING_FACTOR * m_playerMass; // Mass-dependent damping
    sf::Vector2f dampingForce(-velocity.x * DAMPING_FACTOR * m_playerMass, -velocity.y * DAMPING_FACTOR * m_playerMass);
    totalForce += dampingForce;

    // Acceleration: a = F/m
    // sf::Vector2f acceleration = totalForce / m_playerMass;
    sf::Vector2f acceleration(totalForce.x / m_playerMass, totalForce.y / m_playerMass);

    // Update velocity: v = v0 + a*t
    // sf::Vector2f newVelocity = velocity + acceleration * dt.asSeconds();
    sf::Vector2f newVelocity(velocity.x + acceleration.x * dt.asSeconds(),
                             velocity.y + acceleration.y * dt.asSeconds());

    // Speed cap
    float currentSpeed = std::sqrt(newVelocity.x * newVelocity.x + newVelocity.y * newVelocity.y);
    if (currentSpeed > MAX_SPEED)
    {
        newVelocity = (newVelocity / currentSpeed) * MAX_SPEED;
    }

    player.setVelocity(newVelocity);

    // Update position: p = p0 + v*t (using new velocity for more stability)
    player.setPosition(position + newVelocity * dt.asSeconds());

    // Wall collision and bounce (simple implementation)
    sf::FloatRect playerBounds = player.getBounds(); // Assuming Player has getBounds()
    float restitution = 0.6f;                        // Bounciness factor (0 to 1)

    // get current pos for bounce
    sf::Vector2f currentPos = player.getPosition();
    sf::Vector2f currentVel = player.getVelocity();

    // Left wall
    // if (player.getPosition().x - playerBounds.width / 2.f < 0) {
    //     // player.setPosition({playerBounds.width / 2.f, player.getPosition().y});
    //     // player.setVelocity({-newVelocity.x * restitution, newVelocity.y});
    //     player.setPosition(sf::Vector2f(playerBounds.width / 2.f, player.getPosition().y)); // Use constructor
    //     player.setVelocity(sf::Vector2f(-newVelocity.x * restitution, newVelocity.y));    // Use constructor
    // }
    if (currentPos.x - playerBounds.width / 2.f < 0)
    {
        player.setPosition(sf::Vector2f(playerBounds.width / 2.0f, currentPos.y));
        player.setVelocity(sf::Vector2f(-currentVel.x * restitution, currentVel.y));
    }

    // Right wall
    // if (player.getPosition().x + playerBounds.width / 2.f > windowSize.x) {
    //     // player.setPosition({windowSize.x - playerBounds.width / 2.f, player.getPosition().y});
    //     // player.setVelocity({-newVelocity.x * restitution, newVelocity.y});
    //     player.setPosition(sf::Vector2f(windowSize.x - playerBounds.width / 2.f, player.getPosition().y)); // Use constructor
    //     player.setVelocity(sf::Vector2f(-newVelocity.x * restitution, newVelocity.y)); // Use constructor
    // }
    else if (currentPos.x + playerBounds.width / 2.f > windowSize.x)
    {
        player.setPosition(sf::Vector2f(windowSize.x - playerBounds.width / 2.f, currentPos.y));
        player.setVelocity(sf::Vector2f(-currentVel.x * restitution, currentVel.y));
    }

    // Top wall
    if (currentPos.y - playerBounds.height / 2.f < 0)
    {
        player.setPosition(sf::Vector2f(currentPos.x, playerBounds.height / 2.f));
        player.setVelocity(sf::Vector2f(currentVel.x, -currentVel.y * restitution));
    }
    // Bottom wall
    else if (currentPos.y + playerBounds.height / 2.f > windowSize.y)
    {
        player.setPosition(sf::Vector2f(currentPos.x, windowSize.y - playerBounds.height / 2.f));
        player.setVelocity(sf::Vector2f(currentVel.x, -currentVel.y * restitution));
        // Optional: If hitting bottom wall hard could still mean "death" or penalty
        // For now, just a bounce.
    }

    // std::cout << "reseting speed./ PhysicsEngine.cpp" << std::endl;

    // Top/Bottom walls (optional, for now player can go off Y screen but bottom laser kills)
    // if (player.getPosition().y - playerBounds.height / 2.f < 0) {
    //     player.setPosition({player.getPosition().x, playerBounds.height / 2.f});
    //     player.setVelocity({newVelocity.x, -newVelocity.y * restitution});
    // }
    // No bottom wall bounce, as there's a death laser
}