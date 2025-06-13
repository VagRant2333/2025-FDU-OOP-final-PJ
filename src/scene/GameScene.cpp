// src/scene/GameScene.cpp
#include "GameScene.h"
#include "../render/ResourceManager.h"
#include <iostream>
#include <string> // For std::to_string
#include <algorithm> // For std::remove_if

const float PADDING = 10.f; // Define PADDING for use in this file
const float PLAYER_START_Y_OFFSET = -100.f;  // Start slightly above center vertically
const float BOTTOM_LASER_HEIGHT = 10.f;

GameScene::GameScene(Game& game)
    : Scene(game),
      // Player needs a texture. Load it in loadAssets and pass it.
      // Temporary dummy texture for player initialization before loadAssets.
      // This will be replaced in loadAssets.
      m_playerTexture(), // Will be loaded
      // m_player({static_cast<float>(game.getWindow().getSize().x) / 2.f, 
      //          static_cast<float>(game.getWindow().getSize().y) / 2.f + PLAYER_START_Y_OFFSET},
      //          m_playerTexture), // Pass a reference, texture loaded in loadAssets()
      m_rng(std::random_device{}()) 
{
    std::cout << "GameScene created. /GameScene.cpp" << std::endl;
    // loadAssets() will be called by Game
}

GameScene::~GameScene() {
    if (m_gameMusic.getStatus() == sf::Music::Playing) {
        m_gameMusic.stop();
    }
    std::cout << "GameScene destroyed." << std::endl;
}

void GameScene::loadAssets() {
    std::cout << "GameScene loading assets. /GameScene.cpp" << std::endl;
    try {
        // Player Texture
        // ASSET_PATH: e.g., "assets/images/player_sprite.png" - User needs to provide this!
        // For now, using a colored rectangle if texture load fails or is missing.
        m_hudFont = ResourceManager::getInstance().getDefaultFont();
        m_playerTexture = ResourceManager::getInstance().loadTexture("player", "../../assets/images/player_sprite.png");

        sf::Vector2f playerStartPos = {
            static_cast<float>(m_game.getWindow().getSize().x) / 5.f, // Adjusted starting X
            static_cast<float>(m_game.getWindow().getSize().y) / 2.f
        };
        // Create the player object HERE
        m_player = std::make_unique<Player>(playerStartPos, m_playerTexture);
        m_player->setCharge(1.0f); 
        m_player->setVelocity({100.f, 0.f}); 

        // Background
        // ASSET_PATH
        m_gameBgTexture = ResourceManager::getInstance().loadTexture("game_bg", "../../assets/images/game_bg.png"); 
        m_backgroundSprite1.setTexture(m_gameBgTexture);
        m_backgroundSprite2.setTexture(m_gameBgTexture);
        

        // Ensure texture repeats or is wide enough. For scrolling, two sprites are common.
        m_backgroundSprite1.setPosition(0, 0);
        m_backgroundSprite2.setPosition(m_gameBgTexture.getSize().x, 0); // Position second sprite after the first
         // Scale to fit height, width will scroll
        float bgScaleY = static_cast<float>(m_game.getWindow().getSize().y) / m_gameBgTexture.getSize().y;
        m_backgroundSprite1.setScale(bgScaleY, bgScaleY); // Assuming square pixels in texture, scale uniformly based on height
        m_backgroundSprite2.setScale(bgScaleY, bgScaleY);
        // Adjust scroll speed based on scaled width
        float scaledBgWidth = m_gameBgTexture.getSize().x * m_backgroundSprite1.getScale().x;
        m_backgroundSprite1.setPosition(0, 0);
        m_backgroundSprite2.setPosition(scaledBgWidth, 0);

        m_bgScrollSpeed = 150.f;


        // Music
        // ASSET_PATH
        if (!m_gameMusic.openFromFile("../../assets/audio/game_theme.ogg")) {
            throw std::runtime_error("Failed to load game_theme.ogg");
        }
        m_gameMusic.setLoop(true);
        m_gameMusic.setVolume(m_game.getMasterVolume() * 0.7f); // Game music slightly quieter
        m_gameMusic.play();

        // Laser sound
        // ASSET_PATH
        m_laserSound.setBuffer(ResourceManager::getInstance().loadSoundBuffer("laser_fire", "../../assets/audio/laser_sound.ogg"));
        m_laserSound.setVolume(m_game.getMasterVolume() * 0.5f);

        // Scroll item texture
        // ASSET_PATH
        m_scrollItemTexture = ResourceManager::getInstance().loadTexture("scroll_item", "../../assets/images/scroll_item.png"); // CREATE THIS

        // Laser texture (optional, currently using shapes)
        // m_laserTexture = ResourceManager::getInstance().loadTexture("laser_beam", "assets/images/laser.png"); // CREATE THIS
        m_laserTexture = ResourceManager::getInstance().loadTexture("laser_beam", "../../assets/images/laser.png");

    } catch (const std::runtime_error& e) {
        std::cerr << "Error loading assets in GameScene: " << e.what() << std::endl;
    }
    setupInitialState();
    std::cout << "GameScene assets loaded." << std::endl;
}

