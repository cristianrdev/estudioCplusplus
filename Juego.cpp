#include "Juego.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
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
    if (!texturaLaserJugador_.loadFromFile("assets/laser_jugador.png"))
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
    for (const auto& [tipo, ruta] : std::initializer_list<std::pair<TipoEnemigo, const char*>>{
        {TipoEnemigo::Nave, "assets/explosion_enemigo_nave.png"},
        {TipoEnemigo::Alien, "assets/explosion_enemigo_alien.png"},
        {TipoEnemigo::Esbirro, "assets/explosion_esbirro.png"},
        {TipoEnemigo::MoluscoGiratorio, "assets/explosion_esbirro.png"},
        {TipoEnemigo::MiniBossMolusco, "assets/explosion_enemigo_alien.png"}})
    {
        if (!texturasExplosionesEnemigos_[tipo].loadFromFile(ruta))
            return -1;
    }
    for (const char* ruta : {
        "assets/explosion_nave_1.png",
        "assets/explosion_nave_2.png",
        "assets/explosion_nave_3.png"})
    {
        sf::Texture textura;
        if (!textura.loadFromFile(ruta))
            return -1;
        texturasExplosionNave_.push_back(std::move(textura));
    }
    for (const char* ruta : {
        "assets/impacto_laser_1.png",
        "assets/impacto_laser_2.png"})
    {
        sf::Texture textura;
        if (!textura.loadFromFile(ruta))
            return -1;
        texturasImpactoLaser_.push_back(std::move(textura));
    }
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
        else if (!gameOver_
            && event->is<sf::Event::KeyPressed>()
            && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::P)
        {
            alternarPausa();
        }
        else if (gameOver_ && event->is<sf::Event::KeyPressed>())
            reiniciar();
    }
}

void Juego::alternarPausa()
{
    pausado_ = !pausado_;

    if (pausado_)
    {
        relojDisparo_.stop();
        relojInicio_.stop();
        relojExplosionNave_.stop();
        relojEsperaGameOver_.stop();
    }
    else
    {
        relojDisparo_.start();
        relojInicio_.start();
        relojExplosionNave_.start();
        relojEsperaGameOver_.start();
    }

    nave_.establecerPausa(pausado_);
    for (auto& enemigo : enemigos_)
        enemigo->establecerPausa(pausado_);
    for (auto& enemigo : enemigosAlien_)
        enemigo->establecerPausa(pausado_);
    for (auto& esbirro : esbirros_)
        esbirro->establecerPausa(pausado_);
    for (auto& molusco : moluscosGiratorios_)
        molusco->establecerPausa(pausado_);
    for (auto& miniBoss : miniBossesMolusco_)
        miniBoss->establecerPausa(pausado_);
}

void Juego::reiniciar()
{
    enemigos_.clear();
    enemigosAlien_.clear();
    esbirros_.clear();
    moluscosGiratorios_.clear();
    miniBossesMolusco_.clear();
    proyectiles_.clear();
    proyectilesEnemigos_.clear();
    explosionesEnemigos_.clear();
    impactosLaser_.clear();
    ultimasOleadasDebug_.clear();
    proximaOleada_ = 0;
    impactosNave_ = 0;
    vidaNave_ = 3;
    frameExplosionNave_ = 0;
    gameOver_ = false;
    naveExplotando_ = false;
    esperandoGameOver_ = false;
    pausado_ = false;
    nave_.reiniciarEstado();
    relojDisparo_.restart();
    relojInicio_.restart();
}

void Juego::iniciarExplosionNave()
{
    const sf::FloatRect limites = nave_.obtenerLimitesColision();
    centroExplosionNave_ = {
        limites.position.x + limites.size.x / 2.f,
        limites.position.y + limites.size.y / 2.f
    };
    frameExplosionNave_ = 0;
    naveExplotando_ = true;
    relojExplosionNave_.restart();
}

void Juego::actualizarExplosionNave()
{
    if (!naveExplotando_)
        return;
    if (relojExplosionNave_.getElapsedTime().asSeconds() < duracionFrameExplosionNave_)
        return;

    ++frameExplosionNave_;
    relojExplosionNave_.restart();

    if (frameExplosionNave_ >= static_cast<int>(texturasExplosionNave_.size()))
    {
        naveExplotando_ = false;
        esperandoGameOver_ = true;
        relojEsperaGameOver_.restart();
    }
}

