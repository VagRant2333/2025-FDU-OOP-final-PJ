// src/entities/Player.cpp
#include "Player.h"
#include <iostream> // For debugging
#include <cmath> // For copysign


// Player::Player(sf::Vector2f startPosition, sf::Texture& texture)
//     : m_velocity(0.f, 0.f), m_charge(1.0f) { // Default positive charge
//     m_sprite.setTexture(texture);
//     m_sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
//     m_sprite.setPosition(startPosition);
//     // ASSET_PATH: A player texture should be loaded and passed here, e.g. "player.png"
//     // m_sprite.setScale(0.1f, 0.1f); // Example scale
// }

Player::Player(sf::Vector2f startPosition, sf::Texture& texture)
    : m_velocity(0.f, 0.f), m_charge(1.0f) {
    m_sprite.setTexture(texture);
    // Ensure texture is loaded before getting its size
    if (texture.getSize().x > 0 && texture.getSize().y > 0) {
        m_sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    } else {
        // Fallback if texture is not yet valid (should not happen if logic is correct)
        std::cerr << "Warning: Player texture invalid at construction." << std::endl;
        m_sprite.setOrigin(0.f, 0.f);
    }
    m_sprite.setPosition(startPosition);
    std::cout << "player setTexture./ Player.cpp" << std::endl;
}

void Player::update(sf::Time deltaTime) {
    // Player's position is updated by PhysicsEngine
    // This update can be used for animations or other non-physics logic
}

void Player::render(sf::RenderWindow& window) {
    window.draw(m_sprite);
    std::cout << "player rendering./ Player.cpp" << std::endl;
}

sf::FloatRect Player::getBounds() const {
    return m_sprite.getGlobalBounds();
}

sf::Vector2f Player::getPosition() const {
    return m_sprite.getPosition();
}

void Player::setPosition(const sf::Vector2f& pos) {
    m_sprite.setPosition(pos);
    std::cout << "player set pos/ Player.cpp" << std::endl;
}

void Player::setVelocity(const sf::Vector2f& vel) {
    m_velocity = vel;
    std::cout << "player set vel./ Player.cpp" << std::endl;
}

sf::Vector2f Player::getVelocity() const {
    return m_velocity;
}

// void Player::increaseCharge(float amount) {
//     m_charge = std::min(m_charge + amount, MAX_CHARGE);
//     if (std::abs(m_charge) < MIN_CHARGE && m_charge != 0.f) { // If it became too small, set to min magnitude
//         m_charge = m_charge > 0 ? MIN_CHARGE : -MIN_CHARGE;
//     }
//     std::cout << "Charge increased to: " << m_charge << std::endl;
// }

void Player::increaseCharge() {
    float currentSign = (m_charge == 0.f) ? 1.f : std::copysign(1.0f, m_charge);
    float currentMagnitude = std::abs(m_charge);

    currentMagnitude += CHARGE_STEP;
    if (currentMagnitude > MAX_CHARGE) {
        currentMagnitude = MAX_CHARGE;
    }
    m_charge = currentSign * currentMagnitude;
    if (std::abs(m_charge) < MIN_CHARGE_MAGNITUDE && m_charge != 0.f) {
        m_charge = std::copysign(MIN_CHARGE_MAGNITUDE, m_charge);
    }
    std::cout << "Charge increased to: " << m_charge << std::endl;
}

// void Player::decreaseCharge(float amount) {
//     m_charge = std::max(m_charge - amount, -MAX_CHARGE);
//      if (std::abs(m_charge) < MIN_CHARGE && m_charge != 0.f) {
//         m_charge = m_charge > 0 ? MIN_CHARGE : -MIN_CHARGE;
//     } else if (m_charge > -MIN_CHARGE && m_charge < 0) { // Prevent crossing zero unless intended
//          m_charge = -MIN_CHARGE;
//      }

//     std::cout << "Charge decreased to: " << m_charge << std::endl;
// }
void Player::decreaseCharge() {
    float currentSign = (m_charge == 0.f) ? 1.f : std::copysign(1.0f, m_charge);
    float currentMagnitude = std::abs(m_charge);

    currentMagnitude -= CHARGE_STEP;
    if (currentMagnitude < MIN_CHARGE_MAGNITUDE) {
        // Allow it to go to zero, or clamp at MIN_CHARGE_MAGNITUDE if it was already non-zero
        if (std::abs(m_charge) > 0.0f) { // if it wasn't already zero
             currentMagnitude = MIN_CHARGE_MAGNITUDE;
        } else {
            currentMagnitude = 0.0f; // if it was zero, it stays zero or becomes min magnitude positive if sign was positive
        }
    }
     if (currentMagnitude < 0.0f) currentMagnitude = 0.0f; // cannot be less than zero magnitude


    m_charge = currentSign * currentMagnitude;
    // If charge was exactly MIN_CHARGE_MAGNITUDE and decreased, it should become 0 or flip sign if that's the desired logic.
    // The current logic will make it MIN_CHARGE_MAGNITUDE. Let's refine:
    // If player decreases charge when at MIN_CHARGE_MAGNITUDE, it should probably go to 0.
    // The problem is if charge is -0.1 and they decrease, it goes to -0.1.
    // Let's adjust `setCharge` to handle this clamping properly.

    setCharge(currentSign * currentMagnitude); // Use setCharge to ensure clamping
    std::cout << "Charge decreased to: " << m_charge << std::endl;
}


// void Player::toggleChargeSign() {
//     if (m_charge != 0.f) { // Don't toggle sign if charge is zero
//         m_charge *= -1.f;
//     }
//     std::cout << "Charge sign toggled to: " << m_charge << std::endl;
// }
void Player::toggleChargeSign() {
    if (std::abs(m_charge) >= MIN_CHARGE_MAGNITUDE) { // Only toggle if charge has some magnitude
        m_charge *= -1.f;
    } else if (m_charge == 0.f) { // If charge is zero, make it min positive
        m_charge = MIN_CHARGE_MAGNITUDE;
    }
    std::cout << "Charge sign toggled to: " << m_charge << std::endl;
}

float Player::getCharge() const {
    return m_charge;
}
// void Player::setCharge(float charge) {
//     m_charge = charge;
//     // Clamp to bounds
//     if (m_charge > MAX_CHARGE) m_charge = MAX_CHARGE;
//     if (m_charge < -MAX_CHARGE) m_charge = -MAX_CHARGE;
//     if (std::abs(m_charge) < MIN_CHARGE && m_charge != 0.f) {
//          m_charge = (m_charge > 0) ? MIN_CHARGE : -MIN_CHARGE;
//     }
// }
void Player::setCharge(float charge) {
    float sign = (charge == 0.f) ? 1.f : std::copysign(1.0f, charge);
    float magnitude = std::abs(charge);

    if (magnitude > MAX_CHARGE) {
        magnitude = MAX_CHARGE;
    }
    // If magnitude is between 0 and MIN_CHARGE_MAGNITUDE (exclusive of 0), set to MIN_CHARGE_MAGNITUDE
    // unless the intent was to set it to 0.
    if (magnitude < MIN_CHARGE_MAGNITUDE && magnitude != 0.f) {
        magnitude = MIN_CHARGE_MAGNITUDE;
    }
    
    m_charge = sign * magnitude;
}