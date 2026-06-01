#pragma once

#include "Personajes.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

struct Proyectil
{
    float x;
    float y;
    bool activo;
};

class Juego {
public:
    Juego();
    int ejecutar();

private:
    void procesarEventos();
    void actualizar();
    void disparar();
    void actualizarProyectiles();
    void dibujar();
    void dibujarProyectil(const Proyectil& proyectil);

    sf::RenderWindow window_;
    Nave nave_;
    std::vector<Proyectil> proyectiles_;
    sf::Clock relojDisparo_;

    float cadenciaDisparo_ = 0.12f;
    float velocidadLaser_ = 8.f;
};
