// src/entities/Entity.h
#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

class Entity
{
public:
    virtual ~Entity() = default;

    virtual void update(sf::Time deltaTime) = 0;
    virtual void render(sf::RenderWindow &window) = 0;

    virtual sf::FloatRect getBounds() const = 0;
    virtual sf::Vector2f getPosition() const = 0;
    virtual void setPosition(const sf::Vector2f &pos) = 0;

    bool isActive = true; // for simple removal
};

#endif // ENTITY_H