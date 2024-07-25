#include "ScoreMenuH.h"
#include <iostream>

void ScoreMenuH::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonReleased) {
        // L�gica espec�fica para la pantalla de puntuaciones
        std::cout << "Manejo de entrada en pantalla de puntuaciones" << std::endl;
    }
    else if (nextHandler) {
        nextHandler->handleInput(event);
    }
}
