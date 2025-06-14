// src/physics/PhysicsEngine.cpp
#include "PhysicsEngine.h"
#include "../entities/Player.h"
#include <cmath>
#include <iostream>

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
        return; // invalid mass

    float charge = player.getCharge();
    sf::Vector2f velocity = player.getVelocity();
    sf::Vector2f position = player.getPosition();

    // E: F_e = qE
    // sf::Vector2f forceElectric = charge * fields.electricField;
    sf::Vector2f forceElectric(charge * fields.electricField.x, charge * fields.electricField.y);

    // M: F_m = q(v x B)
    sf::Vector2f forceMagnetic = {
        charge * velocity.y * fields.magneticField_Z,
        charge * -velocity.x * fields.magneticField_Z};

    sf::Vector2f totalForce = forceElectric + forceMagnetic;

    const float DAMPING_FACTOR = 0.1f;
    // sf::Vector2f dampingForce = -velocity * DAMPING_FACTOR * m_playerMass; // Mass-dependent damping
    sf::Vector2f dampingForce(-velocity.x * DAMPING_FACTOR * m_playerMass, -velocity.y * DAMPING_FACTOR * m_playerMass);
    totalForce += dampingForce;

    // a = F/m
    sf::Vector2f acceleration(totalForce.x / m_playerMass, totalForce.y / m_playerMass);

    // v = v0 + a*t
    sf::Vector2f newVelocity(velocity.x + acceleration.x * dt.asSeconds(),
                             velocity.y + acceleration.y * dt.asSeconds());

    // speed cap
    float currentSpeed = std::sqrt(newVelocity.x * newVelocity.x + newVelocity.y * newVelocity.y);
    if (currentSpeed > MAX_SPEED)
    {
        newVelocity = (newVelocity / currentSpeed) * MAX_SPEED;
    }

    player.setVelocity(newVelocity);

    // Update pos
    player.setPosition(position + newVelocity * dt.asSeconds());

    // air wall
    sf::FloatRect playerBounds = player.getBounds();
    float restitution = 0.6f; // hit factor

    sf::Vector2f currentPos = player.getPosition();
    sf::Vector2f currentVel = player.getVelocity();

    if (currentPos.x - playerBounds.width / 2.f < 0)
    {
        player.setPosition(sf::Vector2f(playerBounds.width / 2.0f, currentPos.y));
        player.setVelocity(sf::Vector2f(-currentVel.x * restitution, currentVel.y));
    }

    // right wall
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

    // top wall
    if (currentPos.y - playerBounds.height / 2.f < 0)
    {
        player.setPosition(sf::Vector2f(currentPos.x, playerBounds.height / 2.f));
        player.setVelocity(sf::Vector2f(currentVel.x, -currentVel.y * restitution));
    }
    // bottom wall
    else if (currentPos.y + playerBounds.height / 2.f > windowSize.y)
    {
        player.setPosition(sf::Vector2f(currentPos.x, windowSize.y - playerBounds.height / 2.f));
        player.setVelocity(sf::Vector2f(currentVel.x, -currentVel.y * restitution));
    }

    // std::cout << "reseting speed./ PhysicsEngine.cpp" << std::endl;
}