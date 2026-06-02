#include "OrquestacionEnemigos.hpp"

const std::vector<OleadaEnemigos> &obtenerOleadasEnemigos()
{
    // tiempo, tipo, cantidad, danio, vida, frecuencia disparo,
    // x inicial, separacion x, comportamiento
    static const std::vector<OleadaEnemigos> oleadas = {
        // tiempo, tipo,        cantidad, danio, vida, frecuencia disparo, x inicial, separacion x,

        {2.f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 300.f, 400.f, ComportamientoEnemigo::EsbirroNormal},
        {2.7f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 300.f, 400.f, ComportamientoEnemigo::EsbirroNormal},
        {3.4f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 300.f, 400.f, ComportamientoEnemigo::EsbirroNormal},


        // tiempo,     tipo,       cantidad,  danio, vida, frecuencia disparo, x inicial, separacion x, {amplitud  ,descenso, oscilacion   }
        {7.f, TipoEnemigo::Alien, 1, 2, 3, 1.8f, 500.f, 0.f, ComportamientoEnemigo::AlienNormal},
        // {9.f, TipoEnemigo::Alien, 1, 2, 3, 1.8f, 500.f, 0.f, ComportamientoEnemigo::AlienNormal},
        {12.f, TipoEnemigo::Alien, 2, 2, 3, 1.8f, 300.f, 300.f, ComportamientoEnemigo::AlienNormal},



        {18.0f, TipoEnemigo::Esbirro, 1, 1, 1, 0.8f, 500.f, 0.f, ComportamientoEnemigo::EsbirroNormal},  //      *     *
        {18.3f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 400.f, 200.f, ComportamientoEnemigo::EsbirroNormal}, //     *       *
        {18.6f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 300.f, 400.f, ComportamientoEnemigo::EsbirroNormal}, //    *         *
        {18.9f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 200.f, 600.f, ComportamientoEnemigo::EsbirroNormal}, //   *           *
        {19.2f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 100.f, 800.f, ComportamientoEnemigo::EsbirroNormal}, //  *             *

        {25.f, TipoEnemigo::Alien, 1, 2, 3, 1.8f, 500.f, 0.f, ComportamientoEnemigo::AlienNormal},
        {26.f, TipoEnemigo::Esbirro, 2, 1, 1, 1.1f, 300.f, 400.f, ComportamientoEnemigo::EsbirroNormal},
        {30.f, TipoEnemigo::Alien, 3, 2, 3, 1.8f, 300.f, 300.f, ComportamientoEnemigo::AlienLento},


        {40.f, TipoEnemigo::MiniBossMolusco, 1, 3, 60, 1.8f, 512.f, 0.f, ComportamientoEnemigo::MiniBossMolusco},
        {45.2f, TipoEnemigo::Esbirro, 2, 1, 1, 0.4f, 100.f, 800.f, ComportamientoEnemigo::EsbirroNormal},
        {47.0f, TipoEnemigo::Esbirro, 2, 1, 1, 0.4f, 100.f, 800.f, ComportamientoEnemigo::EsbirroNormal},


        {59.f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 100.f, 800.f, ComportamientoEnemigo::EsbirroNormal},
        
        
        {61.f, TipoEnemigo::Nave, 4, 1, 3, 1.4f, 180.f, 220.f, ComportamientoEnemigo::NaveDiagonalDerecha},

        // {13.f, TipoEnemigo::Alien, 1, 2, 3, 1.8f, 450.f, 0.f, ComportamientoEnemigo::AlienLento},
        // {16.f, TipoEnemigo::Esbirro, 4, 1, 1, 1.1f, 140.f, 230.f, ComportamientoEnemigo::EsbirroRapido},
        // {21.f, TipoEnemigo::Alien, 3, 1, 3, 2.2f, 170.f, 280.f, ComportamientoEnemigo::AlienRapido},
        // {27.f, TipoEnemigo::Nave, 1, 3, 2, 1.f, 120.f, 230.f, ComportamientoEnemigo::NaveDiagonalIzquierda},
    };

    return oleadas;
}
