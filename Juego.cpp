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
    for (const auto& configuracion : obtenerConfiguracionesProyectiles())
    {
        if (!texturasProyectilesEnemigos_[configuracion.tipo].loadFromFile(
                configuracion.rutaTextura))
        {
            return -1;
        }
    }
    if (!texturaGameOver_.loadFromFile("assets/game_over.png"))
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
        else if (gameOver_ && event->is<sf::Event::KeyPressed>())
            reiniciar();
    }
}

void Juego::reiniciar()
{
    enemigos_.clear();
    enemigosAlien_.clear();
    proyectiles_.clear();
    proyectilesEnemigos_.clear();
    proximaOleada_ = 0;
    impactosNave_ = 0;
    vidaNave_ = 3;
    gameOver_ = false;
    nave_.reiniciarEstado();
    relojDisparo_.restart();
    relojInicio_.restart();
}

void Juego::actualizar()
{
    if (gameOver_)
        return;

    nave_.actualizar();
    procesarApariciones();
    actualizarEnemigos();
    dispararEnemigos();
    actualizarProyectilesEnemigos();
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
            enemigo->activar(
                posicionX,
                std::clamp(oleada.danio, 1, 3),
                std::max(0.1f, oleada.frecuenciaDisparo));
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
            enemigo->activar(
                posicionX,
                std::clamp(oleada.danio, 1, 3),
                std::max(0.1f, oleada.frecuenciaDisparo));
            enemigosAlien_.push_back(std::move(enemigo));
        }
    }
}

void Juego::dispararEnemigos()
{
    for (auto& enemigo : enemigos_)
    {
        if (!enemigo->listoParaDisparar())
            continue;

        const sf::Vector2f origen = enemigo->obtenerOrigenDisparo();
        for (const float desplazamientoX : {-14.f, 0.f, 14.f})
        {
            proyectilesEnemigos_.push_back({
                origen.x + desplazamientoX,
                origen.y,
                0.f,
                6.f,
                obtenerConfiguracionProyectil(TipoProyectilEnemigo::LaserCeleste).danio,
                TipoProyectilEnemigo::LaserCeleste,
                true
            });
        }
    }

    for (auto& enemigo : enemigosAlien_)
    {
        if (!enemigo->listoParaDisparar())
            continue;

        const sf::Vector2f origen = enemigo->obtenerOrigenDisparo();
        for (const float velocidadX : {-3.f, 0.f, 3.f})
        {
            proyectilesEnemigos_.push_back({
                origen.x,
                origen.y,
                velocidadX,
                4.5f,
                obtenerConfiguracionProyectil(TipoProyectilEnemigo::BolaEnergiaPurpura).danio,
                TipoProyectilEnemigo::BolaEnergiaPurpura,
                true
            });
        }
    }
}

void Juego::actualizarProyectilesEnemigos()
{
    for (auto& proyectil : proyectilesEnemigos_)
    {
        proyectil.x += proyectil.velocidadX;
        proyectil.y += proyectil.velocidadY;

        if (proyectil.x < -100.f || proyectil.x > 1124.f || proyectil.y > 1180.f)
            proyectil.activo = false;
    }

    proyectilesEnemigos_.erase(
        std::remove_if(
            proyectilesEnemigos_.begin(),
            proyectilesEnemigos_.end(),
            [](const ProyectilEnemigo& proyectil) { return !proyectil.activo; }),
        proyectilesEnemigos_.end());
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
    if (vidaNave_ <= 0 || nave_.esInvulnerable())
        return;

    const sf::FloatRect limitesNave = nave_.obtenerLimitesColision();

    for (auto& enemigo : enemigos_)
    {
        if (enemigo->estaActivo()
            && limitesNave.findIntersection(enemigo->obtenerLimitesColision()))
        {
            enemigo->desactivar();
            vidaNave_ = std::max(0, vidaNave_ - enemigo->obtenerDanio());
            nave_.recibirDanio();
            ++impactosNave_;
            gameOver_ = vidaNave_ <= 0;
            return;
        }
    }

    for (auto& enemigo : enemigosAlien_)
    {
        if (enemigo->estaActivo()
            && limitesNave.findIntersection(enemigo->obtenerLimitesColision()))
        {
            enemigo->desactivar();
            vidaNave_ = std::max(0, vidaNave_ - enemigo->obtenerDanio());
            nave_.recibirDanio();
            ++impactosNave_;
            gameOver_ = vidaNave_ <= 0;
            return;
        }
    }

    for (auto& proyectil : proyectilesEnemigos_)
    {
        if (proyectil.activo
            && limitesNave.findIntersection(obtenerLimitesProyectilEnemigo(proyectil)))
        {
            const auto& configuracion = obtenerConfiguracionProyectil(proyectil.tipo);
            if (configuracion.desapareceAlImpactar)
                proyectil.activo = false;
            vidaNave_ = std::max(0, vidaNave_ - proyectil.danio);
            nave_.recibirDanio();
            ++impactosNave_;
            gameOver_ = vidaNave_ <= 0;
            return;
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

    dibujarProyectilesEnemigos();
    dibujarDebug();
    if (gameOver_)
        dibujarGameOver();
    window_.display();
}

void Juego::dibujarGameOver()
{
    sf::Sprite sprite(texturaGameOver_);
    sprite.setScale({escalaGameOver_, escalaGameOver_});

    const sf::FloatRect limites = sprite.getGlobalBounds();
    sprite.setPosition({
        (1024.f - limites.size.x) / 2.f,
        (1080.f - limites.size.y) / 2.f
    });

    window_.draw(sprite);
}

void Juego::dibujarProyectilesEnemigos()
{
    for (const auto& proyectil : proyectilesEnemigos_)
    {
        if (proyectil.activo)
            dibujarProyectilEnemigo(proyectil);
    }
}

void Juego::dibujarEnemigos()
{
    for (const auto& enemigo : enemigos_)
        enemigo->dibujar(window_);
    for (const auto& enemigo : enemigosAlien_)
        enemigo->dibujar(window_);
}

void Juego::dibujarProyectilEnemigo(const ProyectilEnemigo& proyectil)
{
    const sf::Texture& textura = obtenerTexturaProyectilEnemigo(proyectil.tipo);
    const float escala = obtenerConfiguracionProyectil(proyectil.tipo).escala;

    sf::Sprite sprite(textura);
    sprite.setScale({escala, escala});
    sprite.setPosition({proyectil.x, proyectil.y});
    window_.draw(sprite);
}

sf::FloatRect Juego::obtenerLimitesProyectilEnemigo(const ProyectilEnemigo& proyectil) const
{
    const sf::Texture& textura = obtenerTexturaProyectilEnemigo(proyectil.tipo);
    const float escala = obtenerConfiguracionProyectil(proyectil.tipo).escala;

    sf::Sprite sprite(textura);
    sprite.setScale({escala, escala});
    sprite.setPosition({proyectil.x, proyectil.y});
    return sprite.getGlobalBounds();
}

const sf::Texture& Juego::obtenerTexturaProyectilEnemigo(TipoProyectilEnemigo tipo) const
{
    return texturasProyectilesEnemigos_.at(tipo);
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