void Juego::actualizar()
{
    if (pausado_)
        return;
    if (gameOver_)
        return;
    if (esperandoGameOver_)
    {
        if (relojEsperaGameOver_.getElapsedTime().asSeconds() >= esperaAntesGameOver_)
        {
            esperandoGameOver_ = false;
            gameOver_ = true;
        }
        return;
    }
    if (naveExplotando_)
    {
        actualizarExplosionNave();
        return;
    }

    nave_.actualizar();
    procesarApariciones();
    actualizarEnemigos();
    dispararEnemigos();
    actualizarProyectilesEnemigos();
    disparar();
    actualizarProyectiles();
    detectarColisionesProyectilesJugador();
    actualizarExplosionesEnemigos();
    actualizarImpactosLaser();
}

void Juego::detectarColisionesProyectilesJugador()
{
    for (auto& proyectil : proyectiles_)
    {
        if (!proyectil.activo)
            continue;

        const sf::FloatRect limitesProyectil = obtenerLimitesProyectilJugador(proyectil);

        for (auto& enemigo : enemigos_)
        {
            if (enemigo->estaActivo())
            {
                const auto interseccion = limitesProyectil.findIntersection(
                    enemigo->obtenerLimitesColision());
                if (!interseccion)
                    continue;
                crearImpactoLaser(*interseccion);
                proyectil.activo = false;
                if (enemigo->recibirDanio(1))
                {
                    crearExplosionEnemigo(TipoEnemigo::Nave, enemigo->obtenerLimitesColision());
                    enemigo->desactivar();
                }
                break;
            }
        }
        if (!proyectil.activo)
            continue;

        for (auto& enemigo : enemigosAlien_)
        {
            if (enemigo->estaActivo())
            {
                const auto interseccion = limitesProyectil.findIntersection(
                    enemigo->obtenerLimitesColision());
                if (!interseccion)
                    continue;
                crearImpactoLaser(*interseccion);
                proyectil.activo = false;
                if (enemigo->recibirDanio(1))
                {
                    crearExplosionEnemigo(TipoEnemigo::Alien, enemigo->obtenerLimitesColision());
                    enemigo->desactivar();
                }
                break;
            }
        }
        if (!proyectil.activo)
            continue;

        for (auto& esbirro : esbirros_)
        {
            if (esbirro->estaActivo())
            {
                const auto interseccion = limitesProyectil.findIntersection(
                    esbirro->obtenerLimitesColision());
                if (!interseccion)
                    continue;
                crearImpactoLaser(*interseccion);
                proyectil.activo = false;
                if (esbirro->recibirDanio(1))
                {
                    crearExplosionEnemigo(TipoEnemigo::Esbirro, esbirro->obtenerLimitesColision());
                    esbirro->desactivar();
                }
                break;
            }
        }
        if (!proyectil.activo)
            continue;

        for (auto& molusco : moluscosGiratorios_)
        {
            if (molusco->estaActivo())
            {
                const auto interseccion = limitesProyectil.findIntersection(
                    molusco->obtenerLimitesColision());
                if (!interseccion)
                    continue;
                crearImpactoLaser(*interseccion);
                proyectil.activo = false;
                if (molusco->recibirDanio(1))
                {
                    crearExplosionEnemigo(
                        TipoEnemigo::MoluscoGiratorio,
                        molusco->obtenerLimitesColision());
                    molusco->desactivar();
                }
                break;
            }
        }
        if (!proyectil.activo)
            continue;

        for (auto& miniBoss : miniBossesMolusco_)
        {
            if (miniBoss->estaActivo())
            {
                const auto interseccion = limitesProyectil.findIntersection(
                    miniBoss->obtenerLimitesColision());
                if (!interseccion)
                    continue;
                crearImpactoLaser(*interseccion);
                proyectil.activo = false;
                if (miniBoss->recibirDanio(1))
                {
                    crearExplosionEnemigo(
                        TipoEnemigo::MiniBossMolusco,
                        miniBoss->obtenerLimitesColision());
                    miniBoss->desactivar();
                }
                break;
            }
        }
    }
}

