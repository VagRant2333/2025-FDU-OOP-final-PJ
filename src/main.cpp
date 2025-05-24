#include "core/Game.h"
#include <iostream>

const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;
const std::string WINDOW_TITLE = "DenPaKid";

int main() {
    try {
        Game game(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
        std::cout << "instancing game./ main.cpp" << std::endl;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "An exception occurred: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "An unknown exception occurred." << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}