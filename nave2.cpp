#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>

struct BloqueNave {
    int col;
    int fil;
    sf::Color color;
};

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Nave con Colores");
    window.setFramerateLimit(60);

    float tam = 5.f; // TAMAÑO DE LA NAVE
    float x = 340.f; // POSICION INICIAL
    float y = 180.f; //POSICION INICIAL
    float velocidad = 5.f;

    // Definición de la nave: posición + color por cuadrito
    std::vector<BloqueNave> nave = {
        {2, 0, sf::Color::Red},

        {1, 1, sf::Color::Yellow},
        {2, 1, sf::Color::White},
        {3, 1, sf::Color::Yellow},

        {0, 2, sf::Color::Green},
        {1, 2, sf::Color::Blue},
        {2, 2, sf::Color::Cyan},
        {3, 2, sf::Color::Blue},
        {4, 2, sf::Color::Green},

        {1, 3, sf::Color::Magenta},
        {2, 3, sf::Color::White},
        {3, 3, sf::Color::Magenta},

        {1, 4, sf::Color(255, 128, 0)},   // naranjo
        {3, 4, sf::Color(255, 128, 0)}
    };

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

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

        window.clear();

        // Dibujar cada cuadrito según su posición y color
        for (const auto& bloqueInfo : nave) {
            sf::RectangleShape bloque({tam, tam});
            bloque.setPosition({
                x + bloqueInfo.col * tam,
                y + bloqueInfo.fil * tam
            });
            bloque.setFillColor(bloqueInfo.color);
            bloque.setOutlineThickness(1.f);
            bloque.setOutlineColor(sf::Color::Black);

            window.draw(bloque);
        }

        window.display();
    }

    return 0;
}