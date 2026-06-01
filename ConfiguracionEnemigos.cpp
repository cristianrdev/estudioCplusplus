#include "ConfiguracionEnemigos.hpp"

const std::vector<OleadaEnemigos>& obtenerOleadasEnemigos()
{
    // tiempo, tipo, cantidad, danio, frecuencia disparo, x inicial, separacion x,
    // {amplitud, descenso, oscilacion}
    static const std::vector<OleadaEnemigos> oleadas = {
        {1.f, TipoEnemigo::Nave, 2, 1, 1.4f, 180.f, 420.f, {100.f, 2.5f, 0.035f}},
        {4.f, TipoEnemigo::Alien, 1, 2, 1.8f, 450.f, 0.f, {280.f, 1.8f, 0.025f}},
        {6.f, TipoEnemigo::Esbirro, 4, 1, 1.1f, 140.f, 230.f, {0.f, 5.2f, 0.f}},
        {8.f, TipoEnemigo::Alien, 3, 1, 2.2f, 170.f, 280.f, {90.f, 2.2f, 0.04f}},
        {13.f, TipoEnemigo::Nave, 4, 3, 1.f, 120.f, 230.f, {70.f, 3.f, 0.05f}},
    };

    return oleadas;
}
