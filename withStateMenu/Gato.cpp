 #include "Gato.h"

Gato::Gato(float x, float y) {
    hitBox.setPosition(x, y);
	hitBox.setSize(sf::Vector2f(anchoHitbox, altoHitbox));
    hitBox.setFillColor(sf::Color::Red);

    this->monedas = 0;
    this->vidas = 3;
    this->numBalas = 0;

    this->left = false;
    this->right = false;
    this->keyZ = false;

    this->velocidadVariableX = 0.f;
    this->velocidadVariableY = 0.f;

    this->spacePressed = false;
    this->jumpButtonPressed = false;
    

    deltaTime = 0;
    jumpTime = 0.0f;
    loadSpriteSheet();
    //loadSpriteSheet("sprite.png");
}

void Gato::processEvents(const sf::Event& event) {

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Left)
            left = true;
        if (event.key.code == sf::Keyboard::Right)
            right = true;
        if (event.key.code == sf::Keyboard::Z)
            disparar();
        if (event.key.code == sf::Keyboard::Space && !spacePressed) {
            spacePressed = true;
            //teclaSuelta = false;
            clock2.restart();
        }
    }
    else if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Left)
            left = false;
        if (event.key.code == sf::Keyboard::Right)
            right = false;
        if (event.key.code == sf::Keyboard::Space) {
            spacePressed = false;
            //teclaSuelta = true;
        }
    }
}

//void Gato::saltar() {
//    if (getPosY() + 1 >= PISO.y - altoHitbox) {
//        velocidadY = JUMP_FORCE / MASS;
//        jumpTime = 0.0f;
//    }
//
//    jumpTime += deltaTime;
//    velocidadY += (JUMP_FORCE / MASS) * deltaTime / MAX_JUMP_TIME;  
//}
//
//void Gato::controlarSalto() {
//    this->jumpButtonPressed = static_cast<bool>(spacePressed && clock2.getElapsedTime().asMilliseconds() < 800 && !teclaSuelta);
//
//    if (jumpButtonPressed) {
//        saltar();
//   
//    }
//
//    // aplicar gravedad
//    velocidadY += GRAVITY * deltaTime;
//}
//

void Gato::saltar() {
    if (getPosY() + 1 >= PISO.y - altoHitbox) {
        velocidadY = JUMP_FORCE / MASS; // Velocidad inicial del salto
        jumpTime = 0.0f; // Reiniciar el tiempo de salto
    }

    // Actualizar el tiempo de salto
    jumpTime += deltaTime;

    // Aplicar la fuerza del salto
    if (jumpTime < MAX_JUMP_TIME) {
        velocidadY += (JUMP_FORCE / MASS) * deltaTime / MAX_JUMP_TIME;
    }
}

void Gato::controlarSalto() {
    this->jumpButtonPressed = static_cast<bool>(spacePressed  && !colisionPiso);
    if (jumpButtonPressed) {
        saltar();
    }
    // Aplicar gravedad
    velocidadY += GRAVITY * deltaTime;

}



