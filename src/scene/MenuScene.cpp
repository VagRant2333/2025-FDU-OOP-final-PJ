// src/scene/MenuScene.cpp
#include "MenuScene.h"
#include "../render/ResourceManager.h"
#include "../core/Game.h"
#include <iostream>

const float SIDEBAR_WIDTH = 250.f;
const float PADDING = 10.f;
const float BUTTON_HEIGHT = 50.f;
const float BUTTON_WIDTH_MAIN = 200.f;

MenuScene::MenuScene(Game &game)
    : Scene(game),
      m_font(ResourceManager::getInstance().getDefaultFont()),
      m_startButton("", m_font, 24, {0, 0}, {0, 0}),
      m_exitButton("", m_font, 24, {0, 0}, {0, 0}),
      m_volumeUpButton("+", m_font, 20, {0, 0}, {30, 30}),
      m_volumeDownButton("-", m_font, 20, {0, 0}, {30, 30}),
      m_closeScrollViewButton("Close", m_font, 20, {0, 0}, {100, 40})
{
    std::cout << "MenuScene created." << std::endl;
}

MenuScene::~MenuScene()
{
    if (m_menuMusic.getStatus() == sf::Music::Playing)
    {
        m_menuMusic.stop();
    }
    std::cout << "MenuScene destroyed." << std::endl;
}

void MenuScene::loadAssets()
{
    std::cout << "MenuScene loading assets." << std::endl;
    try
    {
        // Background
        sf::Texture &bgTex = ResourceManager::getInstance().loadTexture("menu_bg", "../../assets/images/menu_bg.png");
        m_backgroundSprite.setTexture(bgTex);

        // float bgScaleX = static_cast<float>(m_game.getWindow().getSize().x) / bgTex.getSize().x;
        // float bgScaleY = static_cast<float>(m_game.getWindow().getSize().y) / bgTex.getSize().y;
        // m_backgroundSprite.setScale(bgScaleX, bgScaleY);
        sf::Vector2u windowSize = m_game.getWindow().getSize();
        sf::Vector2u textureSize = bgTex.getSize();
        float scaleX = (float)windowSize.x / textureSize.x;
        float scaleY = (float)windowSize.y / textureSize.y;
        float scale = std::max(scaleX, scaleY);
        m_backgroundSprite.setScale(scale, scale);

        // title
        m_gameTitleText.setFont(m_font);
        m_gameTitleText.setString("DenPaKid");
        m_gameTitleText.setCharacterSize(72);
        m_gameTitleText.setFillColor(sf::Color::Yellow);
        m_gameTitleText.setStyle(sf::Text::Bold);

        sf::FloatRect titleRect = m_gameTitleText.getLocalBounds();
        m_gameTitleText.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
        float titleX = (m_game.getWindow().getSize().x + SIDEBAR_WIDTH) / 2.0f;
        m_gameTitleText.setPosition(titleX, m_game.getWindow().getSize().y * 0.2f);

        // music
        if (!m_menuMusic.openFromFile("../../assets/audio/menu_theme.ogg"))
        {
            throw std::runtime_error("Failed to load menu_theme.ogg");
        }
        m_menuMusic.setLoop(true);
        m_menuMusic.setVolume(m_game.getMasterVolume());
        m_menuMusic.play();

        // scroll
        m_scrollIconTexture = ResourceManager::getInstance().loadTexture("scroll_item_icon", "../../assets/images/scroll_item.png");
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Error loading assets in MenuScene: " << e.what() << std::endl;
    }
    setupUI(); // Setup UI elements after assets are potentially loaded
    std::cout << "MenuScene assets loaded." << std::endl;
}

