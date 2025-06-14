// src/entities/Player.cpp
#include "Player.h"
#include <iostream>
#include <cmath>

Player::Player(sf::Vector2f startPosition, sf::Texture &texture)
    : m_velocity(0.f, 0.f), m_charge(1.0f), m_dashCharges(MAX_DASH_CHARGES)
{
    m_sprite.setTexture(texture);
    // make sure texture is loaded before getting its size
    if (texture.getSize().x > 0 && texture.getSize().y > 0)
    {
        m_sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
        float desiredWidth = 64.f;
        float scale = desiredWidth / m_sprite.getLocalBounds().width;
        m_sprite.setScale(scale, scale);
    }
    else
    {
        // if texture is not valid
        std::cerr << "Warning: Player texture invalid at construction." << std::endl;
        m_sprite.setOrigin(0.f, 0.f);
    }
    m_sprite.setPosition(startPosition);
    // std::cout << "player setTexture./ Player.cpp" << std::endl;
}

void Player::update(sf::Time deltaTime)
{
    // position is updated by PhysicsEngine
    // for animation
}

void Player::render(sf::RenderWindow &window)
{
    window.draw(m_sprite);
    // std::cout << "player rendering./ Player.cpp" << std::endl;
}

sf::FloatRect Player::getBounds() const
{
    return m_sprite.getGlobalBounds();
}

sf::Vector2f Player::getPosition() const
{
    return m_sprite.getPosition();
}

void Player::setPosition(const sf::Vector2f &pos)
{
    m_sprite.setPosition(pos);
    // std::cout << "player set pos/ Player.cpp" << std::endl;
}

void Player::setVelocity(const sf::Vector2f &vel)
{
    m_velocity = vel;
    // std::cout << "player set vel./ Player.cpp" << std::endl;
}

sf::Vector2f Player::getVelocity() const
{
    return m_velocity;
}


void Player::increaseCharge()
{
    float currentSign = (m_charge == 0.f) ? 1.f : std::copysign(1.0f, m_charge);
    float currentMagnitude = std::abs(m_charge);

    currentMagnitude += CHARGE_STEP;
    if (currentMagnitude > MAX_CHARGE)
    {
        currentMagnitude = MAX_CHARGE;
    }
    m_charge = currentSign * currentMagnitude;
    if (std::abs(m_charge) < MIN_CHARGE_MAGNITUDE && m_charge != 0.f)
    {
        m_charge = std::copysign(MIN_CHARGE_MAGNITUDE, m_charge);
    }
    std::cout << "Charge increased to: " << m_charge << std::endl;
}

// void Player::decreaseCharge(float amount) {
//     m_charge = std::max(m_charge - amount, -MAX_CHARGE);
//      if (std::abs(m_charge) < MIN_CHARGE && m_charge != 0.f) {
//         m_charge = m_charge > 0 ? MIN_CHARGE : -MIN_CHARGE;
//     } else if (m_charge > -MIN_CHARGE && m_charge < 0) {
//          m_charge = -MIN_CHARGE;
//      }

//     std::cout << "Charge decreased to: " << m_charge << std::endl;
// }
void Player::decreaseCharge()
{
    float currentSign = (m_charge == 0.f) ? 1.f : std::copysign(1.0f, m_charge);
    float currentMagnitude = std::abs(m_charge);

    currentMagnitude -= CHARGE_STEP;
    if (currentMagnitude < MIN_CHARGE_MAGNITUDE)
    {
        // allow to go to zero
        if (std::abs(m_charge) > 0.0f)
        { // if it wasn't already zero
            currentMagnitude = MIN_CHARGE_MAGNITUDE;
        }
        else
        {
            currentMagnitude = 0.0f; // if it was zero, it stays zero
        }
    }
    if (currentMagnitude < 0.0f)
        currentMagnitude = 0.0f; // cannot be less than zero magnitude

    m_charge = currentSign * currentMagnitude;

    setCharge(currentSign * currentMagnitude);
    std::cout << "Charge decreased to: " << m_charge << std::endl;
}

// void Player::toggleChargeSign() {
//     if (m_charge != 0.f) { // Don't toggle sign if charge is zero
//         m_charge *= -1.f;
//     }
//     std::cout << "Charge sign toggled to: " << m_charge << std::endl;
// }
void Player::toggleChargeSign()
{
    if (std::abs(m_charge) >= MIN_CHARGE_MAGNITUDE)
    { // toggle if charge has some magnitude
        m_charge *= -1.f;
    }
    else if (m_charge == 0.f)
    { // if charge is zero, min
        m_charge = MIN_CHARGE_MAGNITUDE;
    }
    std::cout << "Charge sign toggled to: " << m_charge << std::endl;
}

float Player::getCharge() const
{
    return m_charge;
}

void Player::setCharge(float charge)
{
    float sign = (charge == 0.f) ? 1.f : std::copysign(1.0f, charge);
    float magnitude = std::abs(charge);

    if (magnitude > MAX_CHARGE)
    {
        magnitude = MAX_CHARGE;
    }
    // if magnitude is between 0 and min
    // unless set to 0
    if (magnitude < MIN_CHARGE_MAGNITUDE && magnitude != 0.f)
    {
        magnitude = MIN_CHARGE_MAGNITUDE;
    }

    m_charge = sign * magnitude;
}

void Player::dash(const sf::Vector2f &direction)
{
    if (m_dashCharges > 0)
    {
        --m_dashCharges;
        sf::Vector2f normalizeDir = direction;
        float mag = std::sqrt(direction.x * direction.x + direction.y + direction.y);
        if (mag > 0)
        {
            normalizeDir /= mag;
        }
        m_sprite.move(normalizeDir * DASH_DISTANCE);
        std::cout << "Dash!!!" << std::endl;
    }
}

void Player::resetDashCharges()
{
    m_dashCharges = MAX_DASH_CHARGES;
}

int Player::getDashCharges() const
{
    return m_dashCharges;
}