#include "GameScene.h"
#include "../render/ResourceManager.h"
#include <iostream>
#include <string>
#include <algorithm>

const float PADDING = 10.f;
const float PLAYER_START_Y_OFFSET = -100.f;
const float BOTTOM_LASER_HEIGHT = 10.f;

GameScene::GameScene(Game &game)
    : Scene(game),
      m_playerTexture(),
      m_rng(std::random_device{}())
{
    // std::cout << "GameScene created. /GameScene.cpp" << std::endl;
}

GameScene::~GameScene()
{
    if (m_gameMusic.getStatus() == sf::Music::Playing)
    {
        m_gameMusic.stop();
    }
    // std::cout << "GameScene destroyed." << std::endl;
}

void GameScene::loadAssets()
{
    // std::cout << "GameScene loading assets. /GameScene.cpp" << std::endl;
    try
    {
        m_hudFont = ResourceManager::getInstance().getDefaultFont();
        m_playerTexture = ResourceManager::getInstance().loadTexture("player", "../../assets/images/player_sprite.png");

        sf::Vector2f playerStartPos = {
            static_cast<float>(m_game.getWindow().getSize().x) / 5.f,
            static_cast<float>(m_game.getWindow().getSize().y) / 2.f};
        // create the player object
        m_player = std::make_unique<Player>(playerStartPos, m_playerTexture);
        m_player->setCharge(1.0f);
        m_player->setVelocity({100.f, 0.f});

        // background
        m_gameBgTexture = ResourceManager::getInstance().loadTexture("game_bg", "../../assets/images/game_bg.png");
        m_gameBgTexture.setRepeated(true);
        // scale
        float bgScaleX = static_cast<float>(m_game.getWindow().getSize().x) / m_gameBgTexture.getSize().x;
        float bgScaleY = static_cast<float>(m_game.getWindow().getSize().y) / m_gameBgTexture.getSize().y;
        float uniformScale = std::max(bgScaleX, bgScaleY);

        m_backgroundSprite1.setTexture(m_gameBgTexture);
        m_backgroundSprite2.setTexture(m_gameBgTexture);

        m_backgroundSprite1.setScale(uniformScale, uniformScale);
        m_backgroundSprite2.setScale(uniformScale, uniformScale);

        float scaledBgWidth = m_gameBgTexture.getSize().x * uniformScale;

        m_backgroundSprite1.setPosition(0, 0);
        m_backgroundSprite2.setPosition(scaledBgWidth - 1, 0);

        m_bgScrollSpeed = 150.f;
        // m_backgroundSprite1.setTexture(m_gameBgTexture);
        // m_backgroundSprite2.setTexture(m_gameBgTexture);

        // // Ensure texture repeats or is wide enough. For scrolling, two sprites are common.
        // m_backgroundSprite1.setPosition(0, 0);
        // m_backgroundSprite2.setPosition(m_gameBgTexture.getSize().x, 0); // Position second sprite after the first
        //                                                                  // Scale to fit height, width will scroll
        // float bgScaleY = static_cast<float>(m_game.getWindow().getSize().y) / m_gameBgTexture.getSize().y;
        // m_backgroundSprite1.setScale(bgScaleY, bgScaleY); // Assuming square pixels in texture, scale uniformly based on height
        // m_backgroundSprite2.setScale(bgScaleY, bgScaleY);
        // // Adjust scroll speed based on scaled width
        // float scaledBgWidth = m_gameBgTexture.getSize().x * m_backgroundSprite1.getScale().x;
        // m_backgroundSprite1.setPosition(0, 0);
        // m_backgroundSprite2.setPosition(scaledBgWidth, 0);

        // music
        if (!m_gameMusic.openFromFile("../../assets/audio/game_theme.ogg"))
        {
            throw std::runtime_error("Failed to load game_theme.ogg");
        }
        m_gameMusic.setLoop(true);
        m_gameMusic.setVolume(m_game.getMasterVolume() * 0.7f);
        m_gameMusic.play();

        // laser sound
        m_laserSound.setBuffer(ResourceManager::getInstance().loadSoundBuffer("laser_fire", "../../assets/audio/laser_sound.ogg"));
        m_laserSound.setVolume(m_game.getMasterVolume() * 0.5f);

        // scroll
        m_scrollItemTexture = ResourceManager::getInstance().loadTexture("scroll_item", "../../assets/images/scroll_item.png");

        m_laserTexture = ResourceManager::getInstance().loadTexture("laser_beam", "../../assets/images/laser.png");
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Error loading assets in GameScene: " << e.what() << std::endl;
    }
    setupInitialState();
    // std::cout << "GameScene assets loaded." << std::endl;
}

