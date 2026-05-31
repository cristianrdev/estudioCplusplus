#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Nave Shooter");
    window.setFramerateLimit(60);

    float tam = 5.f;       // tamaño de cada cuadrito
    float x = 340.f;        // posición base de la nave
    float y = 180.f;
    float velocidad = 5.f;

    // Cada par {columna, fila} representa un cuadrado de la nave
    std::vector<sf::Vector2f> formaNave = {
        {2, 0},

        {1, 1}, {2, 1}, {3, 1},

        {0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2},

        {1, 3}, {2, 3}, {3, 3},

        {1, 4}, {3, 4}
    };

    std::vector<sf::RectangleShape> bloques;

    for (const auto& pos : formaNave) {
        sf::RectangleShape bloque({tam, tam});
        bloque.setFillColor(sf::Color::Cyan);
        bloque.setOutlineThickness(1.f);
        bloque.setOutlineColor(sf::Color::Black);
        bloques.push_back(bloque);
    }

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

        // Actualizar posición de cada bloque de la nave
        for (size_t i = 0; i < bloques.size(); i++) {
            bloques[i].setPosition({
                x + formaNave[i].x * tam,
                y + formaNave[i].y * tam
            });
        }

        window.clear();

        for (const auto& bloque : bloques) {
            window.draw(bloque);
        }

        window.display();
    }

    return 0;
}