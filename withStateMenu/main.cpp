#include <SFML/Graphics.hpp>
#include "Menu.h"


int main() {
    //std::cout << valItemCatnip << std::endl;
    //std::cout << valItemAji << std::endl;
    //std::cout << valItemMoneda << std::endl;
    //std::cout << valItemPollo << std::endl;
    //std::cout << valEnemigoErizo<< std::endl;

    sf::RenderWindow window(sf::VideoMode(800, 600), "SuperKat");

    // Crear e iniciar el menú
    Menu menu(window);
    menu.run();
    return 0;
}