void Gato::detectarPisoTecho(const std::vector<std::vector<int>>& map) {
    if ((getPosY() >= 0 && getPosY() + altoHitbox < numRows * cellSize) &&
        (getPosX() >= 0 && getPosX() + altoHitbox < numCols * cellSize)) {
        for (int i = getPosY() / cellSize; i < numRows; i++) {
            //if (map[i][getPosX() / cellSize] != 0 || map[i][(getPosX() + hitBox.getSize().x) / cellSize] != 0) {
            if ((map[i][getPosX() / cellSize] >= rangeBloqueBegin || map[i][(getPosX() + hitBox.getSize().x) / cellSize] >= rangeBloqueBegin) &&
                (map[i][getPosX() / cellSize] <= rangeBloqueEspecialEnd || map[i][(getPosX() + hitBox.getSize().x) / cellSize] <= rangeBloqueEspecialEnd)) {
                PISO.y = i * cellSize;

                PISO.x = getPosX() + anchoHitbox / 2;
                break;
            }
            else {
                //PISO.y = 400.0f;
                PISO.y = (numRows+1)*cellSize;
                PISO.x = getPosX() + anchoHitbox / 2;
            }
        }

        for (int i = getPosY() / cellSize; i >= 0; i--) {
            //if (map[i][getPosX() / cellSize] != 0 || map[i][(getPosX() + hitBox.getSize().x) / cellSize] != 0){
            if ((map[i][getPosX() / cellSize] >= rangeBloqueBegin || map[i][(getPosX() + hitBox.getSize().x) / cellSize] >= rangeBloqueBegin) &&
                (map[i][getPosX() / cellSize] <= rangeBloqueEspecialEnd || map[i][(getPosX() + hitBox.getSize().x) / cellSize] <= rangeBloqueEspecialEnd)) {

                //std::cout << map[i][getPosY() / cellSize] << " " << map[i][(getPosX() + hitBox.getSize().x) / cellSize] << std::endl;

                TECHO.y = (i + 1) * cellSize;
                TECHO.x = getPosX() + anchoHitbox / 2;
                break;
            }
            else {
                TECHO.y = -1000.0f;
            }
        }
    }
}

void Gato::detectarObjIzqDer(const std::vector<std::vector<int>>& map) {
    if ((getPosY() >= 0 && getPosY() + altoHitbox < numRows * cellSize) &&
        (getPosX() >= 0 && getPosX() + anchoHitbox < numCols * cellSize)) {

        // Detecci�n a la derecha
        for (int i = getPosX() / cellSize; i < numCols; i++) {
            //if (map[getPosY() / cellSize][i] != 0 || map[(getPosY() + altoHitbox) / cellSize][i] != 0) {
            if ((map[getPosY() / cellSize][i] >= rangeBloqueBegin || map[(getPosY() + altoHitbox) / cellSize][i] >= rangeBloqueBegin) &&
                (map[getPosY() / cellSize][i] <= rangeBloqueEspecialEnd || map[(getPosY() + altoHitbox) / cellSize][i] <= rangeBloqueEspecialEnd)) {
                
                //std::cout << map[getPosY() / cellSize][i] << " " << map[(getPosY() + altoHitbox)/ cellSize][i] << std::endl;

                OBJDER.x = i * cellSize;
				OBJDER.y = getPosY() + altoHitbox / 2;
                break;
            }
            else {
                OBJDER.x = numCols * cellSize;
            }
        }

        // Detecci�n a la izquierda
        for (int i = getPosX() / cellSize; i >= 0; i--) {
            //if (map[getPosY() / cellSize][i] != 0 || map[(getPosY() + altoHitbox) / cellSize][i] != 0) {
            if ((map[getPosY() / cellSize][i] >= rangeBloqueBegin || map[(getPosY() + altoHitbox) / cellSize][i] >= rangeBloqueBegin) && 
                (map[getPosY() / cellSize][i] <= rangeBloqueEspecialEnd || map[(getPosY() + altoHitbox) / cellSize][i] <= rangeBloqueEspecialEnd)) {


                OBJIZQ.x = (i + 1) * cellSize;
				OBJIZQ.y = getPosY() + altoHitbox / 2;
                break;
            }
            else {
                OBJIZQ.x = 0;
            }
        }
    }
}


void Gato::controlarMovimientoVertical(const std::vector<std::vector<int>>& map) {
    float nextMove = getPosY() + velocidadY * deltaTime;
    velocidadVariableY = velocidadY * deltaTime;

    if (nextMove < TECHO.y) {
        collideWithBlock(static_cast<int>(TECHO.x / cellSize), static_cast<int>(TECHO.y / cellSize) - 1.0f);
        nextMove = TECHO.y;
        velocidadY = 0;
        //teclaSuelta = true;
    }

    hitBox.move(0.f, nextMove - getPosY());
    //std::cout << getPosY() + altoHitbox << " " << PISO.y << std::endl;

    if (velocidadY > 0) {
        colisionPiso = true;
    }

    if (getPosY() + altoHitbox > PISO.y) {
        //collideWithBlock(static_cast<int>(PISO.x / cellSize), static_cast<int>(PISO.y / cellSize));
        velocidadY = 0.0f;

        hitBox.setPosition(getPosX(), PISO.y - altoHitbox - 1.0f);
    }

    if (getPosY() > numRows * cellSize) {
        morir();
    }

    //std::cout << velocidadY << " " << << std::endl;
    if (velocidadY == 0 && !spacePressed) {
        colisionPiso = false;
    }

    if (velocidadY != 0) {
        jumping = true;
    }
    else {
        jumping = false;
    }

}


