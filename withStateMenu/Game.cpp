// Game.cpp
#include "Game.h"

Game::Game()
    : window(sf::VideoMode(800, 600), "Mapa y Personaje"),  // Tamaño de la ventana 800x600
    colision(),
    view(sf::FloatRect(0, 0, 800, 600))
{
    mapa = std::make_unique<Mapa>("map.txt", "tilemap_packed.png");
    gato = std::make_unique<Gato>(2 * cellSize, 2 * cellSize);

    enemigosManager = std::make_unique<EnemigosManager>();
    bloquesManager = std::make_unique<BloquesManager>();
    itemsManager = std::make_unique<ItemsManager>();

    enemigosManager->crearEnemigos(mapa);
    bloquesManager->crearBloques(mapa, gato);
    itemsManager->cargarPosicionesItemMap(mapa);
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        gato->processEvents(event);
    }
}

void Game::determinarLimiteVentana() {
    posWindowFloat.x = gato->getPosX();
    posWindowFloat.y = gato->getPosY();

    if (600 > numRows * cellSize) {
        posWindowFloat.y = numRows * cellSize - 600 / 2;
    }
    else if (posWindowFloat.y < 600 / 2) {
        posWindowFloat.y = 600 / 2;
    }
    else if (posWindowFloat.y > numRows * cellSize - 600 / 2) {
        posWindowFloat.y = numRows * cellSize - 600 / 2;
    }

    if (800 > numCols * cellSize) {
        posWindowFloat.x = 800 / 2;
    }
    else if (posWindowFloat.x < 800 / 2) {
        posWindowFloat.x = 800 / 2;
    }
    else if (posWindowFloat.x > numCols * cellSize - 800 / 2) {
        posWindowFloat.x = numCols * cellSize - 800 / 2;
    }
}

void Game::update() {
    sf::Time elapsed = clock.getElapsedTime();
    if (elapsed.asMilliseconds() < 16) {
        sf::sleep(sf::milliseconds(16 - elapsed.asMilliseconds()));
    }
    float deltaTime = clock.restart().asSeconds();

    gato->update(deltaTime, mapa->getMap());
    enemigosManager->update(deltaTime, mapa->getMap(), gato);
    itemsManager->update(deltaTime, mapa->getMap());

    colision.verificarColisionHitboxEnemigo(gato, enemigosManager->getVectorEnemigos());
    colision.verificarColisionBalaEnemigo(gato, enemigosManager->getVectorEnemigos());
    bloquesManager->verificarColisionGatoBloque(gato, mapa, itemsManager);
    itemsManager->verificarColisionHitboxItem(gato);

    determinarLimiteVentana();

    view.setCenter(posWindowFloat);
    window.setView(view);
}

void Game::render() {
    window.clear(sf::Color(0, 191, 255));

    mapa->draw(window);
    gato->drawTo(window);
    enemigosManager->draw(window);
    bloquesManager->draw(window);
    itemsManager->draw(window);
    window.display();
}
