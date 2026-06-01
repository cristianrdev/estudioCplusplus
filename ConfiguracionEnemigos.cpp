#include "ConfiguracionEnemigos.hpp"

const std::vector<OleadaEnemigos>& obtenerOleadasEnemigos()
{
    // tiempo, tipo, cantidad, danio, vida, frecuencia disparo, x inicial,
    // separacion x, {amplitud o velocidad diagonal x, descenso, oscilacion}
    static const std::vector<OleadaEnemigos> oleadas = {
        {6.f, TipoEnemigo::Esbirro, 2, 1, 1, 1.1f, 140.f, 230.f, {0.f, 5.2f, 0.f}},
        {9.f, TipoEnemigo::Nave, 2, 1, 2, 1.4f, 180.f, 420.f, {2.2f, 2.5f, 0.f}},
        {13.f, TipoEnemigo::Alien, 1, 2, 3, 1.8f, 450.f, 0.f, {280.f, 1.8f, 0.025f}},
        {16.f, TipoEnemigo::Esbirro, 4, 1, 1, 1.1f, 140.f, 230.f, {0.f, 5.2f, 0.f}},
        {21.f, TipoEnemigo::Alien, 3, 1, 3, 2.2f, 170.f, 280.f, {90.f, 2.2f, 0.04f}},
        {27.f, TipoEnemigo::Nave, 4, 3, 2, 1.f, 120.f, 230.f, {-2.8f, 3.f, 0.f}},
    };

    return oleadas;
}