void Gato::controlarMovimientoHorizontal(float deltaTime, const std::vector<std::vector<int>>& map) {
    
    float proxMovimientoX = 0.0f;

    if (left && !stopIzq) {
        proxMovimientoX -= velocidadX * deltaTime;
        velocidadVariableX = proxMovimientoX;
    }
    if (right && !stopDer) {
        proxMovimientoX += velocidadX * deltaTime;
        velocidadVariableX;
    }
    else {
        velocidadVariableX = 0;
    }
    
    if (proxMovimientoX != 0) {
        //std::cout << OBJDER.x << " " << getPosX() + anchoHitbox + proxMovimientoX << std::endl;
        if (getPosX() + anchoHitbox + proxMovimientoX > OBJDER.x) {
			//collideWithBlock(static_cast<int>(OBJDER.x / cellSize), static_cast<int>(OBJDER.y / cellSize));
			proxMovimientoX = OBJDER.x - getPosX() - anchoHitbox - 1.0f;
            //std::cout << "stop der " << std::endl;
        }
       
        else if (getPosX() + proxMovimientoX < OBJIZQ.x) {
            //collideWithBlock(static_cast<int>(OBJIZQ.x / cellSize) - 1.0f, static_cast<int>(OBJIZQ.y / }cellSize));
            proxMovimientoX = OBJIZQ.x - getPosX() + 1.0f;
            //std::cout << "stop izq" << std::endl;
        }

        else {
            hitBox.move(proxMovimientoX, 0.f);
        }
        
    }
}

sf::Vector2f Gato::getPosition() const {
    return hitBox.getPosition();
}

void Gato::loadSpriteSheet() {

    if (!texturaGatoCaminar.loadFromFile("spriteGatoCaminando.png")) {
        std::cerr << "Error al cargar la textura" << std::endl;
    }
    if (!texturaGatoParado.loadFromFile("spriteGatoParado.png")) {
        std::cerr << "Error al cargar la textura" << std::endl;
    }

    if (!texturaGatoSaltando.loadFromFile("spriteGatoSalto.png")) {
        std::cerr << "Error al cargar la textura" << std::endl;
    }

    spriteGatoCaminar.setTexture(texturaGatoCaminar);
    spriteGatoCaminar.setTextureRect(sf::IntRect(0, 0, anchoSprite, altoSprite));

    spriteGatoParado.setTexture(texturaGatoParado);
    spriteGatoParado.setTextureRect(sf::IntRect(0, 0, anchoSprite, altoSprite));

    spriteGatoSaltando.setTexture(texturaGatoSaltando);
    spriteGatoSaltando.setTextureRect(sf::IntRect(0, 0, anchoSprite, altoSprite));

}



void Gato::update(float deltaTime, const std::vector<std::vector<int>>& map) {
    this->deltaTime = deltaTime;

    detectarObjIzqDer(map);
    detectarPisoTecho(map);

    controlarSalto();

    controlarMovimientoHorizontal(deltaTime, map);
    controlarMovimientoVertical(map);
    moverSprites();
    
    for (auto& bala : balas) {
        bala->update(deltaTime, map);
    }
    verificarTiempoVidaBalas();
    verificarColisionBalaBloque();
    

}

sf::RectangleShape Gato::getHitBox() {
	return hitBox;
}

float Gato::getAnchoHitbox() {
	return anchoHitbox;
}

float Gato::getAltoHitbox() {
	return altoHitbox;
}

float Gato::getPosX() {
	return hitBox.getPosition().x;
}

float Gato::getPosY() {
	return hitBox.getPosition().y;
}