void GameScene::setupInitialState()
{
    // std::cout << "set up initial state. /GameScene.cpp" << std::endl;
    m_isGameOver = false;
    m_distanceTraveled = 0.f;
    if (m_player)
    {
        m_player->setPosition({static_cast<float>(m_game.getWindow().getSize().x) / 5.f,
                               static_cast<float>(m_game.getWindow().getSize().y) / 2.f});
        m_player->setVelocity({100.f, 0.f});
        m_player->setCharge(1.0f);
        m_player->resetDashCharges();
    }

    // HUD
    m_distanceText.setFont(m_hudFont);
    m_distanceText.setCharacterSize(24);
    m_distanceText.setFillColor(sf::Color::White);
    m_distanceText.setPosition(m_game.getWindow().getSize().x - 250.f, 20.f);

    m_chargeText.setFont(m_hudFont);
    m_chargeText.setCharacterSize(24);
    m_chargeText.setFillColor(sf::Color::White);
    m_chargeText.setPosition(m_game.getWindow().getSize().x - 250.f, 50.f);

    m_dashChargesText.setFont(m_hudFont);
    m_dashChargesText.setCharacterSize(24);
    m_dashChargesText.setFillColor(sf::Color::Cyan);
    m_dashChargesText.setPosition(m_game.getWindow().getSize().x - 250.f, 80.f);

    // Bottom Laser
    // m_bottomLaser.setSize(sf::Vector2f(static_cast<float>(m_game.getWindow().getSize().x), BOTTOM_LASER_HEIGHT));
    // m_bottomLaser.setFillColor(sf::Color(255, 0, 0, 150)); // Semi-transparent red
    // m_bottomLaser.setPosition(0, m_game.getWindow().getSize().y - BOTTOM_LASER_HEIGHT);

    // Field Visuals Setup (text for E, shapes for B)
    // m_eFieldPositiveText.setFont(m_hudFont);
    // m_eFieldPositiveText.setCharacterSize(20);
    // m_eFieldPositiveText.setFillColor(sf::Color::Red);
    // m_eFieldPositiveText.setString("+ + + E + + +");

    // field random
    randomizeFields();
    updateFieldVisuals();

    m_laserSpawnTimer = sf::Time::Zero;
    m_scrollSpawnTimer = sf::Time::Zero;
    m_timeBetweenScrollSpawns = sf::seconds(10.f + (m_rng() % 10));
    m_lasers.clear();
    m_scrollsInScene.clear();

    m_physicsEngine.setPlayerMass(1.0f);
}

void GameScene::handleInput(sf::Event &event, sf::RenderWindow &window)
{
    if (m_isGameOver)
        return;

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Q)
        {
            m_player->decreaseCharge();
        }
        else if (event.key.code == sf::Keyboard::E)
        {
            m_player->increaseCharge();
        }
        else if (event.key.code == sf::Keyboard::Space)
        {
            m_player->toggleChargeSign();
        }
        else if (event.key.code == sf::Keyboard::W)
        {
            m_player->dash({0.f, -1.f});
        }
        else if (event.key.code == sf::Keyboard::A)
        {
            m_player->dash({-1.f, 0.f});
        }
        else if (event.key.code == sf::Keyboard::S)
        {
            m_player->dash({0.f, 1.f});
        }
        else if (event.key.code == sf::Keyboard::D)
        {
            m_player->dash({1.f, 0.f});
        }
        // else if (event.key.code == sf::Keyboard::R) {
        //     randomizeFields();
        // }
    }
}

