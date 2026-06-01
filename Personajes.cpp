#include "Personajes.hpp"

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