void Gato::pararMovimientoXDer(float esquinaIzqObj) {
	hitBox.move(esquinaIzqObj - getPosX() - anchoHitbox - 1.0f, 0);
    spriteGato.setTextureRect(sf::IntRect(0, 0, anchoSprite, altoSprite));
}

void Gato::seguirMovimientoXDer() {
	this->stopDer = false;
}


void Gato::morir() {
    hitBox.setPosition(2 * cellSize, 2 * cellSize);
}

void Gato::collideWithBlock(int i, int j) {
    notify(i, j);
}

void Gato::setVidas(unsigned int vidas) {
    this->vidas = this->vidas + vidas;
}

unsigned int Gato::getVidas() const {
    return vidas;
}

void Gato::setVelocidadX(float velocidadX) {
    this->velocidadX = velocidadX;
}

void Gato::aumentarVelocidadX(float velocidadPlus) {
    if (!isAumentarVelocidadUsado) {
        this->velocidadX = this->velocidadX + velocidadPlus;
        isAumentarVelocidadUsado = true;
    }
}

float Gato::getVelocidadX() {
    return velocidadVariableX;
}

float Gato::getVelocidadY() {
    return velocidadVariableY;
}

unsigned int Gato::getMonedas() {
    return monedas;
}

void Gato::aumentarMonedas() {
    this-> monedas += 1;
}

void Gato::setBalas(int numBalas) {
    this->numBalas = this->numBalas + numBalas;
}

int Gato::getBalas() const {
    return numBalas;
}

void Gato::drawTo(sf::RenderWindow& window) {
    //window.draw(hitBox);
    drawSprites(window);
    for (auto& bala : balas) {
        bala->drawTo(window);
    }
}


void Gato::drawSprites(sf::RenderWindow& window) {
    if (jumping) {
        window.draw(spriteGatoSaltando);
    }
    else if ((!left && !right) || (left && right)) {
        window.draw(spriteGatoParado);
    }
    else if (left || right) {
        window.draw(spriteGatoCaminar);
    }
}

