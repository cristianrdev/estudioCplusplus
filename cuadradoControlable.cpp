#include <SFML/Graphics.hpp>
#include <optional>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Mover Cuadrado");
    window.setFramerateLimit(60);

    // Crear cuadrado
    sf::RectangleShape cuadrado({100.f, 100.f});
    cuadrado.setFillColor(sf::Color::Blue);
    cuadrado.setPosition({350.f, 250.f});

    float velocidad = 5.f;

    while (window.isOpen()) {

        // Eventos
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // Movimiento con teclado
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            cuadrado.move({0.f, -velocidad});
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            cuadrado.move({0.f, velocidad});
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            cuadrado.move({-velocidad, 0.f});
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            cuadrado.move({velocidad, 0.f});
        }

        // Dibujar
        window.clear();
        window.draw(cuadrado);
        window.display();
    }

    return 0;
}