void GameScene::setupInitialState() {
    std::cout << "set up initial state. /GameScene.cpp" << std::endl;
    m_isGameOver = false;
    m_distanceTraveled = 0.f;
    if (m_player) { // check if unique_ptr is valid
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

    // // TODO

    // m_eFieldNegativeText.setFont(m_hudFont);
    // m_eFieldNegativeText.setCharacterSize(20);
    // m_eFieldNegativeText.setFillColor(sf::Color::Blue);
    // m_eFieldNegativeText.setString("- - - E - - -");
    
    // Initial field randomization
    randomizeFields();
    updateFieldVisuals();

    m_laserSpawnTimer = sf::Time::Zero;
    m_scrollSpawnTimer = sf::Time::Zero; // Start spawning scrolls after some delay
    m_timeBetweenScrollSpawns = sf::seconds(10.f + (m_rng() % 10)); // 10-19s
    m_lasers.clear();
    m_scrollsInScene.clear();

    m_physicsEngine.setPlayerMass(1.0f); // Set player mass for physics calculations
}

void GameScene::handleInput(sf::Event& event, sf::RenderWindow& window) {
    if (m_isGameOver) return;

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Q) {
            m_player->decreaseCharge();
        } else if (event.key.code == sf::Keyboard::E) {
            m_player->increaseCharge();
        } else if (event.key.code == sf::Keyboard::Space) {
            m_player->toggleChargeSign();
        } else if (event.key.code == sf::Keyboard::W) {
            m_player->dash({0.f, -1.f});
        } else if (event.key.code == sf::Keyboard::A) {
            m_player->dash({-1.f, 0.f});
        } else if (event.key.code == sf::Keyboard::S) {
            m_player->dash({0.f, 1.f});
        } else if (event.key.code == sf::Keyboard::D) {
            m_player->dash({1.f, 0.f});
        }
        // Debug: Randomize fields
        // else if (event.key.code == sf::Keyboard::R) {
        //     randomizeFields();
        // }
    }
}

