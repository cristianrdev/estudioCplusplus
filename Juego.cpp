#include "Juego.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>

Juego::Juego()
    : window_(sf::VideoMode({1024, 1080}), "Nave Shooter")
{
    window_.setFramerateLimit(60);
    proyectiles_.reserve(128);
    proyectilesEnemigos_.reserve(1024);
    fogonazosCanones_.reserve(32);
    impactosLaser_.reserve(128);
}

namespace
{
constexpr float tamanioTileTerreno = 128.f;
constexpr int columnasTilemapTerreno = 8;
constexpr int filasSectorRocoso = 16;
constexpr float anchoSectorTerreno = columnasTilemapTerreno * tamanioTileTerreno;
constexpr float altoSectorTerreno = filasSectorRocoso * tamanioTileTerreno;

std::uint32_t siguienteAleatorio(std::uint32_t& estado)
{
    estado = estado * 1664525u + 1013904223u;
    return estado;
}

float aleatorioEntre(std::uint32_t& estado, float minimo, float maximo)
{
    const float proporcion = static_cast<float>(siguienteAleatorio(estado) & 0xffffu)
        / 65535.f;
    return minimo + (maximo - minimo) * proporcion;
}
}

int Juego::ejecutar()
{
    texturasCargadas_ = 0;
    if (!nave_.cargarTexturas())
        return -1;
    texturasCargadas_ += nave_.obtenerCantidadTexturasCargadas();
    if (!cargarTexturaContabilizada(texturaEnemigo_, "assets/enemigo_contorno.png"))
        return -1;
    if (!cargarTexturaContabilizada(
            texturaEnemigoAlienFrame1_, "assets/enemigo_alien_contorno.png"))
        return -1;
    if (!cargarTexturaContabilizada(
            texturaEnemigoAlienFrame2_, "assets/enemigo_alien_2_contorno.png"))
        return -1;
    if (!cargarTexturaContabilizada(texturaEsbirro_, "assets/esbirro_contorno.png"))
        return -1;
    if (!cargarTexturaContabilizada(
            texturaMoluscoGiratorio_, "assets/molusco_giratorio_contorno.png"))
        return -1;
    if (!cargarTexturaContabilizada(
            texturaMiniBossMoluscoFrame1_, "assets/miniboss_molusco_1_contorno.png"))
        return -1;
    if (!cargarTexturaContabilizada(
            texturaMiniBossMoluscoFrame2_, "assets/miniboss_molusco_2_contorno.png"))
        return -1;
    if (!cargarTexturaContabilizada(
            texturaMiniBossMoluscoFrame3_, "assets/miniboss_molusco_3_contorno.png"))
        return -1;
    if (!cargarTexturaContabilizada(texturaPescadoGigante_, "assets/pescado_gigante.png"))
        return -1;
    if (!cargarTexturaContabilizada(texturaLaserJugador_, "assets/laser_jugador.png"))
        return -1;
    if (!cargarTexturaContabilizada(texturaLaserJugadorAzul_, "assets/laser_jugador_azul.png"))
        return -1;
    tamanioLaserJugador_ = {
        texturaLaserJugador_.getSize().x * escalaLaserJugador_,
        texturaLaserJugador_.getSize().y * escalaLaserJugador_
    };
    tamanioLaserJugadorAzul_ = {
        texturaLaserJugadorAzul_.getSize().x * escalaLaserJugadorAzul_,
        texturaLaserJugadorAzul_.getSize().y * escalaLaserJugadorAzul_
    };
    if (!cargarTexturaContabilizada(texturaCapsulaItemFrame1_, "assets/capsula_item_1.png"))
        return -1;
    if (!cargarTexturaContabilizada(texturaCapsulaItemFrame2_, "assets/capsula_item_2.png"))
        return -1;
    if (!cargarTexturaContabilizada(texturaPowerUpP_, "assets/power_up_p.png"))
        return -1;
    if (!cargarTexturaContabilizada(texturaAtlasRocasFondo_, "assets/atlas_rocas_fondo.png"))
        return -1;
    if (!cargarTexturaContabilizada(
            texturaSectorTerrenoFondo_, "assets/sector_terreno_rocoso.png"))
        return -1;
    for (const auto& configuracion : obtenerConfiguracionesProyectiles())
    {
        if (!cargarTexturaContabilizada(
                texturasProyectilesEnemigos_[configuracion.tipo],
                configuracion.rutaTextura))
        {
            return -1;
        }
    }
    if (!cargarTexturaContabilizada(texturaGameOver_, "assets/game_over.png"))
        return -1;
    for (const auto& [tipo, ruta] : std::initializer_list<std::pair<TipoEnemigo, const char*>>{
        {TipoEnemigo::Nave, "assets/explosion_enemigo_nave.png"},
        {TipoEnemigo::Alien, "assets/explosion_enemigo_alien.png"},
        {TipoEnemigo::Esbirro, "assets/explosion_esbirro.png"},
        {TipoEnemigo::MoluscoGiratorio, "assets/explosion_esbirro.png"},
        {TipoEnemigo::MiniBossMolusco, "assets/explosion_enemigo_alien.png"},
        {TipoEnemigo::PescadoGigante, "assets/explosion_enemigo_nave.png"}})
    {
        if (!cargarTexturaContabilizada(texturasExplosionesEnemigos_[tipo], ruta))
            return -1;
    }
    for (const char* ruta : {
        "assets/explosion_nave_1.png",
        "assets/explosion_nave_2.png",
        "assets/explosion_nave_3.png"})
    {
        sf::Texture textura;
        if (!cargarTexturaContabilizada(textura, ruta))
            return -1;
        texturasExplosionNave_.push_back(std::move(textura));
    }
    for (const char* ruta : {
        "assets/impacto_laser_1.png",
        "assets/impacto_laser_2.png"})
    {
        sf::Texture textura;
        if (!cargarTexturaContabilizada(textura, ruta))
            return -1;
        texturasImpactoLaser_.push_back(std::move(textura));
    }
    if (!fuenteDebug_.openFromFile("C:/Windows/Fonts/consola.ttf"))
        return -1;

    textoDebug_.setCharacterSize(22);
    textoDebug_.setFillColor(sf::Color::White);
    textoDebug_.setPosition({12.f, 10.f});
    relojInicio_.restart();
    relojFrame_.restart();
    inicializarEstrellasFondo();

    while (window_.isOpen())
    {
        const float duracionFrameMs = relojFrame_.restart().asSeconds() * 1000.f;
        registrarRendimientoFrame(duracionFrameMs);
        procesarEventos();
        actualizar();
        dibujar();
    }

    return 0;
}

