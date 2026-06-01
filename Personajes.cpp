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
    window.draw(fuegoIzq_);
    window.draw(fuegoDer_);
    window.draw(spriteNave_);
}

sf::Vector2f Nave::obtenerOrigenDisparo() const
{
    const float anchoNave = spriteNave_.getGlobalBounds().size.x;
    return {x_ + (anchoNave / 2.f) - 4.f, y_ - 20.f};
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
    reaparecer();
    return true;
}

void Enemigo::actualizar()
{
    fase_ += velocidadOscilacion_;
    y_ += velocidadVertical_;

    const float ancho = sprite_.getGlobalBounds().size.x;
    const float x = xCentro_ + amplitud_ * std::cos(fase_) - ancho / 2.f;
    sprite_.setPosition({x, y_});

    if (y_ > 1080.f)
        reaparecer();
}

void Enemigo::dibujar(sf::RenderWindow& window) const
{
    window.draw(sprite_);
}

void Enemigo::reaparecer()
{
    y_ = -sprite_.getGlobalBounds().size.y;
    fase_ = 0.f;
    const float ancho = sprite_.getGlobalBounds().size.x;
    sprite_.setPosition({xCentro_ + amplitud_ - ancho / 2.f, y_});
}

bool EnemigoAlien::cargarTexturas()
{
    if (!texturaFrame1_.loadFromFile("assets/enemigo_alien.png"))
        return false;
    if (!texturaFrame2_.loadFromFile("assets/enemigo_alien_2.png"))
        return false;

    sprite_.setTexture(texturaFrame1_, true);
    sprite_.setScale({escala_, escala_});
    reaparecer();
    return true;
}

void EnemigoAlien::configurarMovimientoCoseno(
    float xCentro,
    float amplitud,
    float velocidadVertical,
    float velocidadOscilacion)
{
    xCentro_ = xCentro;
    amplitud_ = amplitud;
    velocidadVertical_ = velocidadVertical;
    velocidadOscilacion_ = velocidadOscilacion;
}

void EnemigoAlien::actualizar()
{
    actualizarAnimacion();

    fase_ += velocidadOscilacion_;
    y_ += velocidadVertical_;

    const float ancho = sprite_.getGlobalBounds().size.x;
    const float x = xCentro_ + amplitud_ * std::cos(fase_) - ancho / 2.f;
    sprite_.setPosition({x, y_});

    if (y_ > 1080.f)
        reaparecer();
}

void EnemigoAlien::dibujar(sf::RenderWindow& window) const
{
    window.draw(sprite_);
}

void EnemigoAlien::actualizarAnimacion()
{
    if (relojAnimacion_.getElapsedTime().asSeconds() < 0.18f)
        return;

    frame_ = (frame_ + 1) % 2;
    sprite_.setTexture(frame_ == 0 ? texturaFrame1_ : texturaFrame2_, true);
    relojAnimacion_.restart();
}

void EnemigoAlien::reaparecer()
{
    y_ = -sprite_.getGlobalBounds().size.y;
    fase_ = 0.f;
    const float ancho = sprite_.getGlobalBounds().size.x;
    sprite_.setPosition({xCentro_ + amplitud_ - ancho / 2.f, y_});
}