void GameScene::update(sf::Time deltaTime)
{
    if (m_isGameOver)
        return;

    m_physicsEngine.updatePlayer(*m_player, deltaTime, m_currentFields, m_game.getWindow().getSize());
    m_player->update(deltaTime);

    updateBackground(deltaTime);
    m_distanceTraveled += m_bgScrollSpeed * deltaTime.asSeconds() * 0.1f;

    // spawn
    m_laserSpawnTimer += deltaTime;
    if (m_laserSpawnTimer >= m_timeBetweenLaserSpawns)
    {
        spawnLaser();
        m_laserSpawnTimer = sf::Time::Zero;
        // random spawn time
        std::uniform_real_distribution<float> dist(1.5f, 4.0f);
        m_timeBetweenLaserSpawns = sf::seconds(dist(m_rng));
    }

    m_scrollSpawnTimer += deltaTime;
    if (m_scrollsInScene.size() < m_maxScrollsOnScreen && m_game.getCollectedScrollsCount() < m_game.getTotalScrolls())
    {
        if (m_scrollSpawnTimer >= m_timeBetweenScrollSpawns)
        {
            spawnScroll();
            m_scrollSpawnTimer = sf::Time::Zero;
            std::uniform_real_distribution<float> dist(8.f, 15.f); // random next scroll spawn
            m_timeBetweenScrollSpawns = sf::seconds(dist(m_rng));
        }
    }

    updateLasers(deltaTime);
    updateScrolls(deltaTime);
    updateHUD();
    updateFieldVisuals();
    for (const auto &laser : m_lasers)
    {
        if (laser.isActive && m_player->getBounds().intersects(laser.getBounds()))
        {
            m_isGameOver = true;
            break;
        }
    }
    if (m_player->getPosition().y < -m_player->getBounds().height)
    {
        m_isGameOver = true;
    }

    if (m_isGameOver)
    {
        m_gameMusic.stop();
        m_game.playerDied(m_distanceTraveled);
    }
    else
    {
        m_game.checkWinCondition(m_distanceTraveled);
        if (m_game.getCurrentState() == GameState::GameWon)
        {
            m_isGameOver = true;
        }
    }
}

void GameScene::updateBackground(sf::Time deltaTime)
{
    float moveDistance = m_bgScrollSpeed * deltaTime.asSeconds();
    m_backgroundSprite1.move(-moveDistance, 0);
    m_backgroundSprite2.move(-moveDistance, 0);

    float scaledBgWidth = m_gameBgTexture.getSize().x * m_backgroundSprite1.getScale().x;

    if (m_backgroundSprite1.getPosition().x + scaledBgWidth <= 0)
    {
        m_backgroundSprite1.setPosition(m_backgroundSprite2.getPosition().x + scaledBgWidth, 0);
    }
    if (m_backgroundSprite2.getPosition().x + scaledBgWidth <= 0)
    {
        m_backgroundSprite2.setPosition(m_backgroundSprite1.getPosition().x + scaledBgWidth, 0);
    }
}

void GameScene::randomizeFields()
{
    // std::cout << "GameScene randomize fields.. /GameScene.cpp" << std::endl;
    std::uniform_real_distribution<float> e_dist(-50.f, 50.f);
    std::uniform_real_distribution<float> b_dist(-2.f, 2.f);
    std::uniform_int_distribution<int> direction_dist(0, 1); // E field direction

    if (direction_dist(m_rng) == 0)
    {
        // m_currentFields.electricField = {e_dist(m_rng), 0.f};
        m_currentFields.electricField = sf::Vector2f(e_dist(m_rng), 0.f);
    }
    else
    {
        // m_currentFields.electricField = {0.f, e_dist(m_rng) / 2.f};
        m_currentFields.electricField = sf::Vector2f(0.f, e_dist(m_rng) / 2.f);
    }
    m_currentFields.magneticField_Z = b_dist(m_rng);

    // called when spawning laser or on a separate timer
    std::cout << "Fields randomized: E(" << m_currentFields.electricField.x << "," << m_currentFields.electricField.y
              << "), B(" << m_currentFields.magneticField_Z << ")" << std::endl;
}