void Juego::crearImpactoLaser(const sf::FloatRect& interseccion)
{
    impactosLaser_.push_back({
        {
            interseccion.position.x + interseccion.size.x / 2.f,
            interseccion.position.y + interseccion.size.y / 2.f
        },
        relojInicio_.getElapsedTime().asSeconds()
    });
}

void Juego::crearExplosionEnemigo(TipoEnemigo tipo, const sf::FloatRect& limites)
{
    explosionesEnemigos_.push_back({
        {
            limites.position.x + limites.size.x / 2.f,
            limites.position.y + limites.size.y / 2.f
        },
        tipo,
        relojInicio_.getElapsedTime().asSeconds()
    });
}

void Juego::actualizarExplosionesEnemigos()
{
    const float tiempoActual = relojInicio_.getElapsedTime().asSeconds();
    explosionesEnemigos_.erase(
        std::remove_if(
            explosionesEnemigos_.begin(),
            explosionesEnemigos_.end(),
            [tiempoActual, this](const ExplosionEnemigo& explosion) {
                return tiempoActual - explosion.tiempoInicio >= duracionExplosionEnemigo_;
            }),
        explosionesEnemigos_.end());
}

void Juego::actualizarImpactosLaser()
{
    const float tiempoActual = relojInicio_.getElapsedTime().asSeconds();
    const float duracion = duracionFrameImpactoLaser_ * texturasImpactoLaser_.size();
    impactosLaser_.erase(
        std::remove_if(
            impactosLaser_.begin(),
            impactosLaser_.end(),
            [tiempoActual, duracion](const ImpactoLaser& impacto) {
                return tiempoActual - impacto.tiempoInicio >= duracion;
            }),
        impactosLaser_.end());
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
        registrarOleadaDebug(oleadas[proximaOleada_], segundos);
        crearOleada(oleadas[proximaOleada_]);
        ++proximaOleada_;
    }
}

void Juego::registrarOleadaDebug(const OleadaEnemigos& oleada, float tiempoReal)
{
    const auto& comportamiento = obtenerComportamientoEnemigo(oleada.comportamiento);
    const char* tipo = "Esbirro";
    if (oleada.tipo == TipoEnemigo::Nave)
        tipo = "Nave";
    else if (oleada.tipo == TipoEnemigo::Alien)
        tipo = "Alien";
    else if (oleada.tipo == TipoEnemigo::MiniBossMolusco)
        tipo = "MiniBossMolusco";
    else if (oleada.tipo == TipoEnemigo::MoluscoGiratorio)
        tipo = "MoluscoGiratorio";

    std::ostringstream registro;
    registro << std::fixed << std::setprecision(2)
             << "t_real=" << tiempoReal << "s"
             << " t_programado=" << oleada.tiempoSegundos << "s"
             << " tipo=" << tipo
             << " cantidad=" << oleada.cantidad
             << " danio=" << oleada.danio
             << " vida=" << oleada.vida
             << " frecuencia_disparo=" << oleada.frecuenciaDisparo << "s"
             << " x_inicial=" << oleada.posicionXInicial
             << " separacion_x=" << oleada.separacionX
             << " comportamiento=" << comportamiento.nombre;

    ultimasOleadasDebug_.push_back(registro.str());
    if (ultimasOleadasDebug_.size() > 5)
        ultimasOleadasDebug_.erase(ultimasOleadasDebug_.begin());

    std::cout << "\n[DEBUG] Ultimas oleadas generadas (" << ultimasOleadasDebug_.size()
              << "/5):\n";
    for (const auto& linea : ultimasOleadasDebug_)
        std::cout << "  " << linea << '\n';
    std::cout << std::flush;
}

