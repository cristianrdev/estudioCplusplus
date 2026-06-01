#include "Juego.hpp"

#include <algorithm>
#include <optional>

Juego::Juego()
    : window_(sf::VideoMode({1024, 1080}), "Nave Shooter")
{
    window_.setFramerateLimit(60);
}

int Juego::ejecutar()
{
    if (!nave_.cargarTexturas())
        return -1;
    if (!enemigo_.cargarTextura())
        return -1;

    while (window_.isOpen())
    {
        procesarEventos();
        actualizar();
        dibujar();
    }

    return 0;
}

void Juego::procesarEventos()
{
    while (const std::optional event = window_.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window_.close();
    }
}

void Juego::actualizar()
{
    nave_.actualizar();
    enemigo_.actualizar();
    disparar();
    actualizarProyectiles();
}

void Juego::disparar()
{
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        return;
    if (relojDisparo_.getElapsedTime().asSeconds() < cadenciaDisparo_)
        return;

    const sf::Vector2f origen = nave_.obtenerOrigenDisparo();
    proyectiles_.push_back({origen.x, origen.y, true});
    relojDisparo_.restart();
}

void Juego::actualizarProyectiles()
{
    for (auto& proyectil : proyectiles_)
    {
        if (proyectil.activo)
            proyectil.y -= velocidadLaser_;

        if (proyectil.y < -40.f)
            proyectil.activo = false;
    }

    proyectiles_.erase(
        std::remove_if(
            proyectiles_.begin(),
            proyectiles_.end(),
            [](const Proyectil& proyectil) { return !proyectil.activo; }),
        proyectiles_.end());
}

void Juego::dibujar()
{
    window_.clear();
    enemigo_.dibujar(window_);
    nave_.dibujar(window_);

    for (const auto& proyectil : proyectiles_)
    {
        if (proyectil.activo)
            dibujarProyectil(proyectil);
    }

    window_.display();
}

void Juego::dibujarProyectil(const Proyectil& proyectil)
{
    sf::RectangleShape laserRojo({8.f, 30.f});
    laserRojo.setPosition({proyectil.x, proyectil.y});
    laserRojo.setFillColor(sf::Color::Red);

    sf::RectangleShape laserBlanco({4.f, 30.f});
    laserBlanco.setPosition({proyectil.x + 2.f, proyectil.y});
    laserBlanco.setFillColor(sf::Color::White);

    sf::RectangleShape punta({8.f, 4.f});
    punta.setPosition({proyectil.x, proyectil.y - 4.f});
    punta.setFillColor(sf::Color::Yellow);

    window_.draw(laserRojo);
    window_.draw(laserBlanco);
    window_.draw(punta);
}
