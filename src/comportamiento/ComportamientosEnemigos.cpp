#include "ComportamientosEnemigos.hpp"

#include <stdexcept>

const ConfiguracionComportamientoEnemigo& obtenerComportamientoEnemigo(
    ComportamientoEnemigo comportamiento)
{
    // nombre, {horizontal o amplitud, descenso, oscilacion},
    // {altura espera miniboss, segundos espera, velocidad horizontal miniboss}
    static const ConfiguracionComportamientoEnemigo comportamientos[] = {
        {"EsbirroNormal", {0.f, 2.5f, 0.f}, {}},
        {"EsbirroRapido", {0.f, 5.2f, 0.f}, {}},
        {"AlienNormal", {100.f, 2.f, 0.04f}, {}},
        {"AlienLento", {100.f, 1.8f, 0.025f}, {}},
        {"AlienRapido", {140.f, 2.6f, 0.055f}, {}},
        {"NaveDiagonalDerecha", {2.2f, 3.5f, 0.f}, {}},
        {"NaveDiagonalIzquierda", {-2.8f, 3.f, 0.f}, {}},
        {"MoluscoGiratorio", {2.24f, 2.8f, 0.f}, {}},
        {"MoluscoGiratorioIzquierda", {-2.24f, 2.8f, 0.f}, {}},
        {"MiniBossMolusco", {0.f, 2.4f, 0.f}, {200.f, 10.f, 3.2f}},
        {"PescadoGigante", {0.f, 1.2f, 0.f}, {}},
        {"CangrejoMetalico", {2.4f, 1.5f, 0.f}, {}},
        {"TortugaGiratoria", {0.f, 1.35f, 0.f}, {}},
    };

    const int indice = static_cast<int>(comportamiento);
    const int cantidad = sizeof(comportamientos) / sizeof(comportamientos[0]);
    if (indice < 0 || indice >= cantidad)
        throw std::invalid_argument("Comportamiento enemigo sin configurar");

    return comportamientos[indice];
}