void GameScene::update(sf::Time deltaTime) {
    if (m_isGameOver) return;

    m_physicsEngine.updatePlayer(*m_player, deltaTime, m_currentFields, m_game.getWindow().getSize());
    m_player->update(deltaTime); // For any internal player logic

    updateBackground(deltaTime);
    m_distanceTraveled += m_bgScrollSpeed * deltaTime.asSeconds() * 0.1f; // Scale distance units

    // Spawning logic
    m_laserSpawnTimer += deltaTime;
    if (m_laserSpawnTimer >= m_timeBetweenLaserSpawns) {
        spawnLaser();
        m_laserSpawnTimer = sf::Time::Zero;
        // Randomize next spawn time slightly
        std::uniform_real_distribution<float> dist(1.5f, 4.0f);
        m_timeBetweenLaserSpawns = sf::seconds(dist(m_rng));
    }

    m_scrollSpawnTimer += deltaTime;
    if (m_scrollsInScene.size() < m_maxScrollsOnScreen && m_game.getCollectedScrollsCount() < m_game.getTotalScrolls()) {
        if (m_scrollSpawnTimer >= m_timeBetweenScrollSpawns) {
            spawnScroll();
            m_scrollSpawnTimer = sf::Time::Zero;
            std::uniform_real_distribution<float> dist(8.f, 15.f); // Randomize next scroll spawn
            m_timeBetweenScrollSpawns = sf::seconds(dist(m_rng));
        }
    }


    updateLasers(deltaTime);
    updateScrolls(deltaTime);
    updateHUD();
    updateFieldVisuals(); // Update visuals if fields change dynamically (they do via randomizeFields)

    // Check for game over conditions
    // if (m_player->getBounds().intersects(m_bottomLaser.getGlobalBounds())) {
    //     m_isGameOver = true;
    // }
    for (const auto& laser : m_lasers) {
        if (laser.isActive && m_player->getBounds().intersects(laser.getBounds())) {
            m_isGameOver = true;
            break;
        }
    }
    // Player off screen top/bottom too much? (Bottom handled by laser)
    if (m_player->getPosition().y < -m_player->getBounds().height) { // Gone too high
         m_isGameOver = true; // Or push back down
    }


    if (m_isGameOver) {
        m_gameMusic.stop();
        m_game.playerDied(m_distanceTraveled); // Game class handles what to do on death
    } else {
        // Check win condition only if not game over
        m_game.checkWinCondition(m_distanceTraveled);
        if (m_game.getCurrentState() == GameState::GameWon) { // If game signals win
            m_isGameOver = true; // Stop scene updates, Game class will show win message
        }
    }
}

void GameScene::updateBackground(sf::Time deltaTime) {
    std::cout << "GameScene upadate background. /GameScene.cpp" << std::endl;
    float moveDistance = m_bgScrollSpeed * deltaTime.asSeconds();
    m_backgroundSprite1.move(-moveDistance, 0);
    m_backgroundSprite2.move(-moveDistance, 0);

    float scaledBgWidth = m_gameBgTexture.getSize().x * m_backgroundSprite1.getScale().x;

    if (m_backgroundSprite1.getPosition().x + scaledBgWidth <= 0) {
        m_backgroundSprite1.setPosition(m_backgroundSprite2.getPosition().x + scaledBgWidth, 0);
    }
    if (m_backgroundSprite2.getPosition().x + scaledBgWidth <= 0) {
        m_backgroundSprite2.setPosition(m_backgroundSprite1.getPosition().x + scaledBgWidth, 0);
    }
}

void GameScene::randomizeFields() {
    std::cout << "GameScene randomize fields.. /GameScene.cpp" << std::endl;
    std::uniform_real_distribution<float> e_dist(-50.f, 50.f); // Electric field strength range
    std::uniform_real_distribution<float> b_dist(-2.f, 2.f);   // Magnetic field strength range
    std::uniform_int_distribution<int> direction_dist(0, 1); // For E field direction

    if (direction_dist(m_rng) == 0) { // Horizontal E field
        // m_currentFields.electricField = {e_dist(m_rng), 0.f};
        m_currentFields.electricField = sf::Vector2f(e_dist(m_rng), 0.f); // Use constructor
    } else { // Vertical E field (less common for side scroller, but possible)
        // m_currentFields.electricField = {0.f, e_dist(m_rng) / 2.f}; // Weaker vertical field
        m_currentFields.electricField = sf::Vector2f(0.f, e_dist(m_rng) / 2.f);
    }
    m_currentFields.magneticField_Z = b_dist(m_rng);

    // Periodically change fields during gameplay too, not just at start
    // This can be tied to a timer. For now, called when spawning laser or on a separate timer.
    std::cout << "Fields randomized: E(" << m_currentFields.electricField.x << "," << m_currentFields.electricField.y 
              << "), B(" << m_currentFields.magneticField_Z << ")" << std::endl;
}