bool Juego::cargarTexturaContabilizada(sf::Texture& textura, const char* ruta)
{
    if (!textura.loadFromFile(ruta))
        return false;

    ++texturasCargadas_;
    return true;
}

void Juego::registrarRendimientoFrame(float duracionMs)
{
    if (duracionMs <= 0.f)
        return;

    acumuladoMuestraFps_ += duracionMs;
    peorFrameMuestraMs_ = std::max(peorFrameMuestraMs_, duracionMs);
    ++framesMuestraFps_;

    if (acumuladoMuestraFps_ >= 500.f)
    {
        fps_ = framesMuestraFps_ * 1000.f / acumuladoMuestraFps_;
        peorFrameVisibleMs_ = peorFrameMuestraMs_;
        acumuladoMuestraFps_ = 0.f;
        peorFrameMuestraMs_ = 0.f;
        framesMuestraFps_ = 0;
    }

    const float tiempoJuego = relojInicio_.getElapsedTime().asSeconds();
    if (duracionMs < umbralFrameLentoMs_
        || tiempoJuego - tiempoUltimoLogRendimiento_ < 0.25f)
    {
        return;
    }

    tiempoUltimoLogRendimiento_ = tiempoJuego;
    const std::size_t enemigosActivos = enemigos_.size()
        + enemigosAlien_.size()
        + esbirros_.size()
        + moluscosGiratorios_.size()
        + miniBossesMolusco_.size()
        + pescadosGigantes_.size();

    std::cout << std::fixed << std::setprecision(2)
              << "\n[PERF] frame_lento"
              << " t=" << tiempoJuego << "s"
              << " frame=" << duracionMs << "ms"
              << " fps_hud=" << fps_
              << " enemigos=" << enemigosActivos
              << " proyectiles_jugador=" << proyectiles_.size()
              << " proyectiles_enemigos=" << proyectilesEnemigos_.size()
              << " fogonazos=" << fogonazosCanones_.size()
              << " impactos=" << impactosLaser_.size()
              << " fondos=" << tilesTerrenoFondo_.size()
              << " rocas=" << elementosFondo_.size()
              << '\n'
              << std::flush;
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
    for (auto& pescado : pescadosGigantes_)
        pescado->establecerPausa(pausado_);
}

