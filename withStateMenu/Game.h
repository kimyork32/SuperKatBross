// Game.h
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "BackGround.h"
#include "Gato.h"
#include "Colision.h"
#include "Mapa.h"
#include "Definitions.hpp"
#include "EnemigosManager.h"
#include "BloquesManager.h"
#include "ItemsManager.h"

class Game {
public:
    Game();  // Declaración del constructor
    void run();

private:
    sf::RenderWindow window;
    sf::View view;
    Colision colision;
    std::unique_ptr<Mapa> mapa;
    std::unique_ptr<Gato> gato;
    std::unique_ptr<EnemigosManager> enemigosManager;
    std::unique_ptr<BloquesManager> bloquesManager;
    std::unique_ptr<ItemsManager> itemsManager;
    sf::Vector2f posWindowFloat;
    bool colisionLados = false;
    sf::Clock clock;

    void handleEvents();
    void determinarLimiteVentana();
    void update();
    void render();
};

#endif // GAME_H
