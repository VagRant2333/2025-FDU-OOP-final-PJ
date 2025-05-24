// src/render/ResourceManager.h
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <map>
#include <stdexcept> // Required for std::runtime_error
#include <iostream>  // Required for std::cerr


class ResourceManager {
public:
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    sf::Texture& loadTexture(const std::string& id, const std::string& filepath) {
        std::cout << "load texture./ ResourceManager.cpp" << std::endl;
        if (m_textures.find(id) == m_textures.end()) {
            sf::Texture texture;
            if (!texture.loadFromFile(filepath)) {
                throw std::runtime_error("Failed to load texture: " + filepath);
            }
            m_textures[id] = texture; // This copies the texture
            std::cout << "Loaded texture: " << filepath << " as " << id << std::endl;
        }
        return m_textures.at(id);
    }

    sf::Font& loadFont(const std::string& id, const std::string& filepath) {
        std::cout << "load font./ ResourceManager.cpp" << std::endl;
        if (m_fonts.find(id) == m_fonts.end()) {
            sf::Font font;
            if (!font.loadFromFile(filepath)) {
                // Since we're using default font if this fails, we can make a default font.
                // For now, let's throw an error or log.
                // A truly robust default would involve embedding a font or using a known system font.
                // The user requested default font, so we can skip custom font loading if it's problematic.
                // However, SFML Text needs a font. We'll try to load one, if not, text won't show.
                // For "default font", SFML doesn't have one built-in that works out-of-the-box for sf::Text without loadFromFile.
                // Let's assume a simple "arial.ttf" should be provided for basic text.
                std::cerr << "Warning: Failed to load font: " + filepath + ". Text might not display." << std::endl;
                // Create a placeholder font or handle error appropriately
                // For now, if font loading fails, using it will be problematic.
                // A simple fallback is to not display text or use a very basic shape.
                // The requirement "directly in the UI display default fonts" is tricky with SFML.
                // We MUST load a font file. I'll add a default font asset.
                // ASSET_PATH: Create a font file, e.g., assets/fonts/arial.ttf
                if (!m_defaultFont.loadFromFile("../../assets/fonts/arial.ttf")) { // Placeholder path
                     throw std::runtime_error("Failed to load default font: assets/fonts/arial.ttf");
                }
                m_fonts[id] = m_defaultFont;
            } else {
                 m_fonts[id] = font;
            }
            std::cout << "Loaded font: " << filepath << " as " << id << std::endl;
        }
        return m_fonts.at(id);
    }
    
    // Use this for default font if no path is given, assuming "assets/fonts/arial.ttf" exists
    sf::Font& getDefaultFont() {
        if (m_fonts.find("default") == m_fonts.end()) {
            if (!m_defaultFont.loadFromFile("../../assets/fonts/arial.ttf")) { // ASSET_PATH
                throw std::runtime_error("FATAL: Could not load default font: assets/fonts/arial.ttf. Please ensure this file exists.");
            }
            m_fonts["default"] = m_defaultFont;
            std::cout << "Loaded default font as 'default'" << std::endl;
        }
        return m_fonts.at("default");
    }


    sf::SoundBuffer& loadSoundBuffer(const std::string& id, const std::string& filepath) {
        std::cout << "load sound buffer./ ResourceManager.cpp" << std::endl;
        if (m_soundBuffers.find(id) == m_soundBuffers.end()) {
            sf::SoundBuffer buffer;
            if (!buffer.loadFromFile(filepath)) {
                throw std::runtime_error("Failed to load sound buffer: " + filepath);
            }
            m_soundBuffers[id] = buffer;
            std::cout << "Loaded sound buffer: " << filepath << " as " << id << std::endl;
        }
        return m_soundBuffers.at(id);
    }

private:
    ResourceManager() = default; // Private constructor for Singleton
    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, sf::Font> m_fonts;
    std::map<std::string, sf::SoundBuffer> m_soundBuffers;
    sf::Font m_defaultFont; // Fallback font
};

#endif // RESOURCEMANAGER_H