void Juego::crearOleada(const OleadaEnemigos& oleada)
{
    const auto& comportamiento = obtenerComportamientoEnemigo(oleada.comportamiento);

    for (int i = 0; i < oleada.cantidad; ++i)
    {
        const float posicionX = oleada.posicionXInicial + i * oleada.separacionX;

        if (oleada.tipo == TipoEnemigo::Nave)
        {
            auto enemigo = std::make_unique<Enemigo>();
            if (!enemigo->cargarTextura())
                continue;
            enemigo->configurarMovimientoDiagonal(
                comportamiento.movimiento.amplitudOVelocidadHorizontal,
                comportamiento.movimiento.velocidadVertical);
            enemigo->activar(
                posicionX,
                std::clamp(oleada.danio, 1, 3),
                std::max(1, oleada.vida),
                std::max(0.1f, oleada.frecuenciaDisparo));
            enemigos_.push_back(std::move(enemigo));
        }
        else if (oleada.tipo == TipoEnemigo::Alien)
        {
            auto enemigo = std::make_unique<EnemigoAlien>();
            if (!enemigo->cargarTexturas())
                continue;
            enemigo->configurarMovimientoCoseno(
                comportamiento.movimiento.amplitudOVelocidadHorizontal,
                comportamiento.movimiento.velocidadVertical,
                comportamiento.movimiento.velocidadOscilacion);
            enemigo->activar(
                posicionX,
                std::clamp(oleada.danio, 1, 3),
                std::max(1, oleada.vida),
                std::max(0.1f, oleada.frecuenciaDisparo));
            enemigosAlien_.push_back(std::move(enemigo));
        }
        else if (oleada.tipo == TipoEnemigo::Esbirro)
        {
            auto esbirro = std::make_unique<Esbirro>();
            if (!esbirro->cargarTextura())
                continue;
            esbirro->configurarVelocidad(comportamiento.movimiento.velocidadVertical);
            esbirro->activar(
                posicionX,
                std::clamp(oleada.danio, 1, 3),
                std::max(1, oleada.vida),
                std::max(0.1f, oleada.frecuenciaDisparo));
            esbirros_.push_back(std::move(esbirro));
        }
        else if (oleada.tipo == TipoEnemigo::MoluscoGiratorio)
        {
            auto molusco = std::make_unique<MoluscoGiratorio>();
            if (!molusco->cargarTextura())
                continue;
            molusco->configurarMovimiento(
                comportamiento.movimiento.amplitudOVelocidadHorizontal,
                comportamiento.movimiento.velocidadVertical);
            molusco->activar(
                posicionX,
                std::clamp(oleada.danio, 1, 3),
                std::max(1, oleada.vida),
                std::max(0.1f, oleada.frecuenciaDisparo));
            moluscosGiratorios_.push_back(std::move(molusco));
        }
        else
        {
            auto miniBoss = std::make_unique<MiniBossMolusco>();
            if (!miniBoss->cargarTexturas())
                continue;
            miniBoss->configurarMovimiento(
                comportamiento.movimiento.velocidadVertical,
                comportamiento.miniBoss.alturaEspera,
                comportamiento.miniBoss.duracionEspera,
                comportamiento.miniBoss.velocidadHorizontal);
            miniBoss->activar(
                posicionX,
                std::clamp(oleada.danio, 1, 3),
                std::max(1, oleada.vida),
                std::max(0.1f, oleada.frecuenciaDisparo));
            miniBossesMolusco_.push_back(std::move(miniBoss));
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

    for (auto& esbirro : esbirros_)
    {
        if (!esbirro->listoParaDisparar())
            continue;

        const sf::Vector2f origen = esbirro->obtenerOrigenDisparo();
        const sf::Vector2f objetivo = nave_.obtenerCentro();
        const float direccionX = objetivo.x - origen.x;
        const float direccionY = objetivo.y - origen.y;
        const float distancia = std::sqrt(direccionX * direccionX + direccionY * direccionY);
        if (distancia <= 0.f)
            continue;

        constexpr float velocidad = 6.5f;
        proyectilesEnemigos_.push_back({
            origen.x,
            origen.y,
            direccionX / distancia * velocidad,
            direccionY / distancia * velocidad,
            obtenerConfiguracionProyectil(TipoProyectilEnemigo::PuntoEnergiaAmarillo).danio,
            TipoProyectilEnemigo::PuntoEnergiaAmarillo,
            true
        });
    }

    for (auto& miniBoss : miniBossesMolusco_)
    {
        if (!miniBoss->listoParaDisparar())
            continue;

        for (const sf::Vector2f origen : {
            miniBoss->obtenerOrigenDisparoIzquierdo(),
            miniBoss->obtenerOrigenDisparoDerecho()})
        {
            for (const float velocidadX : {-3.f, 0.f, 3.f})
            {
                proyectilesEnemigos_.push_back({
                    origen.x,
                    origen.y,
                    velocidadX,
                    4.5f,
                    obtenerConfiguracionProyectil(
                        TipoProyectilEnemigo::BolaEnergiaPurpura).danio,
                    TipoProyectilEnemigo::BolaEnergiaPurpura,
                    true
                });
            }
        }
    }

    for (auto& molusco : moluscosGiratorios_)
    {
        if (!molusco->listoParaDisparar())
            continue;

        const sf::Vector2f origen = molusco->obtenerOrigenDisparo();
        const sf::Vector2f objetivo = nave_.obtenerCentro();
        const float direccionX = objetivo.x - origen.x;
        const float direccionY = objetivo.y - origen.y;
        const float distancia = std::sqrt(direccionX * direccionX + direccionY * direccionY);
        if (distancia <= 0.f)
            continue;

        constexpr float velocidad = 6.5f;
        proyectilesEnemigos_.push_back({
            origen.x,
            origen.y,
            direccionX / distancia * velocidad,
            direccionY / distancia * velocidad,
            obtenerConfiguracionProyectil(TipoProyectilEnemigo::PuntoEnergiaAmarillo).danio,
            TipoProyectilEnemigo::PuntoEnergiaAmarillo,
            true
        });
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
    for (auto& esbirro : esbirros_)
        esbirro->actualizar();
    for (auto& molusco : moluscosGiratorios_)
        molusco->actualizar();
    for (auto& miniBoss : miniBossesMolusco_)
        miniBoss->actualizar();

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
    esbirros_.erase(
        std::remove_if(
            esbirros_.begin(),
            esbirros_.end(),
            [](const auto& esbirro) { return !esbirro->estaActivo(); }),
        esbirros_.end());
    moluscosGiratorios_.erase(
        std::remove_if(
            moluscosGiratorios_.begin(),
            moluscosGiratorios_.end(),
            [](const auto& molusco) { return !molusco->estaActivo(); }),
        moluscosGiratorios_.end());
    miniBossesMolusco_.erase(
        std::remove_if(
            miniBossesMolusco_.begin(),
            miniBossesMolusco_.end(),
            [](const auto& miniBoss) { return !miniBoss->estaActivo(); }),
        miniBossesMolusco_.end());
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
            if (vidaNave_ <= 0)
                iniciarExplosionNave();
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
            if (vidaNave_ <= 0)
                iniciarExplosionNave();
            return;
        }
    }

    for (auto& esbirro : esbirros_)
    {
        if (esbirro->estaActivo()
            && limitesNave.findIntersection(esbirro->obtenerLimitesColision()))
        {
            esbirro->desactivar();
            vidaNave_ = std::max(0, vidaNave_ - esbirro->obtenerDanio());
            nave_.recibirDanio();
            ++impactosNave_;
            if (vidaNave_ <= 0)
                iniciarExplosionNave();
            return;
        }
    }

    for (auto& miniBoss : miniBossesMolusco_)
    {
        if (miniBoss->estaActivo()
            && limitesNave.findIntersection(miniBoss->obtenerLimitesColision()))
        {
            vidaNave_ = std::max(0, vidaNave_ - miniBoss->obtenerDanio());
            nave_.recibirDanio();
            ++impactosNave_;
            if (vidaNave_ <= 0)
                iniciarExplosionNave();
            return;
        }
    }

    for (auto& molusco : moluscosGiratorios_)
    {
        if (molusco->estaActivo()
            && limitesNave.findIntersection(molusco->obtenerLimitesColision()))
        {
            molusco->desactivar();
            vidaNave_ = std::max(0, vidaNave_ - molusco->obtenerDanio());
            nave_.recibirDanio();
            ++impactosNave_;
            if (vidaNave_ <= 0)
                iniciarExplosionNave();
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
            if (vidaNave_ <= 0)
                iniciarExplosionNave();
            return;
        }
    }
}

void Juego::dibujar()
{
    window_.clear();
    dibujarEnemigos();
    if (!naveExplotando_ && !esperandoGameOver_ && !gameOver_)
        nave_.dibujar(window_);

    for (const auto& proyectil : proyectiles_)
    {
        if (proyectil.activo)
            dibujarProyectil(proyectil);
    }

    dibujarProyectilesEnemigos();
    dibujarExplosionesEnemigos();
    dibujarImpactosLaser();
    dibujarDebug();
    if (naveExplotando_)
        dibujarExplosionNave();
    if (gameOver_)
        dibujarGameOver();
    window_.display();
}

void Juego::dibujarImpactosLaser()
{
    const float tiempoActual = relojInicio_.getElapsedTime().asSeconds();

    for (const auto& impacto : impactosLaser_)
    {
        const int frame = static_cast<int>(
            (tiempoActual - impacto.tiempoInicio) / duracionFrameImpactoLaser_);
        if (frame < 0 || frame >= static_cast<int>(texturasImpactoLaser_.size()))
            continue;

        sf::Sprite sprite(texturasImpactoLaser_[frame]);
        sprite.setScale({escalaImpactoLaser_, escalaImpactoLaser_});

        const sf::FloatRect limites = sprite.getGlobalBounds();
        sprite.setPosition({
            impacto.centro.x - limites.size.x / 2.f,
            impacto.centro.y - limites.size.y / 2.f
        });
        window_.draw(sprite);
    }
}

void Juego::dibujarExplosionesEnemigos()
{
    const float tiempoActual = relojInicio_.getElapsedTime().asSeconds();

    for (const auto& explosion : explosionesEnemigos_)
    {
        const float progreso = (tiempoActual - explosion.tiempoInicio)
            / duracionExplosionEnemigo_;
        const float escalaBase = explosion.tipo == TipoEnemigo::Alien
            ? 0.1f
            : explosion.tipo == TipoEnemigo::MiniBossMolusco
                ? 0.22f
                : explosion.tipo == TipoEnemigo::Nave ? 0.075f : 0.055f;
        const float escala = escalaBase * (0.65f + progreso * 0.7f);

        sf::Sprite sprite(texturasExplosionesEnemigos_.at(explosion.tipo));
        sprite.setScale({escala, escala});
        sprite.setColor(sf::Color(255, 255, 255, static_cast<std::uint8_t>(255.f * (1.f - progreso))));

        const sf::FloatRect limites = sprite.getGlobalBounds();
        sprite.setPosition({
            explosion.centro.x - limites.size.x / 2.f,
            explosion.centro.y - limites.size.y / 2.f
        });
        window_.draw(sprite);
    }
}

void Juego::dibujarExplosionNave()
{
    if (frameExplosionNave_ >= static_cast<int>(texturasExplosionNave_.size()))
        return;

    sf::Sprite sprite(texturasExplosionNave_[frameExplosionNave_]);
    sprite.setScale({escalaExplosionNave_, escalaExplosionNave_});

    const sf::FloatRect limites = sprite.getGlobalBounds();
    sprite.setPosition({
        centroExplosionNave_.x - limites.size.x / 2.f,
        centroExplosionNave_.y - limites.size.y / 2.f
    });

    window_.draw(sprite);
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
    for (const auto& esbirro : esbirros_)
        esbirro->dibujar(window_);
    for (const auto& molusco : moluscosGiratorios_)
        molusco->dibujar(window_);
    for (const auto& miniBoss : miniBossesMolusco_)
        miniBoss->dibujar(window_);
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
    else if (pausado_)
        texto << "\nPAUSA";
    textoDebug_.setString(texto.str());
    window_.draw(textoDebug_);
}

void Juego::dibujarProyectil(const Proyectil& proyectil)
{
    sf::Sprite sprite(texturaLaserJugador_);
    sprite.setScale({escalaLaserJugador_, escalaLaserJugador_});
    sprite.setPosition({proyectil.x, proyectil.y});
    window_.draw(sprite);
}

sf::FloatRect Juego::obtenerLimitesProyectilJugador(const Proyectil& proyectil) const
{
    sf::Sprite sprite(texturaLaserJugador_);
    sprite.setScale({escalaLaserJugador_, escalaLaserJugador_});
    sprite.setPosition({proyectil.x, proyectil.y});
    return sprite.getGlobalBounds();
}