void Juego::reiniciar()
{
    enemigos_.clear();
    enemigosAlien_.clear();
    esbirros_.clear();
    moluscosGiratorios_.clear();
    miniBossesMolusco_.clear();
    pescadosGigantes_.clear();
    proyectiles_.clear();
    proyectilesEnemigos_.clear();
    capsulasItems_.clear();
    powerUps_.clear();
    elementosFondo_.clear();
    tilesTerrenoFondo_.clear();
    estrellasFondo_.clear();
    explosionesEnemigos_.clear();
    impactosLaser_.clear();
    fogonazosCanones_.clear();
    proximaOleada_ = 0;
    proximaAparicionItem_ = 0;
    proximaAparicionElementoFondo_ = 0;
    proximaAparicionFondo_ = 0;
    impactosNave_ = 0;
    vidaNave_ = 3;
    frameExplosionNave_ = 0;
    gameOver_ = false;
    naveExplotando_ = false;
    esperandoGameOver_ = false;
    pausado_ = false;
    laserDobleActivo_ = false;
    nave_.reiniciarEstado();
    relojDisparo_.restart();
    relojInicio_.restart();
    relojFrame_.restart();
    fps_ = 0.f;
    acumuladoMuestraFps_ = 0.f;
    peorFrameMuestraMs_ = 0.f;
    peorFrameVisibleMs_ = 0.f;
    framesMuestraFps_ = 0;
    tiempoUltimoLogRendimiento_ = -10.f;
    inicializarEstrellasFondo();
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
    {
        actualizarExplosionesEnemigos();
        actualizarImpactosLaser();
        actualizarFogonazosCanones();
        return;
    }
    if (esperandoGameOver_)
    {
        actualizarExplosionesEnemigos();
        actualizarImpactosLaser();
        actualizarFogonazosCanones();
        if (relojEsperaGameOver_.getElapsedTime().asSeconds() >= esperaAntesGameOver_)
        {
            esperandoGameOver_ = false;
            gameOver_ = true;
        }
        return;
    }
    if (naveExplotando_)
    {
        actualizarExplosionesEnemigos();
        actualizarImpactosLaser();
        actualizarFogonazosCanones();
        actualizarExplosionNave();
        return;
    }

    nave_.actualizar();
    actualizarEstrellasFondo();
    procesarAparicionesFondo();
    actualizarTerrenoFondo();
    procesarAparicionesElementosFondo();
    procesarApariciones();
    procesarAparicionesItems();
    actualizarElementosFondo();
    actualizarEnemigos();
    actualizarCapsulasItems();
    actualizarPowerUps();
    dispararEnemigos();
    actualizarProyectilesEnemigos();
    disparar();
    actualizarProyectiles();
    detectarColisionesProyectilesCapsulas();
    detectarColisionesProyectilesJugador();
    detectarColisionesPowerUps();
    actualizarExplosionesEnemigos();
    actualizarImpactosLaser();
    actualizarFogonazosCanones();
}

void Juego::inicializarEstrellasFondo()
{
    struct CapaEstrellas
    {
        int cantidad;
        float velocidadMinima;
        float velocidadMaxima;
        float tamanioMinimo;
        float tamanioMaximo;
        std::uint8_t brilloMinimo;
        std::uint8_t brilloMaximo;
    };

    constexpr CapaEstrellas capas[] = {
        {55, 0.35f, 0.65f, 1.f, 1.f, 80, 145},
        {32, 0.8f, 1.2f, 1.f, 2.f, 130, 205},
        {16, 1.45f, 2.f, 2.f, 3.f, 185, 255},
    };

    std::uint32_t estado = 0x5a17u;
    estrellasFondo_.clear();
    for (const auto& capa : capas)
    {
        for (int i = 0; i < capa.cantidad; ++i)
        {
            const auto brillo = static_cast<std::uint8_t>(
                aleatorioEntre(estado, capa.brilloMinimo, capa.brilloMaximo));
            const float tamanio = std::floor(
                aleatorioEntre(estado, capa.tamanioMinimo, capa.tamanioMaximo) + 0.5f);
            estrellasFondo_.push_back({
                aleatorioEntre(estado, 0.f, 1023.f),
                aleatorioEntre(estado, 0.f, 1079.f),
                aleatorioEntre(estado, capa.velocidadMinima, capa.velocidadMaxima),
                tamanio,
                aleatorioEntre(estado, 0.f, 6.2831853f),
                brillo,
                siguienteAleatorio(estado) % 4u == 0u,
                tamanio >= 2.f && siguienteAleatorio(estado) % 3u == 0u
            });
        }
    }
}

void Juego::actualizarEstrellasFondo()
{
    for (auto& estrella : estrellasFondo_)
    {
        estrella.y += estrella.velocidadY;
        if (estrella.y > 1082.f)
            estrella.y = -estrella.tamanio * 2.f;
    }
}

void Juego::procesarAparicionesFondo()
{
    const auto& apariciones = obtenerAparicionesFondo();
    const float segundos = relojInicio_.getElapsedTime().asSeconds();

    while (proximaAparicionFondo_ < apariciones.size()
        && apariciones[proximaAparicionFondo_].tiempoSegundos <= segundos)
    {
        crearAparicionFondo(apariciones[proximaAparicionFondo_]);
        ++proximaAparicionFondo_;
    }
}

