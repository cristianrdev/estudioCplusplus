#include "Personajes.hpp"

#include <cmath>

bool Nave::cargarTexturas()
{
    if (!texturaCentro_.loadFromFile("assets/centro.png"))
        return false;
    if (!texturaIzquierda_.loadFromFile("assets/izquierda.png"))
        return false;
    if (!texturaDerecha_.loadFromFile("assets/derecha.png"))
        return false;
    if (!fuegoTex1_.loadFromFile("assets/fuego1.png"))
        return false;
    if (!fuegoTex2_.loadFromFile("assets/fuego2.png"))
        return false;
    if (!fuegoTex3_.loadFromFile("assets/fuego3.png"))
        return false;

    spriteNave_.setTexture(texturaCentro_, true);
    fuegoIzq_.setTexture(fuegoTex1_, true);
    fuegoDer_.setTexture(fuegoTex1_, true);
    actualizarPosiciones();
    return true;
}

void Nave::actualizar()
{
    if (invulnerable_
        && relojInvulnerabilidad_.getElapsedTime().asSeconds() >= duracionInvulnerabilidad_)
    {
        invulnerable_ = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
    {
        y_ -= velocidad_;
        largoFuego_ = 0.1f;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
    {
        y_ += velocidad_;
        largoFuego_ = 0.05f;
    }
    else
    {
        largoFuego_ = escalaFuego_;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        x_ -= velocidad_;
        spriteNave_.setTexture(texturaIzquierda_, true);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        x_ += velocidad_;
        spriteNave_.setTexture(texturaDerecha_, true);
    }
    else
    {
        spriteNave_.setTexture(texturaCentro_, true);
    }

    actualizarAnimacionFuego();
    actualizarPosiciones();
}

void Nave::dibujar(sf::RenderWindow& window) const
{
    if (invulnerable_)
    {
        const float segundos = relojInvulnerabilidad_.getElapsedTime().asSeconds();
        const int faseParpadeo = static_cast<int>(segundos / intervaloParpadeo_);
        if (faseParpadeo % 2 != 0)
            return;
    }

    window.draw(fuegoIzq_);
    window.draw(fuegoDer_);
    window.draw(spriteNave_);
}

sf::Vector2f Nave::obtenerOrigenDisparo() const
{
    const float anchoNave = spriteNave_.getGlobalBounds().size.x;
    return {x_ + (anchoNave / 2.f) - 4.f, y_ - 20.f};
}

sf::Vector2f Nave::obtenerCentro() const
{
    const sf::FloatRect limites = spriteNave_.getGlobalBounds();
    return {
        limites.position.x + limites.size.x / 2.f,
        limites.position.y + limites.size.y / 2.f
    };
}

sf::FloatRect Nave::obtenerLimitesColision() const
{
    return spriteNave_.getGlobalBounds();
}

bool Nave::esInvulnerable() const
{
    return invulnerable_;
}

void Nave::recibirDanio()
{
    invulnerable_ = true;
    relojInvulnerabilidad_.restart();
}

void Nave::reiniciarEstado()
{
    invulnerable_ = false;
}

void Nave::actualizarAnimacionFuego()
{
    if (relojFuego_.getElapsedTime().asSeconds() < 0.07f)
        return;

    frameFuego_ = (frameFuego_ + 1) % 3;

    if (frameFuego_ == 0)
    {
        fuegoIzq_.setTexture(fuegoTex1_, true);
        fuegoDer_.setTexture(fuegoTex1_, true);
    }
    else if (frameFuego_ == 1)
    {
        fuegoIzq_.setTexture(fuegoTex2_, true);
        fuegoDer_.setTexture(fuegoTex2_, true);
    }
    else
    {
        fuegoIzq_.setTexture(fuegoTex3_, true);
        fuegoDer_.setTexture(fuegoTex3_, true);
    }

    relojFuego_.restart();
}

void Nave::actualizarPosiciones()
{
    spriteNave_.setScale({escalaNave_, escalaNave_});
    spriteNave_.setPosition({x_, y_});

    const float anchoNave = spriteNave_.getGlobalBounds().size.x;
    const float altoNave = spriteNave_.getGlobalBounds().size.y;

    fuegoIzq_.setScale({escalaFuego_, largoFuego_});
    fuegoDer_.setScale({escalaFuego_, largoFuego_});
    fuegoIzq_.setPosition({x_ + 6.f, y_ + altoNave - 14.f});
    fuegoDer_.setPosition({x_ + anchoNave - 19.f, y_ + altoNave - 14.f});
}

bool Enemigo::cargarTextura()
{
    if (!textura_.loadFromFile("assets/enemigo.png"))
        return false;

    sprite_.setTexture(textura_, true);
    sprite_.setScale({escala_, escala_});
    return true;
}

void Enemigo::activar(float posicionX, int danio, float frecuenciaDisparo)
{
    posicionXInicial_ = posicionX;
    y_ = -sprite_.getGlobalBounds().size.y;
    fase_ = 0.f;
    activo_ = true;
    danio_ = danio;
    frecuenciaDisparo_ = frecuenciaDisparo;
    relojDisparo_.restart();
    sprite_.setPosition({posicionXInicial_, y_});
}

void Enemigo::configurarMovimientoCoseno(
    float amplitud,
    float velocidadVertical,
    float velocidadOscilacion)
{
    amplitud_ = amplitud;
    velocidadVertical_ = velocidadVertical;
    velocidadOscilacion_ = velocidadOscilacion;
}

void Enemigo::actualizar()
{
    if (!activo_)
        return;

    fase_ += velocidadOscilacion_;
    y_ += velocidadVertical_;

    const float x = posicionXInicial_ + amplitud_ * (std::cos(fase_) - 1.f);
    sprite_.setPosition({x, y_});

    if (y_ > 1080.f)
        activo_ = false;
}

void Enemigo::dibujar(sf::RenderWindow& window) const
{
    if (activo_)
        window.draw(sprite_);
}

bool Enemigo::estaActivo() const
{
    return activo_;
}

void Enemigo::desactivar()
{
    activo_ = false;
}

int Enemigo::obtenerDanio() const
{
    return danio_;
}

bool Enemigo::listoParaDisparar()
{
    if (!activo_ || relojDisparo_.getElapsedTime().asSeconds() < frecuenciaDisparo_)
        return false;

    relojDisparo_.restart();
    return true;
}

sf::Vector2f Enemigo::obtenerOrigenDisparo() const
{
    const sf::FloatRect limites = sprite_.getGlobalBounds();
    return {
        limites.position.x + limites.size.x / 2.f,
        limites.position.y + limites.size.y
    };
}

sf::FloatRect Enemigo::obtenerLimitesColision() const
{
    return sprite_.getGlobalBounds();
}

bool EnemigoAlien::cargarTexturas()
{
    if (!texturaFrame1_.loadFromFile("assets/enemigo_alien.png"))
        return false;
    if (!texturaFrame2_.loadFromFile("assets/enemigo_alien_2.png"))
        return false;

    sprite_.setTexture(texturaFrame1_, true);
    sprite_.setScale({escala_, escala_});
    return true;
}

void EnemigoAlien::activar(float posicionX, int danio, float frecuenciaDisparo)
{
    posicionXInicial_ = posicionX;
    y_ = -sprite_.getGlobalBounds().size.y;
    fase_ = 0.f;
    activo_ = true;
    danio_ = danio;
    frecuenciaDisparo_ = frecuenciaDisparo;
    relojDisparo_.restart();
    sprite_.setPosition({posicionXInicial_, y_});
}

void EnemigoAlien::configurarMovimientoCoseno(
    float amplitud,
    float velocidadVertical,
    float velocidadOscilacion)
{
    amplitud_ = amplitud;
    velocidadVertical_ = velocidadVertical;
    velocidadOscilacion_ = velocidadOscilacion;
}

void EnemigoAlien::actualizar()
{
    if (!activo_)
        return;

    actualizarAnimacion();

    fase_ += velocidadOscilacion_;
    y_ += velocidadVertical_;

    const float x = posicionXInicial_ + amplitud_ * (std::cos(fase_) - 1.f);
    sprite_.setPosition({x, y_});

    if (y_ > 1080.f)
        activo_ = false;
}

void EnemigoAlien::dibujar(sf::RenderWindow& window) const
{
    if (activo_)
        window.draw(sprite_);
}

bool EnemigoAlien::estaActivo() const
{
    return activo_;
}

void EnemigoAlien::desactivar()
{
    activo_ = false;
}

int EnemigoAlien::obtenerDanio() const
{
    return danio_;
}

bool EnemigoAlien::listoParaDisparar()
{
    if (!activo_ || relojDisparo_.getElapsedTime().asSeconds() < frecuenciaDisparo_)
        return false;

    relojDisparo_.restart();
    return true;
}

sf::Vector2f EnemigoAlien::obtenerOrigenDisparo() const
{
    const sf::FloatRect limites = sprite_.getGlobalBounds();
    return {
        limites.position.x + limites.size.x / 2.f,
        limites.position.y + limites.size.y
    };
}

sf::FloatRect EnemigoAlien::obtenerLimitesColision() const
{
    return sprite_.getGlobalBounds();
}

void EnemigoAlien::actualizarAnimacion()
{
    if (relojAnimacion_.getElapsedTime().asSeconds() < 0.18f)
        return;

    frame_ = (frame_ + 1) % 2;
    sprite_.setTexture(frame_ == 0 ? texturaFrame1_ : texturaFrame2_, true);
    relojAnimacion_.restart();
}

bool Esbirro::cargarTextura()
{
    if (!textura_.loadFromFile("assets/esbirro.png"))
        return false;

    sprite_.setTexture(textura_, true);
    sprite_.setScale({escala_, escala_});
    return true;
}

void Esbirro::activar(float posicionX, int danio, float frecuenciaDisparo)
{
    y_ = -sprite_.getGlobalBounds().size.y;
    activo_ = true;
    danio_ = danio;
    frecuenciaDisparo_ = frecuenciaDisparo;
    relojDisparo_.restart();
    sprite_.setPosition({posicionX, y_});
}

void Esbirro::configurarVelocidad(float velocidadVertical)
{
    velocidadVertical_ = velocidadVertical;
}

void Esbirro::actualizar()
{
    if (!activo_)
        return;

    y_ += velocidadVertical_;
    sprite_.setPosition({sprite_.getPosition().x, y_});

    if (y_ > 1080.f)
        activo_ = false;
}

void Esbirro::dibujar(sf::RenderWindow& window) const
{
    if (activo_)
        window.draw(sprite_);
}

bool Esbirro::estaActivo() const
{
    return activo_;
}

void Esbirro::desactivar()
{
    activo_ = false;
}

int Esbirro::obtenerDanio() const
{
    return danio_;
}

bool Esbirro::listoParaDisparar()
{
    if (!activo_ || relojDisparo_.getElapsedTime().asSeconds() < frecuenciaDisparo_)
        return false;

    relojDisparo_.restart();
    return true;
}

sf::Vector2f Esbirro::obtenerOrigenDisparo() const
{
    const sf::FloatRect limites = sprite_.getGlobalBounds();
    return {
        limites.position.x + limites.size.x / 2.f,
        limites.position.y + limites.size.y
    };
}

sf::FloatRect Esbirro::obtenerLimitesColision() const
{
    return sprite_.getGlobalBounds();
}
