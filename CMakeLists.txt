cmake_minimum_required(VERSION 3.28)

project(DenPaKid VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# download SFML
# Notice: includes and libs are in 3rd, typically you don't have to fetch by yourself

# include(FetchContent)
# FetchContent_Declare(
#     SFML
#     GIT_REPOSITORY https://github.com/SFML/SFML.git
#     GIT_TAG 2.6.x
# )
# FetchContent_MakeAvailable(SFML)

# if you download the zip by yourself, use the following Fetch(remember to change the address)
# include(FetchContent)
# FetchContent_Declare(
#     SFML
#     URL file://${CMAKE_SOURCE_DIR}/3rd/SFML-2.6.0.zip
# )
# FetchContent_MakeAvailable(SFML)

add_executable(DenPaKid
    src/main.cpp
    src/core/Game.cpp
    src/entities/Entity.cpp
    src/entities/Player.cpp
    src/scene/GameScene.cpp
    src/scene/MenuScene.cpp
    src/ui/UIManager.cpp
    src/physics/PhysicsEngine.cpp
)


target_include_directories(DenPaKid PRIVATE
    ${CMAKE_SOURCE_DIR}/3rd/include
    ${CMAKE_SOURCE_DIR}/core
    ${CMAKE_SOURCE_DIR}/scene
    ${CMAKE_SOURCE_DIR}/ui
    ${CMAKE_SOURCE_DIR}/render
    ${CMAKE_SOURCE_DIR}/physics
    ${CMAKE_SOURCE_DIR}/entities
)

target_link_directories(DenPaKid PRIVATE
    ${CMAKE_SOURCE_DIR}/3rd/lib
)


# link SFML2.6
target_link_libraries(DenPaKid PRIVATE 
    sfml-graphics-d
    sfml-window-d
    sfml-system-d
    sfml-audio-d
    sfml-network-d
)