void Juego::crearAparicionFondo(const AparicionFondo& aparicion)
{
    float posicionY = -altoSectorTerreno;
    for (const auto& sector : tilesTerrenoFondo_)
    {
        if (sector.activo)
            posicionY = std::min(posicionY, sector.y - altoSectorTerreno);
    }

    for (int sector = 0; sector < aparicion.cantidad; ++sector)
    {
        const float desplazamientoX = aparicion.posicionXInicial
            + sector * aparicion.separacionX;
        tilesTerrenoFondo_.push_back({
            desplazamientoX,
            posicionY - sector * altoSectorTerreno,
            aparicion.velocidadY,
            true
        });
    }
}

void Juego::actualizarTerrenoFondo()
{
    for (auto& tile : tilesTerrenoFondo_)
    {
        if (!tile.activo)
            continue;

        tile.y += tile.velocidadY;
        if (tile.y > 1080.f)
            tile.activo = false;
    }
}

void Juego::procesarAparicionesElementosFondo()
{
    const auto& apariciones = obtenerAparicionesElementosFondo();
    const float segundos = relojInicio_.getElapsedTime().asSeconds();

    while (proximaAparicionElementoFondo_ < apariciones.size()
        && apariciones[proximaAparicionElementoFondo_].tiempoSegundos <= segundos)
    {
        crearAparicionElementoFondo(apariciones[proximaAparicionElementoFondo_]);
        ++proximaAparicionElementoFondo_;
    }
}

void Juego::crearAparicionElementoFondo(const AparicionElementoFondo& aparicion)
{
    for (int i = 0; i < aparicion.cantidad; ++i)
    {
        elementosFondo_.push_back({
            aparicion.posicionXInicial + i * aparicion.separacionX,
            -200.f,
            aparicion.velocidadY,
            aparicion.tipo,
            true
        });
    }
}

