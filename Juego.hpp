#pragma once

#include "ConfiguracionEnemigos.hpp"
#include "ConfiguracionProyectiles.hpp"
#include "Personajes.hpp"

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <map>
#include <memory>
#include <vector>

struct Proyectil
{
    float x;
    float y;
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

class Juego {
public:
    Juego();
    int ejecutar();

private:
    void procesarEventos();
    void reiniciar();
    void iniciarExplosionNave();
    void actualizarExplosionNave();
    void actualizar();
    void disparar();
    void actualizarProyectiles();
    void procesarApariciones();
    void crearOleada(const OleadaEnemigos& oleada);
    void actualizarEnemigos();
    void dispararEnemigos();
    void actualizarProyectilesEnemigos();
    void detectarColisionesConNave();
    void dibujar();
    void dibujarEnemigos();
    void dibujarProyectilesEnemigos();
    void dibujarDebug();
    void dibujarGameOver();
    void dibujarExplosionNave();
    void dibujarProyectil(const Proyectil& proyectil);
    void dibujarProyectilEnemigo(const ProyectilEnemigo& proyectil);
    sf::FloatRect obtenerLimitesProyectilEnemigo(const ProyectilEnemigo& proyectil) const;
    const sf::Texture& obtenerTexturaProyectilEnemigo(TipoProyectilEnemigo tipo) const;

    sf::RenderWindow window_;
    Nave nave_;
    std::vector<std::unique_ptr<Enemigo>> enemigos_;
    std::vector<std::unique_ptr<EnemigoAlien>> enemigosAlien_;
    std::vector<Proyectil> proyectiles_;
    std::vector<ProyectilEnemigo> proyectilesEnemigos_;
    sf::Clock relojDisparo_;
    sf::Clock relojInicio_;
    sf::Font fuenteDebug_;
    sf::Text textoDebug_{fuenteDebug_};
    std::map<TipoProyectilEnemigo, sf::Texture> texturasProyectilesEnemigos_;
    sf::Texture texturaGameOver_;
    std::vector<sf::Texture> texturasExplosionNave_;
    sf::Clock relojExplosionNave_;
    sf::Clock relojEsperaGameOver_;
    std::size_t proximaOleada_ = 0;
    int impactosNave_ = 0;
    int vidaNave_ = 3;
    int frameExplosionNave_ = 0;
    bool gameOver_ = false;
    bool naveExplotando_ = false;
    bool esperandoGameOver_ = false;
    sf::Vector2f centroExplosionNave_;

    float cadenciaDisparo_ = 0.12f;
    float velocidadLaser_ = 8.f;
    float escalaGameOver_ = 0.62f;
    float escalaExplosionNave_ = 0.22f;
    float duracionFrameExplosionNave_ = 0.16f;
    float esperaAntesGameOver_ = 2.f;
};