void MenuScene::setupUI()
{
    sf::Vector2u windowSize = m_game.getWindow().getSize();
    float buttonYStart = m_gameTitleText.getPosition().y + m_gameTitleText.getLocalBounds().height * 2.0f;
    float mainButtonX = (windowSize.x + SIDEBAR_WIDTH) / 2.0f - BUTTON_WIDTH_MAIN / 2.0f;

    m_startButton = Button("Start Game", m_font, 30,
                           {mainButtonX, buttonYStart},
                           {BUTTON_WIDTH_MAIN, BUTTON_HEIGHT}, sf::Color(0, 150, 0));
    m_startButton.setOnClickAction([this]()
                                   { m_game.changeScene(GameState::Playing); });

    m_exitButton = Button("Exit Game", m_font, 30,
                          {mainButtonX, buttonYStart + BUTTON_HEIGHT + PADDING * 2.f},
                          {BUTTON_WIDTH_MAIN, BUTTON_HEIGHT}, sf::Color(150, 0, 0));
    m_exitButton.setOnClickAction([this]()
                                  { m_game.m_exitGame = true; });

    // sidebar
    m_sidebarBackground.setSize(sf::Vector2f(SIDEBAR_WIDTH, static_cast<float>(windowSize.y)));
    m_sidebarBackground.setFillColor(sf::Color(50, 50, 50, 200));

    // settings
    m_settingsTitle.setFont(m_font);
    m_settingsTitle.setString("Settings");
    m_settingsTitle.setCharacterSize(28);
    m_settingsTitle.setFillColor(sf::Color::White);
    m_settingsTitle.setPosition(PADDING, PADDING);

    // Volume Controls
    float currentY = m_settingsTitle.getPosition().y + m_settingsTitle.getLocalBounds().height + PADDING * 2.f;
    m_volumeDownButton = Button("-", m_font, 24, {PADDING, currentY}, {40.f, 40.f});
    m_volumeDownButton.setOnClickAction([this]()
                                        {
                                            m_game.setMasterVolume(m_game.getMasterVolume() - 10.f);
                                            onVolumeChanged(); });

    m_volumeValueText.setFont(m_font);
    m_volumeValueText.setCharacterSize(24);
    m_volumeValueText.setFillColor(sf::Color::White);
    // between buttons, updated in onvolumechanged
    m_volumeValueText.setPosition(PADDING + 40.f + PADDING, currentY + 5.f);
    onVolumeChanged();

    m_volumeUpButton = Button("+", m_font, 24, {PADDING + 40.f + PADDING + 60.f + PADDING, currentY}, {40.f, 40.f});
    m_volumeUpButton.setOnClickAction([this]()
                                      {
                                          m_game.setMasterVolume(m_game.getMasterVolume() + 10.f);
                                          onVolumeChanged(); });

    currentY += 40.f + PADDING * 3.f;

    // collection
    m_collectionTitle.setFont(m_font);
    m_collectionTitle.setString("Scrolls");
    m_collectionTitle.setCharacterSize(28);
    m_collectionTitle.setFillColor(sf::Color::White);
    m_collectionTitle.setPosition(PADDING, currentY);
    currentY += m_collectionTitle.getLocalBounds().height + PADDING * 2.f;

    // collection button
    m_scrollButtons.clear();
    float scrollIconSize = 48.f;
    float scrollIconSpacing = PADDING;
    int iconsPerRow = static_cast<int>((SIDEBAR_WIDTH - 2 * PADDING) / (scrollIconSize + scrollIconSpacing));

    for (int i = 0; i < m_game.getTotalScrolls(); ++i)
    {
        if (m_game.isScrollCollected(i))
        {
            Button scrollButton(m_scrollIconTexture, {0, 0});
            scrollButton.sprite.setTextureRect(sf::IntRect(0, 0, m_scrollIconTexture.getSize().x, m_scrollIconTexture.getSize().y));
            scrollButton.sprite.setScale(scrollIconSize / m_scrollIconTexture.getSize().x,
                                         scrollIconSize / m_scrollIconTexture.getSize().y);

            float posX = PADDING + (i % iconsPerRow) * (scrollIconSize + scrollIconSpacing);
            float posY = currentY + (i / iconsPerRow) * (scrollIconSize + scrollIconSpacing);
            scrollButton.sprite.setPosition(posX, posY);
            scrollButton.shape.setPosition(posX, posY);
            scrollButton.shape.setSize({scrollIconSize, scrollIconSize});
            scrollButton.setOnClickAction([this, scrollId = i]()
                                          { displayScrollContent(scrollId); });
            m_scrollButtons.push_back(scrollButton);
        }
        else
        {
        }
    }

    // scroll view UI (hidden at first)
    m_scrollDisplayBackground.setSize(sf::Vector2f(windowSize.x * 0.6f, windowSize.y * 0.7f));
    m_scrollDisplayBackground.setFillColor(sf::Color(30, 30, 70, 220));
    m_scrollDisplayBackground.setOutlineColor(sf::Color::Cyan);
    m_scrollDisplayBackground.setOutlineThickness(2.f);
    m_scrollDisplayBackground.setPosition(windowSize.x * 0.2f, windowSize.y * 0.15f);

    m_scrollDisplayContentText.setFont(m_font);
    m_scrollDisplayContentText.setCharacterSize(20);
    m_scrollDisplayContentText.setFillColor(sf::Color::White);
    m_scrollDisplayContentText.setPosition(m_scrollDisplayBackground.getPosition().x + PADDING,
                                           m_scrollDisplayBackground.getPosition().y + PADDING);

    m_closeScrollViewButton = Button("Close", m_font, 22,
                                     {m_scrollDisplayBackground.getPosition().x + m_scrollDisplayBackground.getSize().x / 2.f - 50.f,
                                      m_scrollDisplayBackground.getPosition().y + m_scrollDisplayBackground.getSize().y - BUTTON_HEIGHT - PADDING},
                                     {100.f, BUTTON_HEIGHT});
    m_closeScrollViewButton.setOnClickAction([this]()
                                             { m_isViewingScroll = false; });
}