void Juego::actualizarElementosFondo()
{
    for (auto& elemento : elementosFondo_)
    {
        if (!elemento.activo)
            continue;

        elemento.y += elemento.velocidadY;
        if (elemento.y > 1280.f)
            elemento.activo = false;
    }

    elementosFondo_.erase(
        std::remove_if(
            elementosFondo_.begin(),
            elementosFondo_.end(),
            [](const ElementoFondo& elemento) { return !elemento.activo; }),
        elementosFondo_.end());
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
                if (enemigo->recibirDanio(proyectil.danio))
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
                if (enemigo->recibirDanio(proyectil.danio))
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
                if (esbirro->recibirDanio(proyectil.danio))
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
                if (molusco->recibirDanio(proyectil.danio))
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

        for (auto& pescado : pescadosGigantes_)
        {
            if (pescado->estaActivo())
            {
                const auto interseccion = limitesProyectil.findIntersection(
                    pescado->obtenerLimitesColision());
                if (!interseccion)
                    continue;
                crearImpactoLaser(*interseccion);
                proyectil.activo = false;
                if (pescado->recibirDanio(proyectil.danio))
                {
                    crearExplosionEnemigo(
                        TipoEnemigo::PescadoGigante,
                        pescado->obtenerLimitesColision());
                    pescado->desactivar();
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
                if (miniBoss->recibirDanio(proyectil.danio))
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

void Juego::actualizarFogonazosCanones()
{
    const float tiempoActual = relojInicio_.getElapsedTime().asSeconds();
    const float duracion = duracionFrameFogonazoCanon_ * 3.f;
    fogonazosCanones_.erase(
        std::remove_if(
            fogonazosCanones_.begin(),
            fogonazosCanones_.end(),
            [tiempoActual, duracion](const FogonazoCanon& fogonazo) {
                return tiempoActual - fogonazo.tiempoInicio >= duracion;
            }),
        fogonazosCanones_.end());
}

void Juego::crearFogonazoCanon(const sf::Vector2f& origen, bool laserAzul)
{
    fogonazosCanones_.push_back({
        {origen.x, origen.y + 20.f},
        relojInicio_.getElapsedTime().asSeconds(),
        laserAzul
    });
}

void Juego::disparar()
{
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        return;
    if (relojDisparo_.getElapsedTime().asSeconds() < cadenciaDisparo_)
        return;

    if (laserDobleActivo_)
    {
        for (const sf::Vector2f origen : {
            nave_.obtenerOrigenDisparoIzquierdo(),
            nave_.obtenerOrigenDisparoDerecho()})
        {
            proyectiles_.push_back({origen.x, origen.y, 2, true, true});
            crearFogonazoCanon(origen, true);
        }
    }
    else
    {
        const sf::Vector2f origen = nave_.obtenerOrigenDisparo();
        proyectiles_.push_back({origen.x, origen.y, 1, false, true});
        crearFogonazoCanon(origen, false);
    }
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

void Juego::procesarAparicionesItems()
{
    const auto& apariciones = obtenerAparicionesItems();
    const float segundos = relojInicio_.getElapsedTime().asSeconds();

    while (proximaAparicionItem_ < apariciones.size()
        && apariciones[proximaAparicionItem_].tiempoSegundos <= segundos)
    {
        crearAparicionItem(apariciones[proximaAparicionItem_]);
        ++proximaAparicionItem_;
    }
}

void Juego::crearAparicionItem(const AparicionItem& aparicion)
{
    for (int i = 0; i < aparicion.cantidad; ++i)
    {
        capsulasItems_.push_back({
            aparicion.posicionXInicial + i * aparicion.separacionX,
            -80.f,
            aparicion.velocidadY,
            std::max(1, aparicion.vida),
            aparicion.tipo,
            true
        });
    }
}

void Juego::actualizarCapsulasItems()
{
    for (auto& capsula : capsulasItems_)
    {
        if (!capsula.activo)
            continue;

        capsula.y += capsula.velocidadY;
        if (capsula.y > 1160.f)
            capsula.activo = false;
    }

    capsulasItems_.erase(
        std::remove_if(
            capsulasItems_.begin(),
            capsulasItems_.end(),
            [](const CapsulaItem& capsula) { return !capsula.activo; }),
        capsulasItems_.end());
}

void Juego::actualizarPowerUps()
{
    for (auto& powerUp : powerUps_)
    {
        if (!powerUp.activo)
            continue;

        powerUp.y += powerUp.velocidadY;
        if (powerUp.y > 1160.f)
            powerUp.activo = false;
    }

    powerUps_.erase(
        std::remove_if(
            powerUps_.begin(),
            powerUps_.end(),
            [](const PowerUp& powerUp) { return !powerUp.activo; }),
        powerUps_.end());
}

void Juego::detectarColisionesProyectilesCapsulas()
{
    for (auto& proyectil : proyectiles_)
    {
        if (!proyectil.activo)
            continue;

        const sf::FloatRect limitesProyectil = obtenerLimitesProyectilJugador(proyectil);
        for (auto& capsula : capsulasItems_)
        {
            if (!capsula.activo)
                continue;

            const auto interseccion = limitesProyectil.findIntersection(
                obtenerLimitesCapsulaItem(capsula));
            if (!interseccion)
                continue;

            crearImpactoLaser(*interseccion);
            proyectil.activo = false;
            capsula.vida -= proyectil.danio;
            if (capsula.vida <= 0)
            {
                crearPowerUp(capsula);
                capsula.activo = false;
            }
            break;
        }
    }
}

void Juego::crearPowerUp(const CapsulaItem& capsula)
{
    powerUps_.push_back({
        capsula.x,
        capsula.y,
        velocidadPowerUp_,
        capsula.tipo,
        true
    });
}

void Juego::detectarColisionesPowerUps()
{
    const sf::FloatRect limitesNave = nave_.obtenerLimitesColision();
    for (auto& powerUp : powerUps_)
    {
        if (!powerUp.activo
            || !limitesNave.findIntersection(obtenerLimitesPowerUp(powerUp)))
        {
            continue;
        }

        if (powerUp.tipo == TipoItem::CapsulaLaserDoble)
            laserDobleActivo_ = true;
        powerUp.activo = false;
    }
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
    const auto& comportamiento = obtenerComportamientoEnemigo(oleada.comportamiento);

    for (int i = 0; i < oleada.cantidad; ++i)
    {
        const float posicionX = oleada.posicionXInicial + i * oleada.separacionX;

        if (oleada.tipo == TipoEnemigo::Nave)
        {
            auto enemigo = std::make_unique<Enemigo>(texturaEnemigo_);
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
            auto enemigo = std::make_unique<EnemigoAlien>(
                texturaEnemigoAlienFrame1_,
                texturaEnemigoAlienFrame2_);
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
            auto esbirro = std::make_unique<Esbirro>(texturaEsbirro_);
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
            auto molusco = std::make_unique<MoluscoGiratorio>(texturaMoluscoGiratorio_);
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
        else if (oleada.tipo == TipoEnemigo::PescadoGigante)
        {
            auto pescado = std::make_unique<PescadoGigante>(texturaPescadoGigante_);
            pescado->configurarVelocidad(comportamiento.movimiento.velocidadVertical);
            pescado->activar(
                posicionX,
                std::clamp(oleada.danio, 1, 3),
                std::max(1, oleada.vida),
                std::max(0.1f, oleada.frecuenciaDisparo));
            pescadosGigantes_.push_back(std::move(pescado));
        }
        else
        {
            auto miniBoss = std::make_unique<MiniBossMolusco>(
                texturaMiniBossMoluscoFrame1_,
                texturaMiniBossMoluscoFrame2_,
                texturaMiniBossMoluscoFrame3_);
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

    for (auto& pescado : pescadosGigantes_)
    {
        if (!pescado->listoParaDisparar())
            continue;

        const bool patronDiagonal = pescado->usarPatronDiagonal();
        const std::initializer_list<sf::Vector2f> velocidades = patronDiagonal
            ? std::initializer_list<sf::Vector2f>{{-4.2f, -4.2f}, {4.2f, -4.2f}, {-4.2f, 4.2f}, {4.2f, 4.2f}}
            : std::initializer_list<sf::Vector2f>{{0.f, -6.f}, {0.f, 6.f}, {-6.f, 0.f}, {6.f, 0.f}};

        for (const sf::Vector2f origen : pescado->obtenerOrigenesDisparo())
        {
            for (const sf::Vector2f velocidad : velocidades)
            {
                proyectilesEnemigos_.push_back({
                    origen.x,
                    origen.y,
                    velocidad.x,
                    velocidad.y,
                    obtenerConfiguracionProyectil(TipoProyectilEnemigo::EscamaMetalica).danio,
                    TipoProyectilEnemigo::EscamaMetalica,
                    true
                });
            }
        }
    }
}

void Juego::actualizarProyectilesEnemigos()
{
    for (auto& proyectil : proyectilesEnemigos_)
    {
        proyectil.x += proyectil.velocidadX;
        proyectil.y += proyectil.velocidadY;

        if (proyectil.x < -100.f || proyectil.x > 1124.f
            || proyectil.y < -100.f || proyectil.y > 1180.f)
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
    for (auto& pescado : pescadosGigantes_)
        pescado->actualizar();

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
    pescadosGigantes_.erase(
        std::remove_if(
            pescadosGigantes_.begin(),
            pescadosGigantes_.end(),
            [](const auto& pescado) { return !pescado->estaActivo(); }),
        pescadosGigantes_.end());
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

    for (auto& pescado : pescadosGigantes_)
    {
        if (pescado->estaActivo()
            && limitesNave.findIntersection(pescado->obtenerLimitesColision()))
        {
            pescado->desactivar();
            vidaNave_ = std::max(0, vidaNave_ - pescado->obtenerDanio());
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
    dibujarEstrellasFondo();
    dibujarTerrenoFondo();
    dibujarElementosFondo();
    dibujarCapsulasItems();
    dibujarPowerUps();
    dibujarEnemigos();
    if (!naveExplotando_ && !esperandoGameOver_ && !gameOver_)
        nave_.dibujar(window_);
    dibujarFogonazosCanones();

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

void Juego::dibujarEstrellasFondo()
{
    const float tiempo = relojInicio_.getElapsedTime().asSeconds();

    for (const auto& estrella : estrellasFondo_)
    {
        float brillo = estrella.brilloBase;
        if (estrella.titila)
            brillo *= 0.62f + 0.38f * (std::sin(tiempo * 4.5f + estrella.faseTitileo) + 1.f) / 2.f;

        const auto alpha = static_cast<std::uint8_t>(std::clamp(brillo, 0.f, 255.f));
        const sf::Color color(190, 220, 255, alpha);
        sf::RectangleShape centro({estrella.tamanio, estrella.tamanio});
        centro.setFillColor(color);
        centro.setPosition({estrella.x, estrella.y});
        window_.draw(centro);

        if (!estrella.cruz)
            continue;

        const sf::Color halo(140, 190, 255, static_cast<std::uint8_t>(alpha / 2));
        sf::RectangleShape horizontal({estrella.tamanio * 3.f, 1.f});
        horizontal.setFillColor(halo);
        horizontal.setPosition({estrella.x - estrella.tamanio, estrella.y + estrella.tamanio / 2.f});
        window_.draw(horizontal);

        sf::RectangleShape vertical({1.f, estrella.tamanio * 3.f});
        vertical.setFillColor(halo);
        vertical.setPosition({estrella.x + estrella.tamanio / 2.f, estrella.y - estrella.tamanio});
        window_.draw(vertical);
    }
}

void Juego::dibujarTerrenoFondo()
{
    for (const auto& sector : tilesTerrenoFondo_)
    {
        if (!sector.activo)
            continue;

        sf::Sprite sprite(texturaSectorTerrenoFondo_);
        sprite.setPosition({std::round(sector.x), std::round(sector.y)});
        window_.draw(sprite);
    }

}

void Juego::dibujarElementosFondo()
{
    for (const auto& elemento : elementosFondo_)
    {
        if (!elemento.activo)
            continue;

        sf::Sprite sprite(texturaAtlasRocasFondo_);
        sprite.setTextureRect(obtenerRectanguloTileFondo(elemento.tipo));
        sprite.setScale({escalaElementosFondo_, escalaElementosFondo_});
        const sf::FloatRect limites = sprite.getGlobalBounds();
        sprite.setPosition({
            elemento.x - limites.size.x / 2.f,
            elemento.y - limites.size.y / 2.f
        });
        window_.draw(sprite);
    }
}

void Juego::dibujarCapsulasItems()
{
    for (const auto& capsula : capsulasItems_)
    {
        if (!capsula.activo)
            continue;

        sf::Sprite sprite(obtenerTexturaCapsulaItem());
        sprite.setScale({escalaCapsulaItem_, escalaCapsulaItem_});
        const sf::FloatRect limites = sprite.getGlobalBounds();
        sprite.setPosition({
            capsula.x - limites.size.x / 2.f,
            capsula.y - limites.size.y / 2.f
        });
        window_.draw(sprite);
    }
}

void Juego::dibujarPowerUps()
{
    for (const auto& powerUp : powerUps_)
    {
        if (!powerUp.activo)
            continue;

        sf::Sprite sprite(texturaPowerUpP_);
        sprite.setScale({escalaPowerUp_, escalaPowerUp_});
        const sf::FloatRect limites = sprite.getGlobalBounds();
        sprite.setPosition({
            powerUp.x - limites.size.x / 2.f,
            powerUp.y - limites.size.y / 2.f
        });
        window_.draw(sprite);
    }
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

void Juego::dibujarFogonazosCanones()
{
    const float tiempoActual = relojInicio_.getElapsedTime().asSeconds();

    for (const auto& fogonazo : fogonazosCanones_)
    {
        const int frame = static_cast<int>(
            (tiempoActual - fogonazo.tiempoInicio) / duracionFrameFogonazoCanon_);
        if (frame < 0 || frame >= 3)
            continue;

        const float radio = 3.f + frame * 2.f;
        const std::uint8_t alpha = static_cast<std::uint8_t>(255 - frame * 70);
        const sf::Color colorExterior = fogonazo.laserAzul
            ? sf::Color(30, 150, 255, alpha)
            : sf::Color(255, 145, 35, alpha);
        const sf::Color colorInterior = fogonazo.laserAzul
            ? sf::Color(190, 245, 255, alpha)
            : sf::Color(255, 245, 170, alpha);

        sf::RectangleShape horizontal({radio * 2.f, 3.f});
        horizontal.setFillColor(colorExterior);
        horizontal.setPosition({fogonazo.centro.x - radio, fogonazo.centro.y - 1.f});
        window_.draw(horizontal);

        sf::RectangleShape vertical({3.f, radio * 2.f});
        vertical.setFillColor(colorExterior);
        vertical.setPosition({fogonazo.centro.x - 1.f, fogonazo.centro.y - radio});
        window_.draw(vertical);

        sf::RectangleShape nucleo({3.f, 3.f});
        nucleo.setFillColor(colorInterior);
        nucleo.setPosition({fogonazo.centro.x - 1.f, fogonazo.centro.y - 1.f});
        window_.draw(nucleo);
    }
}

void Juego::dibujarExplosionesEnemigos()
{
    const float tiempoActual = relojInicio_.getElapsedTime().asSeconds();

    for (const auto& explosion : explosionesEnemigos_)
    {
        const float edad = tiempoActual - explosion.tiempoInicio;
        if (edad >= duracionExplosionEnemigo_)
            continue;

        const float progreso = std::clamp(edad / duracionExplosionEnemigo_, 0.f, 1.f);
        const float escalaBase = explosion.tipo == TipoEnemigo::Alien
            ? 0.1f
            : explosion.tipo == TipoEnemigo::MiniBossMolusco
                ? 0.22f
                : explosion.tipo == TipoEnemigo::PescadoGigante
                    ? 0.16f
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
    for (const auto& pescado : pescadosGigantes_)
        pescado->dibujar(window_);
}

void Juego::dibujarProyectilEnemigo(const ProyectilEnemigo& proyectil)
{
    const sf::Texture& textura = obtenerTexturaProyectilEnemigo(proyectil.tipo);
    const float escala = obtenerConfiguracionProyectil(proyectil.tipo).escala;

    sf::Sprite sprite(textura);
    sprite.setScale({escala, escala});
    if (proyectil.tipo == TipoProyectilEnemigo::EscamaMetalica)
    {
        sprite.setOrigin({
            textura.getSize().x / 2.f,
            textura.getSize().y / 2.f
        });
        const float angulo = std::atan2(proyectil.velocidadY, proyectil.velocidadX)
            * 180.f / 3.14159265f + 90.f;
        sprite.setRotation(sf::degrees(angulo));
    }
    sprite.setPosition({proyectil.x, proyectil.y});
    window_.draw(sprite);
}

sf::FloatRect Juego::obtenerLimitesProyectilEnemigo(const ProyectilEnemigo& proyectil) const
{
    const sf::Texture& textura = obtenerTexturaProyectilEnemigo(proyectil.tipo);
    const float escala = obtenerConfiguracionProyectil(proyectil.tipo).escala;

    sf::Sprite sprite(textura);
    sprite.setScale({escala, escala});
    if (proyectil.tipo == TipoProyectilEnemigo::EscamaMetalica)
    {
        sprite.setOrigin({
            textura.getSize().x / 2.f,
            textura.getSize().y / 2.f
        });
        const float angulo = std::atan2(proyectil.velocidadY, proyectil.velocidadX)
            * 180.f / 3.14159265f + 90.f;
        sprite.setRotation(sf::degrees(angulo));
    }
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
          << "\nFPS: " << fps_
          << "\nPeor frame: " << peorFrameVisibleMs_ << " ms"
          << "\nTexturas cargadas: " << texturasCargadas_
          << "\nVida: " << vidaNave_ << "/3"
          << "\nImpactos: " << impactosNave_
          << "\nLaser: " << (laserDobleActivo_ ? "doble azul" : "normal");
    if (vidaNave_ <= 0)
        texto << "\nGAME OVER";
    else if (pausado_)
        texto << "\nPAUSA";
    textoDebug_.setString(texto.str());
    window_.draw(textoDebug_);
}

void Juego::dibujarProyectil(const Proyectil& proyectil)
{
    sf::Sprite sprite(obtenerTexturaProyectilJugador(proyectil));
    const float escala = proyectil.laserAzul
        ? escalaLaserJugadorAzul_
        : escalaLaserJugador_;
    sprite.setScale({escala, escala});
    sprite.setPosition({proyectil.x, proyectil.y});
    window_.draw(sprite);
}

sf::FloatRect Juego::obtenerLimitesProyectilJugador(const Proyectil& proyectil) const
{
    const sf::Vector2f tamanio = proyectil.laserAzul
        ? tamanioLaserJugadorAzul_
        : tamanioLaserJugador_;
    return {{proyectil.x, proyectil.y}, tamanio};
}

const sf::Texture& Juego::obtenerTexturaProyectilJugador(const Proyectil& proyectil) const
{
    return proyectil.laserAzul ? texturaLaserJugadorAzul_ : texturaLaserJugador_;
}

const sf::Texture& Juego::obtenerTexturaCapsulaItem() const
{
    const int frame = static_cast<int>(
        relojInicio_.getElapsedTime().asSeconds() / 0.18f) % 2;
    return frame == 0 ? texturaCapsulaItemFrame1_ : texturaCapsulaItemFrame2_;
}

sf::FloatRect Juego::obtenerLimitesCapsulaItem(const CapsulaItem& capsula) const
{
    sf::Sprite sprite(obtenerTexturaCapsulaItem());
    sprite.setScale({escalaCapsulaItem_, escalaCapsulaItem_});
    const sf::FloatRect limites = sprite.getGlobalBounds();
    sprite.setPosition({
        capsula.x - limites.size.x / 2.f,
        capsula.y - limites.size.y / 2.f
    });
    return sprite.getGlobalBounds();
}

sf::FloatRect Juego::obtenerLimitesPowerUp(const PowerUp& powerUp) const
{
    sf::Sprite sprite(texturaPowerUpP_);
    sprite.setScale({escalaPowerUp_, escalaPowerUp_});
    const sf::FloatRect limites = sprite.getGlobalBounds();
    sprite.setPosition({
        powerUp.x - limites.size.x / 2.f,
        powerUp.y - limites.size.y / 2.f
    });
    return sprite.getGlobalBounds();
}

sf::IntRect Juego::obtenerRectanguloTileFondo(TipoElementoFondo tipo) const
{
    constexpr int columnas = 5;
    constexpr int anchoTile = 396;
    constexpr int altoTile = 396;
    const int indice = static_cast<int>(tipo);
    return {
        {indice % columnas * anchoTile, indice / columnas * altoTile},
        {anchoTile, altoTile}
    };
}
