#include "Personajes.hpp"

#include <cmath>

bool Nave::cargarTexturas()
{
    if (!texturaCentro_.loadFromFile("assets/centro.png"))
        return false;
    if (!texturaIzquierda_.loadFromFile("assets/izquierda.png"))
        return false;
    if (!texturaDerecha_.loadFromFile("assets/derecha.png"))
        return false;
    if (!fuegoTex1_.loadFromFile("assets/fuego1.png"))
        return false;
    if (!fuegoTex2_.loadFromFile("assets/fuego2.png"))
        return false;
    if (!fuegoTex3_.loadFromFile("assets/fuego3.png"))
        return false;

    spriteNave_.setTexture(texturaCentro_, true);
    fuegoIzq_.setTexture(fuegoTex1_, true);
    fuegoDer_.setTexture(fuegoTex1_, true);
    actualizarPosiciones();
    return true;
}

void Nave::actualizar()
{
    if (invulnerable_
        && relojInvulnerabilidad_.getElapsedTime().asSeconds() >= duracionInvulnerabilidad_)
    {
        invulnerable_ = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
    {
        y_ -= velocidad_;
        largoFuego_ = 0.1f;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
    {
        y_ += velocidad_;
        largoFuego_ = 0.05f;
    }
    else
    {
        largoFuego_ = escalaFuego_;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        x_ -= velocidad_;
        spriteNave_.setTexture(texturaIzquierda_, true);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        x_ += velocidad_;
        spriteNave_.setTexture(texturaDerecha_, true);
    }
    else
    {
        spriteNave_.setTexture(texturaCentro_, true);
    }

    actualizarAnimacionFuego();
    actualizarPosiciones();
}

void Nave::dibujar(sf::RenderWindow& window) const
{
    if (invulnerable_)
    {
        const float segundos = relojInvulnerabilidad_.getElapsedTime().asSeconds();
        const int faseParpadeo = static_cast<int>(segundos / intervaloParpadeo_);
        if (faseParpadeo % 2 != 0)
            return;
    }

    window.draw(fuegoIzq_);
    window.draw(fuegoDer_);
    window.draw(spriteNave_);
}

sf::Vector2f Nave::obtenerOrigenDisparo() const
{
    const float anchoNave = spriteNave_.getGlobalBounds().size.x;
    return {x_ + (anchoNave / 2.f) - 4.f, y_ - 20.f};
}

sf::Vector2f Nave::obtenerCentro() const
{
    const sf::FloatRect limites = spriteNave_.getGlobalBounds();
    return {
        limites.position.x + limites.size.x / 2.f,
        limites.position.y + limites.size.y / 2.f
    };
}

sf::FloatRect Nave::obtenerLimitesColision() const
{
    return spriteNave_.getGlobalBounds();
}

bool Nave::esInvulnerable() const
{
    return invulnerable_;
}

void Nave::recibirDanio()
{
    invulnerable_ = true;
    relojInvulnerabilidad_.restart();
}

void Nave::reiniciarEstado()
{
    invulnerable_ = false;
}

void Nave::establecerPausa(bool pausado)
{
    if (pausado)
    {
        relojFuego_.stop();
        relojInvulnerabilidad_.stop();
    }
    else
    {
        relojFuego_.start();
        relojInvulnerabilidad_.start();
    }
}

void Nave::actualizarAnimacionFuego()
{
    if (relojFuego_.getElapsedTime().asSeconds() < 0.07f)
        return;

    frameFuego_ = (frameFuego_ + 1) % 3;

    if (frameFuego_ == 0)
    {
        fuegoIzq_.setTexture(fuegoTex1_, true);
        fuegoDer_.setTexture(fuegoTex1_, true);
    }
    else if (frameFuego_ == 1)
    {
        fuegoIzq_.setTexture(fuegoTex2_, true);
        fuegoDer_.setTexture(fuegoTex2_, true);
    }
    else
    {
        fuegoIzq_.setTexture(fuegoTex3_, true);
        fuegoDer_.setTexture(fuegoTex3_, true);
    }

    relojFuego_.restart();
}

void Nave::actualizarPosiciones()
{
    spriteNave_.setScale({escalaNave_, escalaNave_});
    spriteNave_.setPosition({x_, y_});

    const float anchoNave = spriteNave_.getGlobalBounds().size.x;
    const float altoNave = spriteNave_.getGlobalBounds().size.y;

    fuegoIzq_.setScale({escalaFuego_, largoFuego_});
    fuegoDer_.setScale({escalaFuego_, largoFuego_});
    fuegoIzq_.setPosition({x_ + 6.f, y_ + altoNave - 14.f});
    fuegoDer_.setPosition({x_ + anchoNave - 19.f, y_ + altoNave - 14.f});
}

bool Enemigo::cargarTextura()
{
    if (!textura_.loadFromFile("assets/enemigo.png"))
        return false;

    sprite_.setTexture(textura_, true);
    sprite_.setScale({escala_, escala_});
    sprite_.setOrigin({
        textura_.getSize().x / 2.f,
        textura_.getSize().y / 2.f
    });
    sprite_.setRotation(sf::degrees(180.f));
    return true;
}

void Enemigo::activar(float posicionX, int danio, int vida, float frecuenciaDisparo)
{
    y_ = -sprite_.getGlobalBounds().size.y;
    activo_ = true;
    danio_ = danio;
    vida_ = vida;
    frecuenciaDisparo_ = frecuenciaDisparo;
    relojDisparo_.restart();
    sprite_.setPosition({posicionX, y_});
}

void Enemigo::configurarMovimientoDiagonal(
    float velocidadHorizontal,
    float velocidadVertical)
{
    velocidadHorizontal_ = velocidadHorizontal;
    velocidadVertical_ = velocidadVertical;
}

void Enemigo::actualizar()
{
    if (!activo_)
        return;

    y_ += velocidadVertical_;
    const float x = sprite_.getPosition().x + velocidadHorizontal_;
    sprite_.setPosition({x, y_});

    const sf::FloatRect limites = sprite_.getGlobalBounds();
    if (limites.position.x + limites.size.x < 0.f
        || limites.position.x > 1024.f
        || limites.position.y > 1080.f)
    {
        activo_ = false;
    }
}

void Enemigo::dibujar(sf::RenderWindow& window) const
{
    if (activo_)
        window.draw(sprite_);
}

bool Enemigo::estaActivo() const
{
    return activo_;
}

void Enemigo::desactivar()
{
    activo_ = false;
}

int Enemigo::obtenerDanio() const
{
    return danio_;
}

bool Enemigo::recibirDanio(int danio)
{
    vida_ -= danio;
    return vida_ <= 0;
}

bool Enemigo::listoParaDisparar()
{
    if (!activo_ || relojDisparo_.getElapsedTime().asSeconds() < frecuenciaDisparo_)
        return false;

    relojDisparo_.restart();
    return true;
}

sf::Vector2f Enemigo::obtenerOrigenDisparo() const
{
    const sf::FloatRect limites = sprite_.getGlobalBounds();
    return {
        limites.position.x + limites.size.x / 2.f,
        limites.position.y + limites.size.y
    };
}

sf::FloatRect Enemigo::obtenerLimitesColision() const
{
    return sprite_.getGlobalBounds();
}

void Enemigo::establecerPausa(bool pausado)
{
    if (pausado)
        relojDisparo_.stop();
    else
        relojDisparo_.start();
}

bool EnemigoAlien::cargarTexturas()
{
    if (!texturaFrame1_.loadFromFile("assets/enemigo_alien.png"))
        return false;
    if (!texturaFrame2_.loadFromFile("assets/enemigo_alien_2.png"))
        return false;

    sprite_.setTexture(texturaFrame1_, true);
    sprite_.setScale({escala_, escala_});
    return true;
}

void EnemigoAlien::activar(float posicionX, int danio, int vida, float frecuenciaDisparo)
{
    posicionXInicial_ = posicionX;
    y_ = -sprite_.getGlobalBounds().size.y;
    fase_ = 0.f;
    activo_ = true;
    danio_ = danio;
    vida_ = vida;
    frecuenciaDisparo_ = frecuenciaDisparo;
    relojDisparo_.restart();
    sprite_.setPosition({posicionXInicial_, y_});
}

void EnemigoAlien::configurarMovimientoCoseno(
    float amplitud,
    float velocidadVertical,
    float velocidadOscilacion)
{
    amplitud_ = amplitud;
    velocidadVertical_ = velocidadVertical;
    velocidadOscilacion_ = velocidadOscilacion;
}

void EnemigoAlien::actualizar()
{
    if (!activo_)
        return;

    actualizarAnimacion();

    fase_ += velocidadOscilacion_;
    y_ += velocidadVertical_;

    const float x = posicionXInicial_ + amplitud_ * (std::cos(fase_) - 1.f);
    sprite_.setPosition({x, y_});

    if (y_ > 1080.f)
        activo_ = false;
}

void EnemigoAlien::dibujar(sf::RenderWindow& window) const
{
    if (activo_)
        window.draw(sprite_);
}

bool EnemigoAlien::estaActivo() const
{
    return activo_;
}

void EnemigoAlien::desactivar()
{
    activo_ = false;
}

int EnemigoAlien::obtenerDanio() const
{
    return danio_;
}

bool EnemigoAlien::recibirDanio(int danio)
{
    vida_ -= danio;
    return vida_ <= 0;
}

bool EnemigoAlien::listoParaDisparar()
{
    if (!activo_ || relojDisparo_.getElapsedTime().asSeconds() < frecuenciaDisparo_)
        return false;

    relojDisparo_.restart();
    return true;
}

sf::Vector2f EnemigoAlien::obtenerOrigenDisparo() const
{
    const sf::FloatRect limites = sprite_.getGlobalBounds();
    return {
        limites.position.x + limites.size.x / 2.f,
        limites.position.y + limites.size.y
    };
}

sf::FloatRect EnemigoAlien::obtenerLimitesColision() const
{
    return sprite_.getGlobalBounds();
}

void EnemigoAlien::establecerPausa(bool pausado)
{
    if (pausado)
    {
        relojAnimacion_.stop();
        relojDisparo_.stop();
    }
    else
    {
        relojAnimacion_.start();
        relojDisparo_.start();
    }
}

void EnemigoAlien::actualizarAnimacion()
{
    if (relojAnimacion_.getElapsedTime().asSeconds() < 0.18f)
        return;

    frame_ = (frame_ + 1) % 2;
    sprite_.setTexture(frame_ == 0 ? texturaFrame1_ : texturaFrame2_, true);
    relojAnimacion_.restart();
}

bool Esbirro::cargarTextura()
{
    if (!textura_.loadFromFile("assets/esbirro.png"))
        return false;

    sprite_.setTexture(textura_, true);
    sprite_.setScale({escala_, escala_});
    return true;
}

void Esbirro::activar(float posicionX, int danio, int vida, float frecuenciaDisparo)
{
    y_ = -sprite_.getGlobalBounds().size.y;
    activo_ = true;
    danio_ = danio;
    vida_ = vida;
    frecuenciaDisparo_ = frecuenciaDisparo;
    relojDisparo_.restart();
    sprite_.setPosition({posicionX, y_});
}

void Esbirro::configurarVelocidad(float velocidadVertical)
{
    velocidadVertical_ = velocidadVertical;
}

void Esbirro::actualizar()
{
    if (!activo_)
        return;

    y_ += velocidadVertical_;
    sprite_.setPosition({sprite_.getPosition().x, y_});

    if (y_ > 1080.f)
        activo_ = false;
}

void Esbirro::dibujar(sf::RenderWindow& window) const
{
    if (activo_)
        window.draw(sprite_);
}

bool Esbirro::estaActivo() const
{
    return activo_;
}

void Esbirro::desactivar()
{
    activo_ = false;
}

int Esbirro::obtenerDanio() const
{
    return danio_;
}

bool Esbirro::recibirDanio(int danio)
{
    vida_ -= danio;
    return vida_ <= 0;
}

bool Esbirro::listoParaDisparar()
{
    if (!activo_ || relojDisparo_.getElapsedTime().asSeconds() < frecuenciaDisparo_)
        return false;

    relojDisparo_.restart();
    return true;
}

sf::Vector2f Esbirro::obtenerOrigenDisparo() const
{
    const sf::FloatRect limites = sprite_.getGlobalBounds();
    return {
        limites.position.x + limites.size.x / 2.f,
        limites.position.y + limites.size.y
    };
}

sf::FloatRect Esbirro::obtenerLimitesColision() const
{
    return sprite_.getGlobalBounds();
}

void Esbirro::establecerPausa(bool pausado)
{
    if (pausado)
        relojDisparo_.stop();
    else
        relojDisparo_.start();
}

bool MoluscoGiratorio::cargarTextura()
{
    if (!textura_.loadFromFile("assets/miniboss_molusco_1.png"))
        return false;

    sprite_.setTexture(textura_, true);
    sprite_.setScale({escala_, escala_});
    sprite_.setOrigin({
        textura_.getSize().x / 2.f,
        textura_.getSize().y / 2.f
    });
    return true;
}

void MoluscoGiratorio::activar(
    float posicionX,
    int danio,
    int vida,
    float frecuenciaDisparo)
{
    const float mitadAlto = sprite_.getGlobalBounds().size.y / 2.f;
    sprite_.setPosition({posicionX, -mitadAlto});
    sprite_.setRotation(sf::degrees(0.f));
    danio_ = danio;
    vida_ = vida;
    frecuenciaDisparo_ = frecuenciaDisparo;
    faseMovimiento_ = FaseMovimiento::DiagonalAbajo;
    activo_ = true;
    relojDisparo_.restart();
}

void MoluscoGiratorio::configurarMovimiento(
    float velocidadHorizontal,
    float velocidadVertical)
{
    velocidadHorizontal_ = velocidadHorizontal;
    velocidadVertical_ = velocidadVertical;
}

void MoluscoGiratorio::actualizar()
{
    if (!activo_)
        return;

    sprite_.rotate(sf::degrees(4.f));

    if (faseMovimiento_ == FaseMovimiento::DiagonalAbajo)
    {
        sprite_.move({velocidadHorizontal_, velocidadVertical_});
        if (sprite_.getPosition().y >= 1080.f / 6.f)
            faseMovimiento_ = FaseMovimiento::RectoAbajo;
    }
    else if (faseMovimiento_ == FaseMovimiento::RectoAbajo)
    {
        sprite_.move({0.f, velocidadVertical_});
        if (sprite_.getPosition().y >= 1080.f * 0.75f)
            faseMovimiento_ = FaseMovimiento::DiagonalArriba;
    }
    else
    {
        sprite_.move({velocidadHorizontal_, -velocidadVertical_});
    }

    const sf::FloatRect limites = sprite_.getGlobalBounds();
    if (limites.position.x > 1024.f
        || limites.position.y + limites.size.y < 0.f)
    {
        activo_ = false;
    }
}

void MoluscoGiratorio::dibujar(sf::RenderWindow& window) const
{
    if (activo_)
        window.draw(sprite_);
}

bool MoluscoGiratorio::estaActivo() const
{
    return activo_;
}

void MoluscoGiratorio::desactivar()
{
    activo_ = false;
}

int MoluscoGiratorio::obtenerDanio() const
{
    return danio_;
}

bool MoluscoGiratorio::recibirDanio(int danio)
{
    vida_ -= danio;
    return vida_ <= 0;
}

bool MoluscoGiratorio::listoParaDisparar()
{
    if (!activo_ || relojDisparo_.getElapsedTime().asSeconds() < frecuenciaDisparo_)
        return false;

    relojDisparo_.restart();
    return true;
}

sf::Vector2f MoluscoGiratorio::obtenerOrigenDisparo() const
{
    const sf::FloatRect limites = sprite_.getGlobalBounds();
    return {
        limites.position.x + limites.size.x / 2.f,
        limites.position.y + limites.size.y
    };
}

sf::FloatRect MoluscoGiratorio::obtenerLimitesColision() const
{
    return sprite_.getGlobalBounds();
}

void MoluscoGiratorio::establecerPausa(bool pausado)
{
    if (pausado)
        relojDisparo_.stop();
    else
        relojDisparo_.start();
}

bool MiniBossMolusco::cargarTexturas()
{
    if (!texturaFrame1_.loadFromFile("assets/miniboss_molusco_1.png"))
        return false;
    if (!texturaFrame2_.loadFromFile("assets/miniboss_molusco_2.png"))
        return false;
    if (!texturaFrame3_.loadFromFile("assets/miniboss_molusco_3.png"))
        return false;

    sprite_.setTexture(texturaFrame1_, true);
    sprite_.setScale({escala_, escala_});
    sprite_.setOrigin({
        texturaFrame1_.getSize().x / 2.f,
        texturaFrame1_.getSize().y / 2.f
    });
    return true;
}

void MiniBossMolusco::activar(
    float posicionX,
    int danio,
    int vida,
    float frecuenciaDisparo)
{
    const float mitadAlto = sprite_.getGlobalBounds().size.y / 2.f;
    sprite_.setPosition({posicionX, -mitadAlto});
    danio_ = danio;
    vida_ = vida;
    frecuenciaDisparo_ = frecuenciaDisparo;
    estado_ = EstadoMovimiento::Llegando;
    activo_ = true;
    relojAnimacion_.restart();
    relojDisparo_.restart();
}

void MiniBossMolusco::configurarMovimiento(
    float velocidadVertical,
    float alturaEspera,
    float duracionEspera,
    float velocidadHorizontal)
{
    velocidadVertical_ = velocidadVertical;
    alturaEspera_ = alturaEspera;
    duracionEspera_ = duracionEspera;
    velocidadHorizontal_ = velocidadHorizontal;
}

void MiniBossMolusco::actualizar()
{
    if (!activo_)
        return;

    actualizarAnimacion();

    if (estado_ == EstadoMovimiento::Llegando)
    {
        sprite_.move({0.f, velocidadVertical_});
        if (sprite_.getPosition().y >= alturaEspera_)
        {
            sprite_.setPosition({sprite_.getPosition().x, alturaEspera_});
            estado_ = EstadoMovimiento::Esperando;
            relojEspera_.restart();
        }
    }
    else if (estado_ == EstadoMovimiento::Esperando)
    {
        float x = sprite_.getPosition().x + velocidadHorizontal_;
        const float mitadAncho = sprite_.getGlobalBounds().size.x / 2.f;
        if (x < mitadAncho || x > 1024.f - mitadAncho)
        {
            velocidadHorizontal_ = -velocidadHorizontal_;
            x = sprite_.getPosition().x + velocidadHorizontal_;
        }
        sprite_.setPosition({x, sprite_.getPosition().y});

        if (relojEspera_.getElapsedTime().asSeconds() >= duracionEspera_)
            estado_ = EstadoMovimiento::Saliendo;
    }
    else
    {
        sprite_.move({0.f, velocidadVertical_});
        if (sprite_.getGlobalBounds().position.y > 1080.f)
            activo_ = false;
    }
}

void MiniBossMolusco::dibujar(sf::RenderWindow& window) const
{
    if (activo_)
        window.draw(sprite_);
}

bool MiniBossMolusco::estaActivo() const
{
    return activo_;
}

void MiniBossMolusco::desactivar()
{
    activo_ = false;
}

int MiniBossMolusco::obtenerDanio() const
{
    return danio_;
}

bool MiniBossMolusco::recibirDanio(int danio)
{
    vida_ -= danio;
    return vida_ <= 0;
}

bool MiniBossMolusco::listoParaDisparar()
{
    if (!activo_ || relojDisparo_.getElapsedTime().asSeconds() < frecuenciaDisparo_)
        return false;

    relojDisparo_.restart();
    return true;
}

sf::Vector2f MiniBossMolusco::obtenerOrigenDisparoIzquierdo() const
{
    const sf::FloatRect limites = sprite_.getGlobalBounds();
    return {
        limites.position.x + limites.size.x * 0.2f,
        limites.position.y + limites.size.y * 0.72f
    };
}

sf::Vector2f MiniBossMolusco::obtenerOrigenDisparoDerecho() const
{
    const sf::FloatRect limites = sprite_.getGlobalBounds();
    return {
        limites.position.x + limites.size.x * 0.8f,
        limites.position.y + limites.size.y * 0.72f
    };
}

sf::FloatRect MiniBossMolusco::obtenerLimitesColision() const
{
    return sprite_.getGlobalBounds();
}

void MiniBossMolusco::establecerPausa(bool pausado)
{
    if (pausado)
    {
        relojAnimacion_.stop();
        relojEspera_.stop();
        relojDisparo_.stop();
    }
    else
    {
        relojAnimacion_.start();
        relojEspera_.start();
        relojDisparo_.start();
    }
}

void MiniBossMolusco::actualizarAnimacion()
{
    if (relojAnimacion_.getElapsedTime().asSeconds() < 0.2f)
        return;

    frame_ = (frame_ + 1) % 3;
    if (frame_ == 0)
        sprite_.setTexture(texturaFrame1_, true);
    else if (frame_ == 1)
        sprite_.setTexture(texturaFrame2_, true);
    else
        sprite_.setTexture(texturaFrame3_, true);
    relojAnimacion_.restart();
}
