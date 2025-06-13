// src/core/Game.cpp
#include "Game.h"
#include "../scene/MenuScene.h"
#include "../scene/GameScene.h"
#include "../render/ResourceManager.h" // For default font
#include <iostream> // For std::cout, std::cerr

Game::Game(unsigned int width, unsigned int height, const std::string& title)
    : m_window(sf::VideoMode(width, height), title),
      m_currentState(GameState::MainMenu),
      m_collectedScrolls(m_totalScrolls, false) {
    std::cout << "initiating Game./ Game.cpp" << std::endl;
    m_window.setVerticalSyncEnabled(true);
    // m_window.setFramerateLimit(60); // Optional

    // Initialize scroll contents (placeholders)
    // YOU NEED TO FILL THESE IN
    m_scrollContents.resize(m_totalScrolls);
    
    m_scrollContents[0] = "...core resonance frequency spiking... can't contain the feedback loop! \n They called it clean energy... fools.\n The Grid wasn't just powered by RET... it was RET. When it went critical... \n... not an explosion... a unraveling. Reality flickered.\n Now the energy's... alive. Angry. We built a god... and it hates its cage. \nnPray your insulation holds... or that you change enough to dance with the lightning";
    m_scrollContents[1] = "Saw a kid runnin' thru Shocker Alley. Not dodgin' the beams... weavin' with 'em. \n Skin glowin' blue. CorpSec zappers? Just made him shine brighter. \n Heard whispers: 'Flicker-born'. Got the juice in his blood. \n RET ain't just killin' us no more... it's makin' new us. This DenPaKid? He ain't hiding.\n  He's talking to the storm. Madness? Or the only way left? \n ---Tag: SPARKS";
    m_scrollContents[2] = "RET Fields aren't random! (Mostly). Look for patterns: \n Charge Gradients = potential paths (like hills).\n  Opposing Polarity Nodes = repulsion 'jump pads' (if timed!). \n Magnetic Flux Lines = guide rails (ride 'em, don't fight 'em!). \nLasers? Coherent EM discharge - pure death. \n Avoid OR disrupt field coherence briefly (massive local charge dump?). \n Theory: A strong enough bio-EM pulse could create momentary safe zones...\n  or attract every loose electron in the sector. \n High risk. Higher reward. - Eng. Voss";
    m_scrollContents[3] = "Traded for synth-protein down in the Drain Pipes. \n Whole sector humming like a pissed-off transformer.\n  Glow-moss on the walls writhes to the current.\n  Kids play 'Dare the Dipole' - idiots. CorpSec patrols in Faraday-suits zap anything that moves... \n or glows wrong. Why run the Fields? Only place left the Corps don't fully control.\n  Only place with pure data-scrolls, uncorrupted by their filters. \n Find the scrolls, find the truth of the Flicker. \n Find a way out... or a way to turn their god against 'em";
    m_scrollContents[4] = "Asset recovery from RET Field Zones remains Priority Alpha. \n Initial 'Flicker-Born' test subjects (Ref: DenPa-Type) show unprecedented EM harmonization. Hypothesis: \n Their unique bio-signature could stabilize critical RET sectors...\n  or safely access the Central Core Archive (CCA). \n The CCA holds the original RET stabilization protocols. \n Possessing it = Absolute control over the Grid's energy... and its survivors. \n Terminate wild variants on sight. Capture viable harmonizers. \n The future is Electromagnetic. \n The future is OURS. - Director Kael";
    

    try {
        m_font = ResourceManager::getInstance().getDefaultFont(); // Load default font for Game class messages
        std::cout << "loading default font./ Game.cpp" << std::endl;
        m_winText.setFont(m_font);
        m_winText.setCharacterSize(20);
        m_winText.setFillColor(sf::Color::White);
        m_winText.setString("The fragments coalesce. You know the Grid's poisoned heart.\n  You know the Corps' desperate greed. You know the storm in your own veins. \n The path ahead thrums with lethal energy a jagged highway forged from the world's collapse.\n  But you are DenPaKid. The Flicker's echo. The spark in the static. \n\n The Central Core Archive awaits, buried deep within the most violent Fields.\n The truth of the past. The key to the future. \n Will you be a weapon? A savior? Or just another surge fading into the noise? Run.\n\n  Adapt. Resonate. The Grid's fate crackles in your wake.");
        sf::FloatRect textRect = m_winText.getLocalBounds();
        m_winText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
        m_winText.setPosition(width/2.0f, height/2.0f);

        m_deathScrollText.setFont(m_font);
        m_deathScrollText.setCharacterSize(24);
        m_deathScrollText.setFillColor(sf::Color::White);
        // Position will be set when displaying

    } catch (const std::runtime_error& e) {
        std::cerr << "Error initializing Game: " << e.what() << std::endl;
        // Game might not be able to display text messages if font fails
    }


    changeScene(GameState::MainMenu);
}

Game::~Game() {
    // Smart pointers handle scene deletion
}