void GameScene::updateFieldVisuals()
{
    // std::cout << "GameScene filed visializing. /GameScene.cpp" << std::endl;
    sf::Vector2u windowSize = m_game.getWindow().getSize();

    m_bFieldSymbols.clear();

    // if (std::abs(m_currentFields.magneticField_Z) > 0.05f) { // Only draw if B is significant
    //     std::cout << "B field drawing dots/points. /GameScene.cpp" << std::endl;
    //     int numSymbols = 20; // Number of symbols to draw
    //     float radius = (m_currentFields.magneticField_Z > 0) ? 3.f : 5.f;
    //     sf::Color bColor = (m_currentFields.magneticField_Z > 0) ? sf::Color(200,200,255,150) : sf::Color(100,100,150,150);

    if (std::abs(m_currentFields.magneticField_Z) > 0.05f)
    { // only draw if B
        int numSymbolsTarget = static_cast<int>(std::abs(m_currentFields.magneticField_Z) * B_FIELD_DENSITY_SCALE_FACTOR);

        // determine grid
        int symbolsPerRow = static_cast<int>(std::sqrt(numSymbolsTarget));
        symbolsPerRow = std::min(symbolsPerRow, MAX_B_FIELD_SYMBOLS_PER_ROW_COL);
        symbolsPerRow = std::max(1, symbolsPerRow); // at least 1 if target > 0
        int symbolsPerCol = symbolsPerRow;          // grid

        if (symbolsPerRow == 0 && numSymbolsTarget > 0)
            symbolsPerRow = 1;
        if (symbolsPerCol == 0 && numSymbolsTarget > 0)
            symbolsPerCol = 1;

        float spacingX = static_cast<float>(windowSize.x) / (symbolsPerRow + 1);
        float spacingY = static_cast<float>(windowSize.y) / (symbolsPerCol + 1);

        sf::Color bColor = (m_currentFields.magneticField_Z > 0) ? sf::Color(255, 100, 100, 180) : sf::Color(180, 50, 50, 180);
        std::string symbolChar = (m_currentFields.magneticField_Z > 0) ? "." : "X";
        unsigned int charSize = (m_currentFields.magneticField_Z > 0) ? 150 : 80;

        for (int r = 0; r < symbolsPerCol; ++r)
        {
            for (int c = 0; c < symbolsPerRow; ++c)
            {
                sf::Text bSymbol;
                bSymbol.setFont(m_hudFont); // m_hudFont is loaded
                bSymbol.setString(symbolChar);
                bSymbol.setCharacterSize(charSize);
                bSymbol.setFillColor(bColor);

                sf::FloatRect textRect = bSymbol.getLocalBounds();
                bSymbol.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                bSymbol.setPosition((c + 1) * spacingX, (r + 1) * spacingY);
                m_bFieldSymbols.push_back(bSymbol);
            }
        }
    }

    // E Field
    m_eFieldLines.clear();
    if (std::abs(m_currentFields.electricField.x) > 1.0f || std::abs(m_currentFields.electricField.y) > 1.0f)
    {
        // std::cout << "drawing E field arrows. /GameScene.cpp" << std::endl;
        int numLines = 5;
        for (int i = 0; i < numLines; ++i)
        {
            float yPos = (windowSize.y / (numLines + 1)) * (i + 1);
            sf::VertexArray line(sf::Lines, 2);
            sf::Vector2f startPos, endPos;
            float arrowSize = 10.f;

            if (std::abs(m_currentFields.electricField.x) > std::abs(m_currentFields.electricField.y))
            {
                startPos = {PADDING * 5, yPos};
                endPos = {windowSize.x - PADDING * 5, yPos};
                line[0].position = startPos;
                line[1].position = endPos;
                line[0].color = sf::Color::Cyan;
                line[1].color = sf::Color::Cyan;
                m_eFieldLines.push_back(line);

                sf::VertexArray arrowhead(sf::Triangles, 3);
                if (m_currentFields.electricField.x > 0)
                { // point right
                    arrowhead[0].position = {endPos.x, endPos.y};
                    arrowhead[1].position = {endPos.x - arrowSize, endPos.y - arrowSize / 2.f};
                    arrowhead[2].position = {endPos.x - arrowSize, endPos.y + arrowSize / 2.f};
                }
                else
                { // point left
                    arrowhead[0].position = {startPos.x, startPos.y};
                    arrowhead[1].position = {startPos.x + arrowSize, startPos.y - arrowSize / 2.f};
                    arrowhead[2].position = {startPos.x + arrowSize, startPos.y + arrowSize / 2.f};
                }
                arrowhead[0].color = sf::Color::Cyan;
                arrowhead[1].color = sf::Color::Cyan;
                arrowhead[2].color = sf::Color::Cyan;
                m_eFieldLines.push_back(arrowhead);
            }
        }
    }
}

