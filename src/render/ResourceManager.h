// src/render/ResourceManager.h
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <map>
#include <stdexcept>
#include <iostream>

class ResourceManager
{
public:
    ResourceManager(const ResourceManager &) = delete;
    ResourceManager &operator=(const ResourceManager &) = delete;

    static ResourceManager &getInstance()
    {
        static ResourceManager instance;
        return instance;
    }

    sf::Texture &loadTexture(const std::string &id, const std::string &filepath)
    {
        // std::cout << "load texture./ ResourceManager.cpp" << std::endl;
        if (m_textures.find(id) == m_textures.end())
        {
            sf::Texture texture;
            if (!texture.loadFromFile(filepath))
            {
                throw std::runtime_error("Failed to load texture: " + filepath);
            }
            m_textures[id] = texture;
            // std::cout << "Loaded texture: " << filepath << " as " << id << std::endl;
        }
        return m_textures.at(id);
    }

    sf::Font &loadFont(const std::string &id, const std::string &filepath)
    {
        // std::cout << "load font./ ResourceManager.cpp" << std::endl;
        if (m_fonts.find(id) == m_fonts.end())
        {
            sf::Font font;
            if (!font.loadFromFile(filepath))
            {

                std::cerr << "Warning: Failed to load font: " + filepath + ". Text might not display." << std::endl;
                if (!m_defaultFont.loadFromFile("../../assets/fonts/Twinster.ttf"))
                { // Placeholder path
                    throw std::runtime_error("Failed to load default font: assets/fonts/Twinster.ttf");
                }
                m_fonts[id] = m_defaultFont;
            }
            else
            {
                m_fonts[id] = font;
            }
            // std::cout << "Loaded font: " << filepath << " as " << id << std::endl;
        }
        return m_fonts.at(id);
    }
    sf::Font &getDefaultFont()
    {
        if (m_fonts.find("default") == m_fonts.end())
        {
            if (!m_defaultFont.loadFromFile("../../assets/fonts/Twinster.ttf"))
            { // ASSET_PATH
                throw std::runtime_error("FATAL: Could not load default font: assets/fonts/Twinster.ttf. Please ensure this file exists.");
            }
            m_fonts["default"] = m_defaultFont;
        }
        return m_fonts.at("default");
    }

    sf::SoundBuffer &loadSoundBuffer(const std::string &id, const std::string &filepath)
    {
        // std::cout << "load sound buffer./ ResourceManager.cpp" << std::endl;
        if (m_soundBuffers.find(id) == m_soundBuffers.end())
        {
            sf::SoundBuffer buffer;
            if (!buffer.loadFromFile(filepath))
            {
                throw std::runtime_error("Failed to load sound buffer: " + filepath);
            }
            m_soundBuffers[id] = buffer;
            // std::cout << "Loaded sound buffer: " << filepath << " as " << id << std::endl;
        }
        return m_soundBuffers.at(id);
    }

private:
    ResourceManager() = default; // constructor
    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, sf::Font> m_fonts;
    std::map<std::string, sf::SoundBuffer> m_soundBuffers;
    sf::Font m_defaultFont;
};

#endif // RESOURCEMANAGER_H