#pragma once

#include <SFML/Graphics.hpp>

class Nave {
public:
    bool cargarTexturas();
    void actualizar();
    void dibujar(sf::RenderWindow& window) const;
    sf::Vector2f obtenerOrigenDisparo() const;
    sf::Vector2f obtenerCentro() const;
    sf::FloatRect obtenerLimitesColision() const;
    bool esInvulnerable() const;
    void recibirDanio();
    void reiniciarEstado();
    void establecerPausa(bool pausado);

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
    sf::Clock relojInvulnerabilidad_;
    bool invulnerable_ = false;
    float duracionInvulnerabilidad_ = 1.f;
    float intervaloParpadeo_ = 0.08f;

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

class Enemigo {
public:
    bool cargarTextura();
    void activar(float posicionX, int danio, int vida, float frecuenciaDisparo);
    void configurarMovimientoDiagonal(
        float velocidadHorizontal,
        float velocidadVertical);
    void actualizar();
    void dibujar(sf::RenderWindow& window) const;
    bool estaActivo() const;
    void desactivar();
    int obtenerDanio() const;
    bool recibirDanio(int danio);
    bool listoParaDisparar();
    sf::Vector2f obtenerOrigenDisparo() const;
    sf::FloatRect obtenerLimitesColision() const;
    void establecerPausa(bool pausado);

private:
    float y_ = -100.f;
    float velocidadHorizontal_ = 2.2f;
    float velocidadVertical_ = 2.5f;
    float escala_ = 0.13f;
    bool activo_ = false;
    int danio_ = 1;
    int vida_ = 2;
    float frecuenciaDisparo_ = 1.f;
    sf::Clock relojDisparo_;

    sf::Texture textura_;
    sf::Sprite sprite_{textura_};
};

class EnemigoAlien {
public:
    bool cargarTexturas();
    void activar(float posicionX, int danio, int vida, float frecuenciaDisparo);
    void configurarMovimientoCoseno(
        float amplitud,
        float velocidadVertical,
        float velocidadOscilacion);
    void actualizar();
    void dibujar(sf::RenderWindow& window) const;
    bool estaActivo() const;
    void desactivar();
    int obtenerDanio() const;
    bool recibirDanio(int danio);
    bool listoParaDisparar();
    sf::Vector2f obtenerOrigenDisparo() const;
    sf::FloatRect obtenerLimitesColision() const;
    void establecerPausa(bool pausado);

private:
    void actualizarAnimacion();

    float posicionXInicial_ = 0.f;
    float y_ = -100.f;
    float fase_ = 0.f;
    float amplitud_ = 300.f;
    float velocidadVertical_ = 1.8f;
    float velocidadOscilacion_ = 0.025f;
    float escala_ = 0.12f;
    int frame_ = 0;
    bool activo_ = false;
    int danio_ = 1;
    int vida_ = 3;
    float frecuenciaDisparo_ = 1.f;

    sf::Clock relojAnimacion_;
    sf::Clock relojDisparo_;
    sf::Texture texturaFrame1_;
    sf::Texture texturaFrame2_;
    sf::Sprite sprite_{texturaFrame1_};
};

class Esbirro {
public:
    bool cargarTextura();
    void activar(float posicionX, int danio, int vida, float frecuenciaDisparo);
    void configurarVelocidad(float velocidadVertical);
    void actualizar();
    void dibujar(sf::RenderWindow& window) const;
    bool estaActivo() const;
    void desactivar();
    int obtenerDanio() const;
    bool recibirDanio(int danio);
    bool listoParaDisparar();
    sf::Vector2f obtenerOrigenDisparo() const;
    sf::FloatRect obtenerLimitesColision() const;
    void establecerPausa(bool pausado);

private:
    float y_ = -100.f;
    float velocidadVertical_ = 5.2f;
    float escala_ = 0.1f;
    bool activo_ = false;
    int danio_ = 1;
    int vida_ = 1;
    float frecuenciaDisparo_ = 1.1f;

    sf::Clock relojDisparo_;
    sf::Texture textura_;
    sf::Sprite sprite_{textura_};
};