void GameScene::spawnLaser()
{
    // std::cout << "sapwn laser. /GameScene.cpp" << std::endl;
    if (m_laserTexture.getSize().x == 0)
    {
        std::cout << "ERROR! m_laserTexture invalid!" << std::endl;
        return;
    }
    std::uniform_int_distribution<int> side_dist(0, 3); // 0:top, 1:bottom, 2:left, 3:right
    int side = side_dist(m_rng);

    sf::Vector2f laserSize;
    sf::Vector2f laserPos;
    sf::Vector2f laserVel;
    float laserSpeed = 150.f + (m_rng() % 200); // random speed
    float rotation = 0.f;
    sf::Vector2u winSize = m_game.getWindow().getSize();

    float scaledHeight = 15.f;

    switch (side)
    {
    case 0: // From Top
        // laserSize = sf::Vector2f(static_cast<float>(5 + m_rng() % 15), static_cast<float>(50 + m_rng() % 100));
        // laserPos = sf::Vector2f(static_cast<float>(m_rng() % winSize.x), -laserSize.y);
        // laserVel = sf::Vector2f(0, laserSpeed);
        laserPos = {static_cast<float>(m_rng() % winSize.x), -scaledHeight / 2.f};
        laserVel = {0, laserSpeed};
        rotation = 90.f;
        std::cout << "Spawned a laser from top." << std::endl;
        break;
    case 1: // From Bottom
        // laserSize = sf::Vector2f(static_cast<float>(5 + m_rng() % 15), static_cast<float>(50 + m_rng() % 100));
        // laserPos = sf::Vector2f(static_cast<float>(m_rng() % winSize.x), static_cast<float>(winSize.y));
        // laserVel = sf::Vector2f(0, -laserSpeed);
        laserPos = {static_cast<float>(m_rng() % winSize.x), static_cast<float>(winSize.y) + scaledHeight / 2.f};
        laserVel = {0, -laserSpeed};
        rotation = -90.f;
        std::cout << "Spawned a laser from bottom." << std::endl;
        break;
    case 2: // From Left
        // laserSize = sf::Vector2f(static_cast<float>(50 + m_rng() % 100), static_cast<float>(5 + m_rng() % 15));
        // laserPos = sf::Vector2f(-laserSize.x, static_cast<float>(m_rng() % winSize.y));
        // laserVel = sf::Vector2f(laserSpeed, 0);
        laserPos = {-scaledHeight / 2.f, static_cast<float>(m_rng() % winSize.y)};
        laserVel = {laserSpeed, 0};
        rotation = 0.f;
        std::cout << "Spawned a laser from left." << std::endl;
        break;
    case 3: // From Right
        // laserSize = sf::Vector2f(static_cast<float>(50 + m_rng() % 100), static_cast<float>(5 + m_rng() % 15));
        // laserPos = sf::Vector2f(static_cast<float>(winSize.x), static_cast<float>(m_rng() % winSize.y));
        // laserVel = sf::Vector2f(-laserSpeed, 0);
        laserPos = {static_cast<float>(winSize.x) + scaledHeight / 2.f, static_cast<float>(m_rng() % winSize.y)};
        laserVel = {-laserSpeed, 0};
        rotation = 180.f;
        std::cout << "Spawned a laser from right." << std::endl;
        break;
    }
    m_lasers.emplace_back(m_laserTexture, laserPos, laserVel, rotation);

    m_laserSound.play();

    if (m_rng() % 3 == 0)
    { // 1/3 change f
        randomizeFields();
    }
}

void GameScene::updateLasers(sf::Time deltaTime)
{
    sf::Vector2u winSize = m_game.getWindow().getSize();
    for (auto &laser : m_lasers)
    {
        if (laser.isActive)
        {
            laser.update(deltaTime);
            sf::FloatRect bounds = laser.getBounds();
            if (bounds.left > winSize.x || bounds.left + bounds.width < 0 ||
                bounds.top > winSize.y || bounds.top + bounds.height < 0)
            {
                laser.isActive = false;
            }
        }
    }
    // remove laser
    m_lasers.erase(std::remove_if(m_lasers.begin(), m_lasers.end(),
                                  [](const Laser &l)
                                  { return !l.isActive; }),
                   m_lasers.end());
}

