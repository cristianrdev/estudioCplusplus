#include "Juego.hpp"

#include <algorithm>
#include <iomanip>
#include <optional>
#include <sstream>

Juego::Juego()
    : window_(sf::VideoMode({1024, 1080}), "Nave Shooter")
{
    window_.setFramerateLimit(60);
}

int Juego::ejecutar()
{
    if (!nave_.cargarTexturas())
        return -1;
    if (!fuenteDebug_.openFromFile("C:/Windows/Fonts/consola.ttf"))
        return -1;

    textoDebug_.setCharacterSize(22);
    textoDebug_.setFillColor(sf::Color::White);
    textoDebug_.setPosition({12.f, 10.f});
    relojInicio_.restart();

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
    procesarApariciones();
    actualizarEnemigos();
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

void Juego::procesarApariciones()
{
    const auto& oleadas = obtenerOleadasEnemigos();
    const float segundos = relojInicio_.getElapsedTime().asSeconds();

    while (proximaOleada_ < oleadas.size()
        && oleadas[proximaOleada_].tiempoSegundos <= segundos)
    {
        crearOleada(oleadas[proximaOleada_]);
        ++proximaOleada_;
    }
}

void Juego::crearOleada(const OleadaEnemigos& oleada)
{
    for (int i = 0; i < oleada.cantidad; ++i)
    {
        const float posicionX = oleada.posicionXInicial + i * oleada.separacionX;

        if (oleada.tipo == TipoEnemigo::Nave)
        {
            auto enemigo = std::make_unique<Enemigo>();
            if (!enemigo->cargarTextura())
                continue;
            enemigo->configurarMovimientoCoseno(
                oleada.movimiento.amplitud,
                oleada.movimiento.velocidadVertical,
                oleada.movimiento.velocidadOscilacion);
            enemigo->activar(posicionX, std::clamp(oleada.danio, 1, 3));
            enemigos_.push_back(std::move(enemigo));
        }
        else
        {
            auto enemigo = std::make_unique<EnemigoAlien>();
            if (!enemigo->cargarTexturas())
                continue;
            enemigo->configurarMovimientoCoseno(
                oleada.movimiento.amplitud,
                oleada.movimiento.velocidadVertical,
                oleada.movimiento.velocidadOscilacion);
            enemigo->activar(posicionX, std::clamp(oleada.danio, 1, 3));
            enemigosAlien_.push_back(std::move(enemigo));
        }
    }
}

void Juego::actualizarEnemigos()
{
    for (auto& enemigo : enemigos_)
        enemigo->actualizar();
    for (auto& enemigo : enemigosAlien_)
        enemigo->actualizar();

    detectarColisionesConNave();

    enemigos_.erase(
        std::remove_if(
            enemigos_.begin(),
            enemigos_.end(),
            [](const auto& enemigo) { return !enemigo->estaActivo(); }),
        enemigos_.end());
    enemigosAlien_.erase(
        std::remove_if(
            enemigosAlien_.begin(),
            enemigosAlien_.end(),
            [](const auto& enemigo) { return !enemigo->estaActivo(); }),
        enemigosAlien_.end());
}

void Juego::detectarColisionesConNave()
{
    if (vidaNave_ <= 0)
        return;

    const sf::FloatRect limitesNave = nave_.obtenerLimitesColision();

    for (auto& enemigo : enemigos_)
    {
        if (enemigo->estaActivo()
            && limitesNave.findIntersection(enemigo->obtenerLimitesColision()))
        {
            enemigo->desactivar();
            vidaNave_ = std::max(0, vidaNave_ - enemigo->obtenerDanio());
            ++impactosNave_;
        }
    }

    for (auto& enemigo : enemigosAlien_)
    {
        if (enemigo->estaActivo()
            && limitesNave.findIntersection(enemigo->obtenerLimitesColision()))
        {
            enemigo->desactivar();
            vidaNave_ = std::max(0, vidaNave_ - enemigo->obtenerDanio());
            ++impactosNave_;
        }
    }
}

void Juego::dibujar()
{
    window_.clear();
    dibujarEnemigos();
    nave_.dibujar(window_);

    for (const auto& proyectil : proyectiles_)
    {
        if (proyectil.activo)
            dibujarProyectil(proyectil);
    }

    dibujarDebug();
    window_.display();
}

void Juego::dibujarEnemigos()
{
    for (const auto& enemigo : enemigos_)
        enemigo->dibujar(window_);
    for (const auto& enemigo : enemigosAlien_)
        enemigo->dibujar(window_);
}

void Juego::dibujarDebug()
{
    std::ostringstream texto;
    texto << std::fixed << std::setprecision(2)
          << "Tiempo: " << relojInicio_.getElapsedTime().asSeconds() << " s"
          << "\nVida: " << vidaNave_ << "/3"
          << "\nImpactos: " << impactosNave_;
    if (vidaNave_ <= 0)
        texto << "\nGAME OVER";
    textoDebug_.setString(texto.str());
    window_.draw(textoDebug_);
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