void Game::run() {
    sf::Clock clock;
    while (m_window.isOpen() && !m_exitGame) {
        sf::Time deltaTime = clock.restart();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        }
        if (m_currentScene) {
            m_currentScene->handleInput(event, m_window);
        }
        if (m_currentState == GameState::GameWon && event.type == sf::Event::KeyPressed) {
             m_exitGame = true; // Or return to menu
        }
        if (m_currentState == GameState::GameOver && lastCollectedScrollId != -1 && event.type == sf::Event::KeyPressed) {
            lastCollectedScrollId = -1; // Acknowledge scroll view
            changeScene(GameState::MainMenu); // Return to menu after viewing scroll
        } else if (m_currentState == GameState::GameOver && lastCollectedScrollId == -1 && event.type == sf::Event::KeyPressed) {
            changeScene(GameState::MainMenu); // Return to menu if no scroll to view
        }
    }
}

void Game::update(sf::Time deltaTime) {
    if (m_currentScene && (m_currentState != GameState::GameWon && m_currentState != GameState::GameOver)) {
        m_currentScene->update(deltaTime);
    }
}

void Game::render() {
    m_window.clear(sf::Color::Black); // Default clear color
    if (m_currentScene && (m_currentState != GameState::GameWon && m_currentState != GameState::GameOver)) {
        m_currentScene->render(m_window);
    } else if (m_currentState == GameState::GameWon) {
        m_window.draw(m_winText);
    } else if (m_currentState == GameState::GameOver) {
        m_window.draw(m_deathScrollText);
    }
    m_window.display();
}

void Game::changeScene(GameState newState) {
    m_currentState = newState;
    lastCollectedScrollId = -1; // Reset last collected scroll on scene change
    m_gameWonMessageDisplayed = false;

    std::cout << "Changing scene to: " << static_cast<int>(newState) << std::endl;

    if (m_currentState == GameState::MainMenu) {
        m_currentScene = std::make_unique<MenuScene>(*this);
    } else if (m_currentState == GameState::Playing) {
        m_currentScene = std::make_unique<GameScene>(*this);
    } else if (m_currentState == GameState::GameOver) {
        // No new scene, just stops updates on GameScene and Game loop handles message/return
        // If a scroll was just collected and led to death, prepare its display
        // This logic is a bit intertwined; playerDied might set a flag or the scroll ID
    } else if (m_currentState == GameState::GameWon) {
        // No new scene, Game loop handles win message
    }
    // Ensure the new scene's assets are loaded if not pre-loaded
    if (m_currentScene) {
        m_currentScene->loadAssets();
    }
}

void Game::setMasterVolume(float volume) {
    m_masterVolume = std::max(0.0f, std::min(100.0f, volume));
    // Scenes will need to query this and set their own music/sound volumes
    if (m_currentScene) {
        m_currentScene->onVolumeChanged();
    }
    std::cout << "Master volume set to: " << m_masterVolume << std::endl;
}

void Game::collectScroll(int scrollId) {
    if (scrollId >= 0 && scrollId < m_totalScrolls) {
        if (!m_collectedScrolls[scrollId]) {
            m_collectedScrolls[scrollId] = true;
            lastCollectedScrollId = scrollId; // Remember this for display if player dies soon
            std::cout << "Collected scroll: " << scrollId << std::endl;
        }
    }
}

bool Game::isScrollCollected(int scrollId) const {
    if (scrollId >= 0 && scrollId < m_totalScrolls) {
        return m_collectedScrolls[scrollId];
    }
    return false;
}

const std::string& Game::getScrollContent(int scrollId) const {
    if (scrollId >= 0 && scrollId < m_scrollContents.size()) {
        return m_scrollContents[scrollId];
    }
    static const std::string empty = "Scroll content not found.";
    return empty;
}

int Game::getCollectedScrollsCount() const {
    int count = 0;
    for (bool collected : m_collectedScrolls) {
        if (collected) {
            count++;
        }
    }
    return count;
}

void Game::playerDied(float distance) {
    std::cout << "Player died. Distance: " << distance << std::endl;
    m_currentState = GameState::GameOver;
    // If a scroll was collected *just now* (lastCollectedScrollId is set)
    // prepare to display it.
    if (lastCollectedScrollId != -1) {
        std::cout << "died, preparing to show scroll./ Game.cpp" << std::endl;
        m_deathScrollText.setString("You perished...\nBut you found a scroll:\n\n" + getScrollContent(lastCollectedScrollId) + "\n\nPress any key to continue.");
        sf::FloatRect bounds = m_deathScrollText.getLocalBounds();
        m_deathScrollText.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
        m_deathScrollText.setPosition(m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f);
    } else {
         m_deathScrollText.setString("You perished...\n\nPress any key to return to menu.");
        sf::FloatRect bounds = m_deathScrollText.getLocalBounds();
        m_deathScrollText.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
        m_deathScrollText.setPosition(m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f);
    }
    // The game loop's event processing will handle returning to menu on key press
}

void Game::checkWinCondition(float distance) {
    if (getCollectedScrollsCount() == m_totalScrolls && distance >= m_requiredDistanceForWin) {
        if (!m_gameWonMessageDisplayed) {
            std::cout << "Win condition met!" << std::endl;
            m_currentState = GameState::GameWon;
            m_gameWonMessageDisplayed = true;
            if (m_currentScene && m_currentScene->getMusic()) { // Stop game music
                m_currentScene->getMusic()->stop();
            }
        }
    }
}