void GameScene::updateFieldVisuals() {
    std::cout << "GameScene filed visializing. /GameScene.cpp" << std::endl;
    sf::Vector2u windowSize = m_game.getWindow().getSize();
    
    // E-Field indicators (side texts)
    // std::cout << "GameScene E filed visializing. /GameScene.cpp" << std::endl;
    // if (m_currentFields.electricField.x > 0) { // Points right
    //     m_eFieldPositiveText.setPosition(windowSize.x - m_eFieldPositiveText.getLocalBounds().width - PADDING, PADDING * 5);
    //     m_eFieldNegativeText.setPosition(PADDING, PADDING * 5);
    // } else if (m_currentFields.electricField.x < 0) { // Points left
    //     m_eFieldPositiveText.setPosition(PADDING, PADDING * 5);
    //     m_eFieldNegativeText.setPosition(windowSize.x - m_eFieldNegativeText.getLocalBounds().width - PADDING, PADDING * 5);
    // } else { // No horizontal E-field text
    //     m_eFieldPositiveText.setString("");
    //     m_eFieldNegativeText.setString("");
    // }
    //  // Restore string if it was cleared
    // if (m_eFieldPositiveText.getString().isEmpty()) m_eFieldPositiveText.setString("+ + + E + + +");
    // if (m_eFieldNegativeText.getString().isEmpty()) m_eFieldNegativeText.setString("- - - E - - -");


    // B-Field indicators (dots/crosses)
    std::cout << "GameScene B field visialing. /GameScene.cpp" << std::endl;
    m_bFieldSymbols.clear();
    
    // if (std::abs(m_currentFields.magneticField_Z) > 0.05f) { // Only draw if B is significant
    //     std::cout << "B field drawing dots/points. /GameScene.cpp" << std::endl;
    //     int numSymbols = 20; // Number of symbols to draw
    //     float radius = (m_currentFields.magneticField_Z > 0) ? 3.f : 5.f; // Dot (out) smaller, Cross (in) line based
    //     sf::Color bColor = (m_currentFields.magneticField_Z > 0) ? sf::Color(200,200,255,150) : sf::Color(100,100,150,150);

    //     for (int i = 0; i < numSymbols; ++i) {
    //         std::uniform_real_distribution<float> x_dist(0, windowSize.x);
    //         std::uniform_real_distribution<float> y_dist(0, windowSize.y);
    //         sf::Vector2f pos = {x_dist(m_rng), y_dist(m_rng)};
            
    //         if (m_currentFields.magneticField_Z > 0) { // Out of screen (dot)
    //             sf::CircleShape dot(radius);
    //             dot.setFillColor(bColor);
    //             dot.setOrigin(radius, radius);
    //             dot.setPosition(pos);
    //             m_bFieldSymbols.push_back(dot);
    //         } else { // Into screen (cross) - represented by two crossed lines or a circle with X
    //             sf::CircleShape cross_bg(radius * 1.2f); // A background circle for the X
    //             cross_bg.setOutlineThickness(1.f);
    //             cross_bg.setOutlineColor(bColor);
    //             cross_bg.setFillColor(sf::Color::Transparent);
    //             cross_bg.setOrigin(radius*1.2f, radius*1.2f);
    //             cross_bg.setPosition(pos);
    //             m_bFieldSymbols.push_back(cross_bg); // This will draw a circle, you'd need lines for a proper X
    //             // For a simple X, could also use sf::Text with "X"
    //         }
    //     }
    // }
    if (std::abs(m_currentFields.magneticField_Z) > 0.05f) { // Only draw if B is somewhat significant
        // Calculate number of symbols based on field strength
        int numSymbolsTarget = static_cast<int>(std::abs(m_currentFields.magneticField_Z) * B_FIELD_DENSITY_SCALE_FACTOR);

        // Determine grid dimensions (roughly square root)
        int symbolsPerRow = static_cast<int>(std::sqrt(numSymbolsTarget));
        symbolsPerRow = std::min(symbolsPerRow, MAX_B_FIELD_SYMBOLS_PER_ROW_COL); // Cap symbols per row
        symbolsPerRow = std::max(1, symbolsPerRow); // Ensure at least 1 if target > 0
        int symbolsPerCol = symbolsPerRow; // Keep it grid-like

        if (symbolsPerRow == 0 && numSymbolsTarget > 0) symbolsPerRow = 1; // ensure at least one if field exists
        if (symbolsPerCol == 0 && numSymbolsTarget > 0) symbolsPerCol = 1;


        float spacingX = static_cast<float>(windowSize.x) / (symbolsPerRow + 1);
        float spacingY = static_cast<float>(windowSize.y) / (symbolsPerCol + 1);

        sf::Color bColor = (m_currentFields.magneticField_Z > 0) ? sf::Color(200,200,255,150) : sf::Color(100,100,150,150);
        std::string symbolChar = (m_currentFields.magneticField_Z > 0) ? "." : "X";
        unsigned int charSize = (m_currentFields.magneticField_Z > 0) ? 30 : 20; // Dots bigger, X smaller

        for (int r = 0; r < symbolsPerCol; ++r) {
            for (int c = 0; c < symbolsPerRow; ++c) {
                sf::Text bSymbol;
                bSymbol.setFont(m_hudFont); // Make sure m_hudFont is loaded
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

    // E-Field lines (arrows) - simplified
    m_eFieldLines.clear();
    if (std::abs(m_currentFields.electricField.x) > 1.0f || std::abs(m_currentFields.electricField.y) > 1.0f) {
        std::cout << "drawing E field arrows. /GameScene.cpp" << std::endl;
        int numLines = 5;
        for (int i = 0; i < numLines; ++i) {
            float yPos = (windowSize.y / (numLines + 1)) * (i + 1);
            sf::VertexArray line(sf::Lines, 2);
            sf::Vector2f startPos, endPos;
            float arrowSize = 10.f;

            if (std::abs(m_currentFields.electricField.x) > std::abs(m_currentFields.electricField.y)) { // Primarily horizontal
                startPos = {PADDING * 5, yPos};
                endPos = {windowSize.x - PADDING * 5, yPos};
                line[0].position = startPos;
                line[1].position = endPos;
                line[0].color = sf::Color::Cyan; line[1].color = sf::Color::Cyan;
                m_eFieldLines.push_back(line);

                // Arrowhead
                sf::VertexArray arrowhead(sf::Triangles, 3);
                if (m_currentFields.electricField.x > 0) { // Points right
                    arrowhead[0].position = {endPos.x, endPos.y};
                    arrowhead[1].position = {endPos.x - arrowSize, endPos.y - arrowSize / 2.f};
                    arrowhead[2].position = {endPos.x - arrowSize, endPos.y + arrowSize / 2.f};
                } else { // Points left
                    arrowhead[0].position = {startPos.x, startPos.y};
                    arrowhead[1].position = {startPos.x + arrowSize, startPos.y - arrowSize / 2.f};
                    arrowhead[2].position = {startPos.x + arrowSize, startPos.y + arrowSize / 2.f};
                }
                arrowhead[0].color = sf::Color::Cyan; arrowhead[1].color = sf::Color::Cyan; arrowhead[2].color = sf::Color::Cyan;
                m_eFieldLines.push_back(arrowhead);
            } // Vertical field lines can be added similarly if needed
        }
    }
}


void GameScene::spawnLaser() {
    std::cout << "sapwn laser. /GameScene.cpp" << std::endl;
    // ASSET_PATH: laser.png could be used here if m_laserTexture is loaded
    // For now, colored rectangles
    if (m_laserTexture.getSize().x == 0) return;
    std::uniform_int_distribution<int> side_dist(0, 3); // 0:top, 1:bottom (but we have death laser), 2:left, 3:right
    int side = side_dist(m_rng);

    sf::Vector2f laserSize;
    sf::Vector2f laserPos;
    sf::Vector2f laserVel;
    float laserSpeed = 150.f + (m_rng() % 200); // Random speed
    float rotation = 0.f; // laser points to right and is horizontal

    sf::Vector2u winSize = m_game.getWindow().getSize();

    switch(side) {
        case 0: // From Top
            // laserSize = sf::Vector2f(static_cast<float>(5 + m_rng() % 15), static_cast<float>(50 + m_rng() % 100));
            // laserPos = sf::Vector2f(static_cast<float>(m_rng() % winSize.x), -laserSize.y);
            // laserVel = sf::Vector2f(0, laserSpeed);
            laserPos = {static_cast<float>(m_rng() % winSize.x), -m_laserTexture.getSize().y / 2.f};
            laserVel = {0, laserSpeed};
            rotation = 90.f;
            break;
        case 1: // From Bottom
            // laserSize = sf::Vector2f(static_cast<float>(5 + m_rng() % 15), static_cast<float>(50 + m_rng() % 100));
            // laserPos = sf::Vector2f(static_cast<float>(m_rng() % winSize.x), static_cast<float>(winSize.y));
            // laserVel = sf::Vector2f(0, -laserSpeed);
            laserPos = {static_cast<float>(m_rng() % winSize.x), static_cast<float>(winSize.y) + m_laserTexture.getSize().y / 2.f};
            laserVel = {0, -laserSpeed};
            rotation = -90.f;
            break;
        case 2: // From Left
            // laserSize = sf::Vector2f(static_cast<float>(50 + m_rng() % 100), static_cast<float>(5 + m_rng() % 15));
            // laserPos = sf::Vector2f(-laserSize.x, static_cast<float>(m_rng() % winSize.y));
            // laserVel = sf::Vector2f(laserSpeed, 0);
            laserPos = {-m_laserTexture.getSize().x / 2.f, static_cast<float>(m_rng() % winSize.y)};
            laserVel = {laserSpeed, 0};
            rotation = 0.f;
            break;
        case 3: // From Right
            // laserSize = sf::Vector2f(static_cast<float>(50 + m_rng() % 100), static_cast<float>(5 + m_rng() % 15));
            // laserPos = sf::Vector2f(static_cast<float>(winSize.x), static_cast<float>(m_rng() % winSize.y));
            // laserVel = sf::Vector2f(-laserSpeed, 0);
            laserPos = {static_cast<float>(winSize.x) + m_laserTexture.getSize().x / 2.f, static_cast<float>(m_rng() % winSize.y)};
            laserVel = {-laserSpeed, 0};
            rotation = 180.f; 
            break;
    }
    m_lasers.emplace_back(m_laserTexture, laserPos, laserVel, rotation);
    m_laserSound.play();

    // After spawning a laser, good time to change fields too for more dynamic gameplay
    if (m_rng() % 3 == 0) { // 1 in 3 chance to change fields with laser
        randomizeFields();
    }
}

void GameScene::updateLasers(sf::Time deltaTime) {
    sf::Vector2u winSize = m_game.getWindow().getSize();
    for (auto& laser : m_lasers) {
        if (laser.isActive) {
            laser.update(deltaTime);
            // Deactivate if off-screen
            sf::FloatRect bounds = laser.getBounds();
            if (bounds.left > winSize.x || bounds.left + bounds.width < 0 ||
                bounds.top > winSize.y || bounds.top + bounds.height < 0) {
                laser.isActive = false;
            }
        }
    }
    // Remove inactive lasers
    m_lasers.erase(std::remove_if(m_lasers.begin(), m_lasers.end(), 
                                  [](const Laser& l){ return !l.isActive; }), 
                   m_lasers.end());
}

void GameScene::spawnScroll() {
    if (m_game.getCollectedScrollsCount() >= m_game.getTotalScrolls()) return; // All scrolls collected

    // Find a scroll ID that hasn't been collected yet and isn't currently in scene
    std::vector<int> availableScrollIds;
    for (int i = 0; i < m_game.getTotalScrolls(); ++i) {
        if (!m_game.isScrollCollected(i)) {
            bool alreadyInScene = false;
            for(const auto& sc : m_scrollsInScene) {
                if (sc.id == i) {
                    alreadyInScene = true;
                    break;
                }
            }
            if (!alreadyInScene) {
                 availableScrollIds.push_back(i);
            }
        }
    }

    if (availableScrollIds.empty()) return; // No available scrolls to spawn

    std::uniform_int_distribution<int> id_dist(0, availableScrollIds.size() - 1);
    int scrollIdToSpawn = availableScrollIds[id_dist(m_rng)];

    sf::Vector2u winSize = m_game.getWindow().getSize();
    // Spawn on the right edge, moving left with background
    float spawnY = static_cast<float>(m_rng() % (winSize.y - 100) + 50); // Random Y, avoid edges
    sf::Vector2f spawnPos = {static_cast<float>(winSize.x) + 50.f, spawnY};
    
    m_scrollsInScene.emplace_back(scrollIdToSpawn, m_scrollItemTexture, spawnPos);
    std::cout << "Spawned scroll ID: " << scrollIdToSpawn << std::endl;
}


void GameScene::updateScrolls(sf::Time deltaTime) {
    // Scrolls move left with the background
    float moveDistance = m_bgScrollSpeed * deltaTime.asSeconds();

    for (auto& scroll : m_scrollsInScene) {
        if (scroll.isActive) {
            scroll.sprite.move(-moveDistance, 0);
            // Check collection
            if (scroll.getBounds().intersects(m_player->getBounds())) {
                m_game.collectScroll(scroll.id);
                scroll.isActive = false; // Collected
                // Potentially play a sound
            }
            // Deactivate if off-screen (left side)
            if (scroll.sprite.getPosition().x + scroll.getBounds().width < 0) {
                scroll.isActive = false;
            }
        }
    }
    // Remove inactive scrolls
    m_scrollsInScene.erase(std::remove_if(m_scrollsInScene.begin(), m_scrollsInScene.end(),
                                         [](const ScrollItem& s){ return !s.isActive; }),
                           m_scrollsInScene.end());
}


void GameScene::updateHUD() {
    if (!m_player) return;
    m_distanceText.setString("Distance: " + std::to_string(static_cast<int>(m_distanceTraveled)));
    
    std::string chargeStr = "Charge: " + std::to_string(m_player->getCharge());
    // Format charge to one decimal place
    char buffer[10];
    std::snprintf(buffer, sizeof(buffer), "%.1f", m_player->getCharge());
    m_chargeText.setString("Charge: " + std::string(buffer));

    m_dashChargesText.setString("Dash: " + std::to_string(m_player->getDashCharges()));
}

void GameScene::render(sf::RenderWindow& window) {
    window.clear(sf::Color(10, 10, 20)); // Dark blue/purple space color

    // Background
    window.draw(m_backgroundSprite1);
    window.draw(m_backgroundSprite2);

    // Field Visuals (draw before player/items)
    for (const auto& line : m_eFieldLines) {
        window.draw(line);
    }
    // if (!m_eFieldPositiveText.getString().isEmpty()) window.draw(m_eFieldPositiveText);
    // if (!m_eFieldNegativeText.getString().isEmpty()) window.draw(m_eFieldNegativeText);
    
    for (const auto& symbol : m_bFieldSymbols) {
        window.draw(symbol);
    }


    // Scrolls
    for (const auto& scroll : m_scrollsInScene) {
        if (scroll.isActive) {
            window.draw(scroll.sprite);
        }
    }

    // Player
    m_player->render(window);

    // Lasers
    for (const auto& laser : m_lasers) {
        if (laser.isActive) {
            window.draw(laser.sprite);
        }
    }
    // window.draw(m_bottomLaser);

    // HUD
    window.draw(m_distanceText);
    window.draw(m_chargeText);
    window.draw(m_dashChargesText);
}

void GameScene::onVolumeChanged() {
    if (m_gameMusic.getStatus() == sf::Music::Playing || m_gameMusic.getStatus() == sf::Music::Paused) {
        m_gameMusic.setVolume(m_game.getMasterVolume() * 0.7f);
    }
    m_laserSound.setVolume(m_game.getMasterVolume() * 0.5f);
    // Other sounds if any
}