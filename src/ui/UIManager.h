// src/ui/Button.h (Could be UIManager.h if you expand it)
#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

struct Button {
    sf::RectangleShape shape;
    sf::Text text;
    sf::Sprite sprite; // Optional: for image buttons
    bool useSprite = false;

    std::function<void()> onClick;

    Button(const std::string& label, sf::Font& font, unsigned int charSize,
           sf::Vector2f position, sf::Vector2f size, 
           sf::Color bgColor = sf::Color(100, 100, 100), 
           sf::Color textColor = sf::Color::White) {
        
        shape.setPosition(position);
        shape.setSize(size);
        shape.setFillColor(bgColor);
        shape.setOutlineThickness(2.f);
        shape.setOutlineColor(sf::Color(50,50,50));

        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(charSize);
        text.setFillColor(textColor);

        // Center text
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top + textRect.height / 2.0f);
        text.setPosition(position.x + size.x / 2.0f,
                         position.y + size.y / 2.0f);
        useSprite = false;
    }

    // Constructor for sprite-based button
    Button(sf::Texture& texture, sf::Vector2f position, sf::Vector2f scale = {1.f, 1.f}) {
        sprite.setTexture(texture);
        sprite.setPosition(position);
        sprite.setScale(scale);
        // Shape can be used for bounds if needed, but transparent for sprite button
        shape.setPosition(position);
        shape.setSize(sf::Vector2f(texture.getSize().x * scale.x, texture.getSize().y * scale.y));
        shape.setFillColor(sf::Color::Transparent);
        useSprite = true;
    }


    void draw(sf::RenderWindow& window) {
        if (useSprite) {
            window.draw(sprite);
        } else {
            window.draw(shape);
            window.draw(text);
        }
    }

    bool isMouseOver(sf::RenderWindow& window) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (useSprite) {
            return sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
        }
        return shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    }

    void setHoverColors(sf::Color bgHoverColor, sf::Color textHoverColor) {
        // Store these and apply in an update method if you want visual feedback
        // For simplicity, this example doesn't dynamically change colors on hover in Button struct itself.
        // Scenes can manage this.
    }

    void setOnClickAction(std::function<void()> action) {
        onClick = action;
    }

    void triggerClick() {
        if (onClick) {
            onClick();
        }
    }
};

#endif // BUTTON_H