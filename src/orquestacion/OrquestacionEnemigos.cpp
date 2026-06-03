#include "OrquestacionEnemigos.hpp"

const std::vector<OleadaEnemigos> &obtenerOleadasEnemigos()
{
    // tiempo, tipo, cantidad, danio, vida, frecuencia disparo,
    // x inicial, separacion x, comportamiento
    static const std::vector<OleadaEnemigos> oleadas = {

        {4.0f, TipoEnemigo::MoluscoGiratorio, 1, 1, 2, 1.5f, 180.f, 0.f, ComportamientoEnemigo::MoluscoGiratorio},
        {4.3f, TipoEnemigo::MoluscoGiratorio, 1, 1, 2, 1.5f, 180.f, 0.f, ComportamientoEnemigo::MoluscoGiratorio},
        {4.6f, TipoEnemigo::MoluscoGiratorio, 1, 1, 2, 1.5f, 180.f, 0.f, ComportamientoEnemigo::MoluscoGiratorio},
        {4.9f, TipoEnemigo::MoluscoGiratorio, 1, 1, 2, 1.5f, 180.f, 0.f, ComportamientoEnemigo::MoluscoGiratorio},
        {5.2f, TipoEnemigo::MoluscoGiratorio, 1, 1, 2, 1.5f, 180.f, 0.f, ComportamientoEnemigo::MoluscoGiratorio},
        {5.5f, TipoEnemigo::MoluscoGiratorio, 1, 1, 2, 1.5f, 180.f, 0.f, ComportamientoEnemigo::MoluscoGiratorio},


        {11.0f, TipoEnemigo::MoluscoGiratorio, 1, 1, 2, 1.5f, 800.f, 0.f, ComportamientoEnemigo::MoluscoGiratorioIzquierda},
        {11.3f, TipoEnemigo::MoluscoGiratorio, 1, 1, 2, 1.5f, 800.f, 0.f, ComportamientoEnemigo::MoluscoGiratorioIzquierda},
        {11.6f, TipoEnemigo::MoluscoGiratorio, 1, 1, 2, 1.5f, 800.f, 0.f, ComportamientoEnemigo::MoluscoGiratorioIzquierda},
        {11.9f, TipoEnemigo::MoluscoGiratorio, 1, 1, 2, 1.5f, 800.f, 0.f, ComportamientoEnemigo::MoluscoGiratorioIzquierda},
        {12.2f, TipoEnemigo::MoluscoGiratorio, 1, 1, 2, 1.5f, 800.f, 0.f, ComportamientoEnemigo::MoluscoGiratorioIzquierda},
        {12.5f, TipoEnemigo::MoluscoGiratorio, 1, 1, 2, 1.5f, 800.f, 0.f, ComportamientoEnemigo::MoluscoGiratorioIzquierda},
        // tiempo, tipo,        cantidad, danio, vida, frecuencia disparo, x inicial, separacion x,
        {17.0f, TipoEnemigo::Esbirro, 1, 1, 1, 0.8f, 500.f, 0.f, ComportamientoEnemigo::EsbirroNormal},  //      *     *
        {13.3f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 400.f, 200.f, ComportamientoEnemigo::EsbirroNormal}, //     *       *
        // {2.f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 300.f, 400.f, ComportamientoEnemigo::EsbirroNormal},
        // {2.7f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 300.f, 400.f, ComportamientoEnemigo::EsbirroNormal},
        // {3.4f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 300.f, 400.f, ComportamientoEnemigo::EsbirroNormal},

        {23.0f, TipoEnemigo::MoluscoGiratorio, 1, 1, 2, 1.5f, 200.f, 0.f, ComportamientoEnemigo::MoluscoGiratorio},
        {23.0f, TipoEnemigo::MoluscoGiratorio, 1, 1, 2, 1.5f, 800.f, 0.f, ComportamientoEnemigo::MoluscoGiratorioIzquierda},

        {25.0f, TipoEnemigo::MoluscoGiratorio, 1, 1, 2, 1.5f, 200.f, 0.f, ComportamientoEnemigo::MoluscoGiratorio},
        {25.0f, TipoEnemigo::MoluscoGiratorio, 1, 1, 2, 1.5f, 800.f, 0.f, ComportamientoEnemigo::MoluscoGiratorioIzquierda},


        {27.0f, TipoEnemigo::MoluscoGiratorio, 1, 1, 2, 1.5f, 200.f, 0.f, ComportamientoEnemigo::MoluscoGiratorio},
        {27.0f, TipoEnemigo::MoluscoGiratorio, 1, 1, 2, 1.5f, 800.f, 0.f, ComportamientoEnemigo::MoluscoGiratorioIzquierda},
        // tiempo,     tipo,       cantidad,  danio, vida, frecuencia disparo, x inicial, separacion x, {amplitud  ,descenso, oscilacion   }
        // {17.f, TipoEnemigo::Alien, 1, 2, 3, 1.8f, 500.f, 0.f, ComportamientoEnemigo::AlienNormal},
        // {9.f, TipoEnemigo::Alien, 1, 2, 3, 1.8f, 500.f, 0.f, ComportamientoEnemigo::AlienNormal},
        // {22.f, TipoEnemigo::Alien, 2, 2, 3, 1.8f, 300.f, 300.f, ComportamientoEnemigo::AlienNormal},



        {33.0f, TipoEnemigo::Esbirro, 1, 1, 1, 0.8f, 500.f, 0.f, ComportamientoEnemigo::EsbirroNormal},  //      *     *
        {33.3f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 400.f, 200.f, ComportamientoEnemigo::EsbirroNormal}, //     *       *
        {33.6f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 300.f, 400.f, ComportamientoEnemigo::EsbirroNormal}, //    *         *
        {33.9f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 200.f, 600.f, ComportamientoEnemigo::EsbirroNormal}, //   *           *
        // {31.2f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 100.f, 800.f, ComportamientoEnemigo::EsbirroNormal}, //  *             *
        {45.f, TipoEnemigo::Alien, 1, 2, 6, 1.8f, 500.f, 0.f, ComportamientoEnemigo::AlienNormal},
        {54.f, TipoEnemigo::Alien, 1, 2, 6, 1.8f, 900.f, 0.f, ComportamientoEnemigo::AlienNormal},
        {56.f, TipoEnemigo::Alien, 1, 2, 6, 1.8f, 400.f, 0.f, ComportamientoEnemigo::AlienNormal},
        {58.f, TipoEnemigo::Alien, 1, 2, 6, 1.8f, 900.f, 0.f, ComportamientoEnemigo::AlienNormal},
        {60.f, TipoEnemigo::Alien, 1, 2, 6, 1.8f, 400.f, 0.f, ComportamientoEnemigo::AlienNormal},
        {64.f, TipoEnemigo::TortugaGiratoria, 1, 2, 4, 1.25f, 512.f, 0.f, ComportamientoEnemigo::TortugaGiratoria},






        // {32.0f, TipoEnemigo::CangrejoMetalico, 2, 1, 2, 1.7f, 260.f, 500.f, ComportamientoEnemigo::CangrejoMetalico},
        // {35.f, TipoEnemigo::Alien, 1, 2, 3, 1.8f, 500.f, 0.f, ComportamientoEnemigo::AlienNormal},
        // {36.f, TipoEnemigo::Esbirro, 2, 1, 1, 1.1f, 300.f, 400.f, ComportamientoEnemigo::EsbirroNormal},
        // {38.0f, TipoEnemigo::CangrejoMetalico, 1, 1, 2, 1.6f, 512.f, 0.f, ComportamientoEnemigo::CangrejoMetalico},
        // {40.f, TipoEnemigo::Alien, 3, 2, 3, 1.8f, 300.f, 300.f, ComportamientoEnemigo::AlienLento},


        // {50.f, TipoEnemigo::MiniBossMolusco, 1, 3, 60, 1.8f, 512.f, 0.f, ComportamientoEnemigo::MiniBossMolusco},
        // {55.2f, TipoEnemigo::Esbirro, 2, 1, 1, 0.4f, 100.f, 800.f, ComportamientoEnemigo::EsbirroNormal},
        // {57.0f, TipoEnemigo::Esbirro, 2, 1, 1, 0.4f, 100.f, 800.f, ComportamientoEnemigo::EsbirroNormal},


        // {69.f, TipoEnemigo::Esbirro, 2, 1, 1, 1.8f, 100.f, 800.f, ComportamientoEnemigo::EsbirroNormal},
        
        
     
        // {71.0f, TipoEnemigo::PescadoGigante, 1, 2, 20, 2.0f, 512.f, 0.f, ComportamientoEnemigo::PescadoGigante},
        // {75.0f, TipoEnemigo::PescadoGigante, 1, 2, 20, 2.0f, 200.f, 0.f, ComportamientoEnemigo::PescadoGigante},
        // {79.0f, TipoEnemigo::PescadoGigante, 1, 2, 20, 2.0f, 700.f, 0.f, ComportamientoEnemigo::PescadoGigante},

        // {27.f, TipoEnemigo::Nave, 1, 3, 2, 1.f, 120.f, 230.f, ComportamientoEnemigo::NaveDiagonalIzquierda},
    };

    return oleadas;
}
