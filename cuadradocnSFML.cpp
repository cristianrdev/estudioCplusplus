#include <SFML/Graphics.hpp>
#include <optional>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Cuadrado SFML");
    window.setFramerateLimit(60);

    sf::RectangleShape cuadrado({200.f, 200.f});
    cuadrado.setPosition({300.f, 200.f});
    cuadrado.setFillColor(sf::Color::Green);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        window.draw(cuadrado);
        window.display();
    }

    return 0;
}