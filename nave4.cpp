#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>

struct Proyectil
{
    float x;
    float y;
    bool activo;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode({1024, 1080}), "Nave Shooter");
    window.setFramerateLimit(60);

    // -------------------------
    // CONFIG
    // -------------------------
    float escalaNave = 0.11f;
    float escalaFuego = 0.08f;
    float largoFuego = escalaFuego;
    float x = 340.f;
    float y = 500.f;
    float velocidad = 8.f;

    float cadenciaDisparo = 0.12f;
    float velocidadLaser = 8.f;

    // Scroll fondo
    float velocidadFondo = 2.0f;

    sf::Clock relojDisparo;
    sf::Clock relojFuego;

    // -------------------------
    // TEXTURA FONDO
    // -------------------------
    sf::Texture texturaFondo;
    if (!texturaFondo.loadFromFile("fondo.png"))
        return -1;

    sf::Sprite fondo1(texturaFondo);
    sf::Sprite fondo2(texturaFondo);

    // Escalar fondo 1024x1536 para cubrir ancho 1920
    fondo1.setScale({1.f, 1.f});
    fondo2.setScale({1.f, 1.f});

    float altoFondoEscalado = 1536.f;

    fondo1.setPosition({0.f, 0.f});
    fondo2.setPosition({0.f, -altoFondoEscalado});

    // -------------------------
    // TEXTURAS NAVE
    // -------------------------
    sf::Texture texturaCentro;
    sf::Texture texturaIzquierda;
    sf::Texture texturaDerecha;

    if (!texturaCentro.loadFromFile("centro.png"))
        return -1;
    if (!texturaIzquierda.loadFromFile("izquierda.png"))
        return -1;
    if (!texturaDerecha.loadFromFile("derecha.png"))
        return -1;

    sf::Sprite nave(texturaCentro);
    nave.setScale({escalaNave, escalaNave});

    // -------------------------
    // TEXTURAS FUEGO
    // -------------------------
    sf::Texture fuegoTex1;
    sf::Texture fuegoTex2;
    sf::Texture fuegoTex3;

    if (!fuegoTex1.loadFromFile("fuego1.png"))
        return -1;
    if (!fuegoTex2.loadFromFile("fuego2.png"))
        return -1;
    if (!fuegoTex3.loadFromFile("fuego3.png"))
        return -1;

    sf::Sprite fuegoIzq(fuegoTex1);
    sf::Sprite fuegoDer(fuegoTex1);

    fuegoIzq.setScale({escalaFuego, largoFuego});
    fuegoDer.setScale({escalaFuego, largoFuego});

    int frameFuego = 0;

    std::vector<Proyectil> proyectiles;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // -------------------------
        // SCROLL FONDO
        // -------------------------
        fondo1.move({0.f, velocidadFondo});
        fondo2.move({0.f, velocidadFondo});

        if (fondo1.getPosition().y >= altoFondoEscalado)
        {
            fondo1.setPosition({0.f, fondo2.getPosition().y - altoFondoEscalado});
        }

        if (fondo2.getPosition().y >= altoFondoEscalado)
        {
            fondo2.setPosition({0.f, fondo1.getPosition().y - altoFondoEscalado});
        }

        // -------------------------
        // MOVIMIENTO + SPRITE NAVE
        // -------------------------
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            y -= velocidad;
            largoFuego = 0.1f;
            //velocidadFondo = 4.0f; // acelera scroll al subir
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            y += velocidad;
            largoFuego = 0.05f;
            //velocidadFondo = 1.0f; // scroll mas lento al bajar
        }
        else
        {
            largoFuego = escalaFuego;
            //velocidadFondo = 2.0f; // normal
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            x -= velocidad;
            nave.setTexture(texturaIzquierda, true);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            x += velocidad;
            nave.setTexture(texturaDerecha, true);
        }
        else
        {
            nave.setTexture(texturaCentro, true);
        }

        nave.setScale({escalaNave, escalaNave});
        nave.setPosition({x, y});

        // -------------------------
        // ANIMACION FUEGO
        // -------------------------
        if (relojFuego.getElapsedTime().asSeconds() >= 0.07f)
        {
            frameFuego++;

            if (frameFuego > 2)
                frameFuego = 0;

            if (frameFuego == 0)
            {
                fuegoIzq.setTexture(fuegoTex1, true);
                fuegoDer.setTexture(fuegoTex1, true);
            }

            if (frameFuego == 1)
            {
                fuegoIzq.setTexture(fuegoTex2, true);
                fuegoDer.setTexture(fuegoTex2, true);
            }

            if (frameFuego == 2)
            {
                fuegoIzq.setTexture(fuegoTex3, true);
                fuegoDer.setTexture(fuegoTex3, true);
            }

            relojFuego.restart();
        }

        fuegoIzq.setScale({escalaFuego, largoFuego});
        fuegoDer.setScale({escalaFuego, largoFuego});

        // posicion fuego detras de nave
        float anchoNave = nave.getGlobalBounds().size.x;
        float altoNave = nave.getGlobalBounds().size.y;

        fuegoIzq.setPosition({
            x + 6.f,
            y + altoNave - 14.f
        });

        fuegoDer.setPosition({
            x + anchoNave - 19.f,
            y + altoNave - 14.f
        });

        // -------------------------
        // DISPARO
        // -------------------------
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        {
            if (relojDisparo.getElapsedTime().asSeconds() >= cadenciaDisparo)
            {
                Proyectil nuevo;

                nuevo.x = x + (anchoNave / 2.f) - 4.f;
                nuevo.y = y - 20.f;
                nuevo.activo = true;

                proyectiles.push_back(nuevo);

                relojDisparo.restart();
            }
        }

        // -------------------------
        // MOVER PROYECTILES
        // -------------------------
        for (auto &p : proyectiles)
        {
            if (p.activo)
            {
                p.y -= velocidadLaser;
            }

            if (p.y < -40.f)
            {
                p.activo = false;
            }
        }

        // -------------------------
        // DIBUJAR
        // -------------------------
        window.clear();

        // Fondo primero
        window.draw(fondo1);
        window.draw(fondo2);

        // fuego detras
        window.draw(fuegoIzq);
        window.draw(fuegoDer);

        // nave
        window.draw(nave);

        // lasers
        for (const auto &p : proyectiles)
        {
            if (!p.activo)
                continue;

            sf::RectangleShape laserRojo({8.f, 30.f});
            laserRojo.setPosition({p.x, p.y});
            laserRojo.setFillColor(sf::Color::Red);

            sf::RectangleShape laserBlanco({4.f, 30.f});
            laserBlanco.setPosition({p.x + 2.f, p.y});
            laserBlanco.setFillColor(sf::Color::White);

            sf::RectangleShape punta({8.f, 4.f});
            punta.setPosition({p.x, p.y - 4.f});
            punta.setFillColor(sf::Color::Yellow);

            window.draw(laserRojo);
            window.draw(laserBlanco);
            window.draw(punta);
        }

        window.display();
    }

    return 0;
}