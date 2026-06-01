#pragma once

#include "ConfiguracionEnemigos.hpp"
#include "Personajes.hpp"

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <memory>
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
    void procesarApariciones();
    void crearOleada(const OleadaEnemigos& oleada);
    void actualizarEnemigos();
    void detectarColisionesConNave();
    void dibujar();
    void dibujarEnemigos();
    void dibujarDebug();
    void dibujarProyectil(const Proyectil& proyectil);

    sf::RenderWindow window_;
    Nave nave_;
    std::vector<std::unique_ptr<Enemigo>> enemigos_;
    std::vector<std::unique_ptr<EnemigoAlien>> enemigosAlien_;
    std::vector<Proyectil> proyectiles_;
    sf::Clock relojDisparo_;
    sf::Clock relojInicio_;
    sf::Font fuenteDebug_;
    sf::Text textoDebug_{fuenteDebug_};
    std::size_t proximaOleada_ = 0;
    int impactosNave_ = 0;
    int vidaNave_ = 3;

    float cadenciaDisparo_ = 0.12f;
    float velocidadLaser_ = 8.f;
};
