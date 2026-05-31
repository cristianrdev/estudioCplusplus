#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>

struct BloqueNave {
    int col;
    int fil;
    sf::Color color;
};

struct Bala {
    float x;
    float y;
    bool activa;
};

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Nave Shooter");
    window.setFramerateLimit(60);

    float tam = 5.f;
    float x = 340.f;
    float y = 500.f;
    float velocidad = 3.f;

    // 🔥 Cadencia de disparo (segundos)
    float cadenciaDisparo = 0.15f;

    // velocidad bala
    float velocidadBala = 7.f;

    sf::Clock relojDisparo;

    std::vector<BloqueNave> nave = {
        {2,0,sf::Color::Red},

        {1,1,sf::Color::Yellow},
        {2,1,sf::Color::White},
        {3,1,sf::Color::Yellow},

        {0,2,sf::Color::Green},
        {1,2,sf::Color::Blue},
        {2,2,sf::Color::Cyan},
        {3,2,sf::Color::Blue},
        {4,2,sf::Color::Green},

        {1,3,sf::Color::Magenta},
        {2,3,sf::Color::White},
        {3,3,sf::Color::Magenta},

        {1,4,sf::Color(255,128,0)},
        {3,4,sf::Color(255,128,0)}
    };

    std::vector<Bala> balas;

    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // Movimiento nave
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) y -= velocidad;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) y += velocidad;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) x -= velocidad;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) x += velocidad;

        // 🔥 Disparo automático manteniendo espacio
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {

            if (relojDisparo.getElapsedTime().asSeconds() >= cadenciaDisparo) {

                Bala nueva;
                nueva.x = x + (2.f * tam);
                nueva.y = y - tam;
                nueva.activa = true;

                balas.push_back(nueva);

                relojDisparo.restart();
            }
        }

        // mover balas
        for (auto& bala : balas) {
            if (bala.activa) {
                bala.y -= velocidadBala;
            }

            if (bala.y < -50.f) {
                bala.activa = false;
            }
        }

        window.clear();

        // Dibujar nave
        for (const auto& bloqueInfo : nave) {
            sf::RectangleShape bloque({tam, tam});

            bloque.setPosition({
                x + bloqueInfo.col * tam,
                y + bloqueInfo.fil * tam
            });

            bloque.setFillColor(bloqueInfo.color);

            window.draw(bloque);
        }

        // Dibujar balas (7 cuadritos)
        for (const auto& bala : balas) {
            if (!bala.activa) continue;

            for (int i = 0; i < 7; i++) {
                sf::RectangleShape pixel({tam, tam});

                pixel.setPosition({
                    bala.x,
                    bala.y + (i * tam)
                });

                pixel.setFillColor(sf::Color::White);

                window.draw(pixel);
            }
        }

        window.display();
    }

    return 0;
}