void Gato::moverSprites() {

    if (left && !right) {
        spriteGatoCaminar.setScale(-escalaX, escalaY); // Invertir el sprite en el eje X
        xTexture = (static_cast<int>(getPosX() / velocidadSprite) % numFigurasSprite) * static_cast<int>(anchoSprite);
        spriteGatoCaminar.setTextureRect(sf::IntRect(xTexture, 0, anchoSprite, altoSprite));
        spriteGatoCaminar.setPosition(getPosX() - ((anchoSprite * escalaX - anchoHitbox) / 2) + anchoSprite * escalaX, getPosY() - ((altoSprite * escalaY - altoHitbox) / 2));

        countSpriteCambio = 0;
        mirandoIzq = true;
    }
    else if (right && !left) {
        spriteGatoCaminar.setScale(escalaX, escalaY); // No invertir el sprite
        xTexture = (numFigurasSprite - 1 - static_cast<int>(getPosX() / velocidadSprite) % numFigurasSprite) * static_cast<int>(anchoSpriteParado);
        spriteGatoCaminar.setTextureRect(sf::IntRect(xTexture, 0, anchoSpriteParado, altoSpriteParado));
        spriteGatoCaminar.setPosition(getPosX() - ((anchoSpriteParado * escalaX - anchoHitbox) / 2), getPosY() - ((altoSpriteParado * escalaY - altoHitbox) / 2));
        
        countSpriteCambio = 0;
        mirandoIzq = false;
    }
    else if ((!left && !right) || (left && right)) {
        countSpriteCambio += velocidadX * deltaTime;
        if (countSpriteCambio > 1000.0f) countSpriteCambio = 0;

        if (mirandoIzq) {
            spriteGatoParado.setScale(-escalaX, escalaY);
            //std::cout << xTexture << std::endl;
            xTexture = (static_cast<int>(countSpriteCambio / velocidadSpriteParado) % numFigurasSprite) * static_cast<int>(anchoSprite);
            spriteGatoParado.setTextureRect(sf::IntRect(xTexture, 0, anchoSprite, altoSprite));
            spriteGatoParado.setPosition(getPosX() - ((anchoSprite * escalaX - anchoHitbox) / 2) + anchoSprite * escalaX, getPosY() - ((altoSprite * escalaY - altoHitbox) / 2));

        }
        else {
            //std::cout << xTexture << std::endl;
            spriteGatoParado.setScale(escalaX, escalaY);
            xTexture = (numFigurasSprite - 1 - static_cast<int>(countSpriteCambio / velocidadSpriteParado) % numFigurasSprite) * static_cast<int>(anchoSprite);
            spriteGatoParado.setTextureRect(sf::IntRect(xTexture, 0, anchoSprite, altoSprite));
            spriteGatoParado.setPosition(getPosX() - ((anchoSprite * escalaX - anchoHitbox) / 2), getPosY() - ((altoSprite * escalaY - altoHitbox) / 2));
        }
    }

    if (jumping) {
        if(velocidadY < 0){
            if (mirandoIzq) {
                spriteGatoSaltando.setScale(-escalaX, escalaY);
                spriteGatoSaltando.setTextureRect(sf::IntRect(0, 0, anchoSprite, altoSprite));
                spriteGatoSaltando.setPosition(getPosX() - ((anchoSprite * escalaX - anchoHitbox) / 2) + anchoSprite * escalaX, getPosY() - ((altoSprite * escalaY - altoHitbox) / 2));
            }
            else {
                spriteGatoSaltando.setScale(escalaX, escalaY);
                spriteGatoSaltando.setTextureRect(sf::IntRect(0, 0, anchoSprite, altoSprite));
                spriteGatoSaltando.setPosition(getPosX() - ((anchoSprite * escalaX - anchoHitbox) / 2), getPosY() - ((altoSprite * escalaY - altoHitbox) / 2));
            }
            
            
        }
        else {
            if (mirandoIzq) {
                spriteGatoSaltando.setScale(-escalaX, escalaY);
                spriteGatoSaltando.setTextureRect(sf::IntRect(anchoSprite, 0, anchoSprite, altoSprite));
                spriteGatoSaltando.setPosition(getPosX() - ((anchoSprite * escalaX - anchoHitbox) / 2) + anchoSprite * escalaX, getPosY() - ((altoSprite * escalaY - altoHitbox) / 2));
            }
            else {
                spriteGatoSaltando.setScale(escalaX, escalaY);
                spriteGatoSaltando.setTextureRect(sf::IntRect(anchoSprite   , 0, anchoSprite, altoSprite));
                spriteGatoSaltando.setPosition(getPosX() - ((anchoSprite * escalaX - anchoHitbox) / 2), getPosY() - ((altoSprite * escalaY - altoHitbox) / 2));
            }
        }
    }
}



void Gato::disparar() {
    if (numBalas != 0) {
        std::unique_ptr<Bala> bala = std::make_unique<Bala>(getPosX() + anchoHitbox / 2, getPosY() + altoHitbox / 2, mirandoIzq);
        bala->setVelocidadX(this->velocidadX*2);
        bala->setTiempoVida(6.f);
        balas.push_back(std::move(bala));

        numBalas--;
    }
}

bool Gato::verificarColisionBalaEnemigo(std::unique_ptr<Enemigo>& enemigo) {
    for (int i = 0; i < balas.size();) {
        if (enemigo->getHitBox().getGlobalBounds().intersects(balas[i]->getHitBox().getGlobalBounds())) {
            balas.erase(balas.begin() + i);
            return true;
        }
        i++;
    }
    return false;
}


void Gato::verificarColisionBalaBloque() {
    for (int i = 0; i < balas.size(); i++) {
        if (balas.at(i)->getPosX() + anchoHitbox > OBJDER.x) {
            balas.erase(balas.begin() + i);
        }

        else if (getPosX() < OBJIZQ.x) {
            balas.erase(balas.begin() + i);
        }
    }
}

void Gato::verificarTiempoVidaBalas() {
    for (int i = 0; i < balas.size(); i++) {
        if (balas.at(i)->verificarTiempoVida()) {
            balas.erase(balas.begin() + i);
        }
    }
}
