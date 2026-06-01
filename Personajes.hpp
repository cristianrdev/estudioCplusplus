#pragma once

#include <SFML/Graphics.hpp>

class Nave {
public:
    bool cargarTexturas();
    void actualizar();
    void dibujar(sf::RenderWindow& window) const;
    sf::Vector2f obtenerOrigenDisparo() const;

private:
    void actualizarAnimacionFuego();
    void actualizarPosiciones();

    float escalaNave_ = 0.11f;
    float escalaFuego_ = 0.08f;
    float largoFuego_ = escalaFuego_;
    float x_ = 340.f;
    float y_ = 500.f;
    float velocidad_ = 8.f;
    int frameFuego_ = 0;

    sf::Clock relojFuego_;

    sf::Texture texturaCentro_;
    sf::Texture texturaIzquierda_;
    sf::Texture texturaDerecha_;
    sf::Texture fuegoTex1_;
    sf::Texture fuegoTex2_;
    sf::Texture fuegoTex3_;

    sf::Sprite spriteNave_{texturaCentro_};
    sf::Sprite fuegoIzq_{fuegoTex1_};
    sf::Sprite fuegoDer_{fuegoTex1_};
};