void MenuScene::displayScrollContent(int scrollId)
{
    m_scrollDisplayContentText.setString(m_game.getScrollContent(scrollId));

    m_isViewingScroll = true;
}

void MenuScene::handleInput(sf::Event &event, sf::RenderWindow &window)
{
    if (m_isViewingScroll)
    {
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                if (m_closeScrollViewButton.isMouseOver(window))
                {
                    m_closeScrollViewButton.triggerClick();
                }
            }
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            m_isViewingScroll = false;
        }
        return;
    }

    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            if (m_startButton.isMouseOver(window))
            {
                m_startButton.triggerClick();
            }
            else if (m_exitButton.isMouseOver(window))
            {
                m_exitButton.triggerClick();
            }
            else if (m_volumeUpButton.isMouseOver(window))
            {
                m_volumeUpButton.triggerClick();
            }
            else if (m_volumeDownButton.isMouseOver(window))
            {
                m_volumeDownButton.triggerClick();
            }
            else
            {
                for (auto &scrollBtn : m_scrollButtons)
                {
                    if (scrollBtn.isMouseOver(window))
                    {
                        scrollBtn.triggerClick();
                        break;
                    }
                }
            }
        }
    }
}

void MenuScene::update(sf::Time deltaTime)
{
}

void MenuScene::render(sf::RenderWindow &window)
{
    window.clear(sf::Color(30, 30, 30)); // dark bg
    window.draw(m_backgroundSprite);
    window.draw(m_gameTitleText);

    m_startButton.draw(window);
    m_exitButton.draw(window);

    // sidebar
    window.draw(m_sidebarBackground);
    window.draw(m_settingsTitle);
    m_volumeUpButton.draw(window);
    m_volumeDownButton.draw(window);
    window.draw(m_volumeValueText);

    window.draw(m_collectionTitle);
    for (auto &scrollBtn : m_scrollButtons)
    {
        scrollBtn.draw(window);
    }

    // scroll view
    if (m_isViewingScroll)
    {
        window.draw(m_scrollDisplayBackground);
        window.draw(m_scrollDisplayContentText);
        m_closeScrollViewButton.draw(window);
    }
}

void MenuScene::onVolumeChanged()
{
    if (m_menuMusic.getStatus() == sf::Music::Playing || m_menuMusic.getStatus() == sf::Music::Paused)
    {
        m_menuMusic.setVolume(m_game.getMasterVolume());
    }
    m_volumeValueText.setString(std::to_string(static_cast<int>(m_game.getMasterVolume())));

    sf::FloatRect textRect = m_volumeValueText.getLocalBounds();
    m_volumeValueText.setOrigin(textRect.left + textRect.width / 2.0f,
                                textRect.top + textRect.height / 2.0f);
    m_volumeValueText.setPosition(m_volumeDownButton.shape.getPosition().x + m_volumeDownButton.shape.getSize().x + PADDING + 30.f,
                                  m_volumeDownButton.shape.getPosition().y + m_volumeDownButton.shape.getSize().y / 2.f);
}