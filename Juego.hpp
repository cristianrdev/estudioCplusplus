#pragma once

#include "src/orquestacion/OrquestacionEnemigos.hpp"
#include "src/orquestacion/OrquestacionItems.hpp"
#include "ConfiguracionProyectiles.hpp"
#include "Personajes.hpp"

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <vector>

struct Proyectil
{
    float x;
    float y;
    int danio;
    bool laserAzul;
    bool activo;
};

struct ProyectilEnemigo
{
    float x;
    float y;
    float velocidadX;
    float velocidadY;
    int danio;
    TipoProyectilEnemigo tipo;
    bool activo;
};

struct ExplosionEnemigo
{
    sf::Vector2f centro;
    TipoEnemigo tipo;
    float tiempoInicio;
};

struct ImpactoLaser
{
    sf::Vector2f centro;
    float tiempoInicio;
};

struct CapsulaItem
{
    float x;
    float y;
    float velocidadY;
    int vida;
    TipoItem tipo;
    bool activo;
};

struct PowerUp
{
    float x;
    float y;
    float velocidadY;
    TipoItem tipo;
    bool activo;
};

class Juego {
public:
    Juego();
    int ejecutar();

private:
    void procesarEventos();
    void alternarPausa();
    void reiniciar();
    void iniciarExplosionNave();
    void actualizarExplosionNave();
    void actualizar();
    void disparar();
    void actualizarProyectiles();
    void detectarColisionesProyectilesJugador();
    void procesarAparicionesItems();
    void crearAparicionItem(const AparicionItem& aparicion);
    void actualizarCapsulasItems();
    void actualizarPowerUps();
    void detectarColisionesProyectilesCapsulas();
    void detectarColisionesPowerUps();
    void crearPowerUp(const CapsulaItem& capsula);
    void actualizarExplosionesEnemigos();
    void actualizarImpactosLaser();
    void crearExplosionEnemigo(TipoEnemigo tipo, const sf::FloatRect& limites);
    void crearImpactoLaser(const sf::FloatRect& interseccion);
    void procesarApariciones();
    void registrarOleadaDebug(const OleadaEnemigos& oleada, float tiempoReal);
    void crearOleada(const OleadaEnemigos& oleada);
    void actualizarEnemigos();
    void dispararEnemigos();
    void actualizarProyectilesEnemigos();
    void detectarColisionesConNave();
    void dibujar();
    void dibujarCapsulasItems();
    void dibujarPowerUps();
    void dibujarEnemigos();
    void dibujarProyectilesEnemigos();
    void dibujarExplosionesEnemigos();
    void dibujarImpactosLaser();
    void dibujarDebug();
    void dibujarGameOver();
    void dibujarExplosionNave();
    void dibujarProyectil(const Proyectil& proyectil);
    sf::FloatRect obtenerLimitesProyectilJugador(const Proyectil& proyectil) const;
    const sf::Texture& obtenerTexturaProyectilJugador(const Proyectil& proyectil) const;
    const sf::Texture& obtenerTexturaCapsulaItem() const;
    sf::FloatRect obtenerLimitesCapsulaItem(const CapsulaItem& capsula) const;
    sf::FloatRect obtenerLimitesPowerUp(const PowerUp& powerUp) const;
    void dibujarProyectilEnemigo(const ProyectilEnemigo& proyectil);
    sf::FloatRect obtenerLimitesProyectilEnemigo(const ProyectilEnemigo& proyectil) const;
    const sf::Texture& obtenerTexturaProyectilEnemigo(TipoProyectilEnemigo tipo) const;

    sf::RenderWindow window_;
    Nave nave_;
    std::vector<std::unique_ptr<Enemigo>> enemigos_;
    std::vector<std::unique_ptr<EnemigoAlien>> enemigosAlien_;
    std::vector<std::unique_ptr<Esbirro>> esbirros_;
    std::vector<std::unique_ptr<MoluscoGiratorio>> moluscosGiratorios_;
    std::vector<std::unique_ptr<MiniBossMolusco>> miniBossesMolusco_;
    std::vector<Proyectil> proyectiles_;
    std::vector<ProyectilEnemigo> proyectilesEnemigos_;
    std::vector<CapsulaItem> capsulasItems_;
    std::vector<PowerUp> powerUps_;
    std::vector<ExplosionEnemigo> explosionesEnemigos_;
    std::vector<ImpactoLaser> impactosLaser_;
    std::vector<std::string> ultimasOleadasDebug_;
    sf::Clock relojDisparo_;
    sf::Clock relojInicio_;
    sf::Font fuenteDebug_;
    sf::Text textoDebug_{fuenteDebug_};
    std::map<TipoProyectilEnemigo, sf::Texture> texturasProyectilesEnemigos_;
    std::map<TipoEnemigo, sf::Texture> texturasExplosionesEnemigos_;
    sf::Texture texturaLaserJugador_;
    sf::Texture texturaLaserJugadorAzul_;
    sf::Texture texturaCapsulaItemFrame1_;
    sf::Texture texturaCapsulaItemFrame2_;
    sf::Texture texturaPowerUpP_;
    sf::Texture texturaGameOver_;
    std::vector<sf::Texture> texturasExplosionNave_;
    std::vector<sf::Texture> texturasImpactoLaser_;
    sf::Clock relojExplosionNave_;
    sf::Clock relojEsperaGameOver_;
    std::size_t proximaOleada_ = 0;
    std::size_t proximaAparicionItem_ = 0;
    int impactosNave_ = 0;
    int vidaNave_ = 3;
    int frameExplosionNave_ = 0;
    bool gameOver_ = false;
    bool naveExplotando_ = false;
    bool esperandoGameOver_ = false;
    bool pausado_ = false;
    bool laserDobleActivo_ = false;
    sf::Vector2f centroExplosionNave_;

    float cadenciaDisparo_ = 0.12f;
    float velocidadLaser_ = 8.f;
    float escalaLaserJugador_ = 0.035f;
    float escalaLaserJugadorAzul_ = 0.035f;
    float escalaCapsulaItem_ = 0.07f;
    float escalaPowerUp_ = 0.07f;
    float velocidadPowerUp_ = 1.8f;
    float duracionExplosionEnemigo_ = 0.45f;
    float duracionFrameImpactoLaser_ = 0.09f;
    float escalaImpactoLaser_ = 0.08f;
    float escalaGameOver_ = 0.62f;
    float escalaExplosionNave_ = 0.22f;
    float duracionFrameExplosionNave_ = 0.16f;
    float esperaAntesGameOver_ = 2.f;
};
