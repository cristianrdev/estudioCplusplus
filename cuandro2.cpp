#include <SFML/Graphics.hpp>
#include <optional>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "4 Cuadros");
    window.setFramerateLimit(60);

    float tam = 10.f;
    float x = 30.f;
    float y = 20.f;
    float velocidad = 5.f;

    sf::RectangleShape verde({tam, tam});
    verde.setFillColor(sf::Color::Green);

    sf::RectangleShape rojo({tam, tam});
    rojo.setFillColor(sf::Color::Red);

    sf::RectangleShape amarillo({tam, tam});
    amarillo.setFillColor(sf::Color::Yellow);

    sf::RectangleShape azul({tam, tam});
    azul.setFillColor(sf::Color::Blue);

    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // Movimiento con flechas
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            y -= velocidad;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            y += velocidad;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            x -= velocidad;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            x += velocidad;
        }

        // Posiciones
        verde.setPosition({x, y});
        rojo.setPosition({x + tam, y});
        amarillo.setPosition({x, y + tam});
        azul.setPosition({x + tam, y + tam});

        window.clear();

        window.draw(verde);
        window.draw(rojo);
        window.draw(amarillo);
        window.draw(azul);

        window.display();
    }

    return 0;
}