void GameScene::spawnScroll()
{
    if (m_game.getCollectedScrollsCount() >= m_game.getTotalScrolls())
        return; // all scrolls collected

    // ID that hasn't been collected yet and isn't currently in scene
    std::vector<int> availableScrollIds;
    for (int i = 0; i < m_game.getTotalScrolls(); ++i)
    {
        if (!m_game.isScrollCollected(i))
        {
            bool alreadyInScene = false;
            for (const auto &sc : m_scrollsInScene)
            {
                if (sc.id == i)
                {
                    alreadyInScene = true;
                    break;
                }
            }
            if (!alreadyInScene)
            {
                availableScrollIds.push_back(i);
            }
        }
    }

    if (availableScrollIds.empty())
        return;

    std::uniform_int_distribution<int> id_dist(0, availableScrollIds.size() - 1);
    int scrollIdToSpawn = availableScrollIds[id_dist(m_rng)];

    sf::Vector2u winSize = m_game.getWindow().getSize();
    float spawnY = static_cast<float>(m_rng() % (winSize.y - 100) + 50);
    sf::Vector2f spawnPos = {static_cast<float>(winSize.x) + 50.f, spawnY};

    m_scrollsInScene.emplace_back(scrollIdToSpawn, m_scrollItemTexture, spawnPos);
    std::cout << "Spawned scroll ID: " << scrollIdToSpawn << std::endl;
}

void GameScene::updateScrolls(sf::Time deltaTime)
{
    float moveDistance = m_bgScrollSpeed * deltaTime.asSeconds();

    for (auto &scroll : m_scrollsInScene)
    {
        if (scroll.isActive)
        {
            scroll.sprite.move(-moveDistance, 0);
            // check collection
            if (scroll.getBounds().intersects(m_player->getBounds()))
            {
                m_game.collectScroll(scroll.id);
                scroll.isActive = false;
            }
            if (scroll.sprite.getPosition().x + scroll.getBounds().width < 0)
            {
                scroll.isActive = false;
            }
        }
    }
    // remove scrolls
    m_scrollsInScene.erase(std::remove_if(m_scrollsInScene.begin(), m_scrollsInScene.end(),
                                          [](const ScrollItem &s)
                                          { return !s.isActive; }),
                           m_scrollsInScene.end());
}

void GameScene::updateHUD()
{
    if (!m_player)
        return;
    m_distanceText.setString("Distance: " + std::to_string(static_cast<int>(m_distanceTraveled)));

    std::string chargeStr = "Charge: " + std::to_string(m_player->getCharge());
    char buffer[10];
    std::snprintf(buffer, sizeof(buffer), "%.1f", m_player->getCharge());
    m_chargeText.setString("Charge: " + std::string(buffer));

    m_dashChargesText.setString("Dash: " + std::to_string(m_player->getDashCharges()));
}

void GameScene::render(sf::RenderWindow &window)
{
    window.clear(sf::Color(10, 10, 20));

    // Background
    window.draw(m_backgroundSprite1);
    window.draw(m_backgroundSprite2);

    // Field Visual
    for (const auto &line : m_eFieldLines)
    {
        window.draw(line);
    }
    // if (!m_eFieldPositiveText.getString().isEmpty()) window.draw(m_eFieldPositiveText);
    // if (!m_eFieldNegativeText.getString().isEmpty()) window.draw(m_eFieldNegativeText);

    for (const auto &symbol : m_bFieldSymbols)
    {
        window.draw(symbol);
    }

    // scrolls
    for (const auto &scroll : m_scrollsInScene)
    {
        if (scroll.isActive)
        {
            window.draw(scroll.sprite);
        }
    }

    // player
    m_player->render(window);

    // laser
    for (const auto &laser : m_lasers)
    {
        if (laser.isActive)
        {
            window.draw(laser.sprite);
        }
    }
    // window.draw(m_bottomLaser);

    // HUD
    window.draw(m_distanceText);
    window.draw(m_chargeText);
    window.draw(m_dashChargesText);
}

void GameScene::onVolumeChanged()
{
    if (m_gameMusic.getStatus() == sf::Music::Playing || m_gameMusic.getStatus() == sf::Music::Paused)
    {
        m_gameMusic.setVolume(m_game.getMasterVolume() * 0.7f);
    }
    m_laserSound.setVolume(m_game.getMasterVolume() * 0.5f);
}