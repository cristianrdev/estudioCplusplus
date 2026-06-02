#include "OrquestacionEnemigos.hpp"

const std::vector<OleadaEnemigos> &obtenerOleadasEnemigos()
{
    // tiempo, tipo, cantidad, danio, vida, frecuencia disparo,
    // x inicial, separacion x, comportamiento
    static const std::vector<OleadaEnemigos> oleadas = {

        {4.0f, TipoEnemigo::MoluscoGiratorio, 1, 1, 1, 1.5f, 180.f, 0.f, ComportamientoEnemigo::MoluscoGiratorio},
        {4.3f, TipoEnemigo::MoluscoGiratorio, 1, 1, 1, 1.5f, 180.f, 0.f, ComportamientoEnemigo::MoluscoGiratorio},
        {4.6f, TipoEnemigo::MoluscoGiratorio, 1, 1, 1, 1.5f, 180.f, 0.f, ComportamientoEnemigo::MoluscoGiratorio},
        {4.9f, TipoEnemigo::MoluscoGiratorio, 1, 1, 1, 1.5f, 180.f, 0.f, ComportamientoEnemigo::MoluscoGiratorio},
        {5.2f, TipoEnemigo::MoluscoGiratorio, 1, 1, 1, 1.5f, 180.f, 0.f, ComportamientoEnemigo::MoluscoGiratorio},
        {5.5f, TipoEnemigo::MoluscoGiratorio, 1, 1, 1, 1.5f, 180.f, 0.f, ComportamientoEnemigo::MoluscoGiratorio},


        {11.0f, TipoEnemigo::MoluscoGiratorio, 1, 1, 1, 1.5f, 350.f, 0.f, ComportamientoEnemigo::MoluscoGiratorio},
        {11.3f, TipoEnemigo::MoluscoGiratorio, 1, 1, 1, 1.5f, 350.f, 0.f, ComportamientoEnemigo::MoluscoGiratorio},
        {11.6f, TipoEnemigo::MoluscoGiratorio, 1, 1, 1, 1.5f, 350.f, 0.f, ComportamientoEnemigo::MoluscoGiratorio},
        {11.9f, TipoEnemigo::MoluscoGiratorio, 1, 1, 1, 1.5f, 350.f, 0.f, ComportamientoEnemigo::MoluscoGiratorio},
        {12.2f, TipoEnemigo::MoluscoGiratorio, 1, 1, 1, 1.5f, 350.f, 0.f, ComportamientoEnemigo::MoluscoGiratorio},
        {12.5f, TipoEnemigo::MoluscoGiratorio, 1, 1, 1, 1.5f, 350.f, 0.f, ComportamientoEnemigo::MoluscoGiratorio},
        // tiempo, tipo,        cantidad, danio, vida, frecuencia disparo, x inicial, separacion x,

        // {2.f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 300.f, 400.f, ComportamientoEnemigo::EsbirroNormal},
        // {2.7f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 300.f, 400.f, ComportamientoEnemigo::EsbirroNormal},
        // {3.4f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 300.f, 400.f, ComportamientoEnemigo::EsbirroNormal},


        // tiempo,     tipo,       cantidad,  danio, vida, frecuencia disparo, x inicial, separacion x, {amplitud  ,descenso, oscilacion   }
        {17.f, TipoEnemigo::Alien, 1, 2, 3, 1.8f, 500.f, 0.f, ComportamientoEnemigo::AlienNormal},
        // {9.f, TipoEnemigo::Alien, 1, 2, 3, 1.8f, 500.f, 0.f, ComportamientoEnemigo::AlienNormal},
        {22.f, TipoEnemigo::Alien, 2, 2, 3, 1.8f, 300.f, 300.f, ComportamientoEnemigo::AlienNormal},



        {28.0f, TipoEnemigo::Esbirro, 1, 1, 1, 0.8f, 500.f, 0.f, ComportamientoEnemigo::EsbirroNormal},  //      *     *
        {28.3f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 400.f, 200.f, ComportamientoEnemigo::EsbirroNormal}, //     *       *
        {28.6f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 300.f, 400.f, ComportamientoEnemigo::EsbirroNormal}, //    *         *
        {28.9f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 200.f, 600.f, ComportamientoEnemigo::EsbirroNormal}, //   *           *
        {29.2f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 100.f, 800.f, ComportamientoEnemigo::EsbirroNormal}, //  *             *

        {35.f, TipoEnemigo::Alien, 1, 2, 3, 1.8f, 500.f, 0.f, ComportamientoEnemigo::AlienNormal},
        {36.f, TipoEnemigo::Esbirro, 2, 1, 1, 1.1f, 300.f, 400.f, ComportamientoEnemigo::EsbirroNormal},
        {40.f, TipoEnemigo::Alien, 3, 2, 3, 1.8f, 300.f, 300.f, ComportamientoEnemigo::AlienLento},


        {50.f, TipoEnemigo::MiniBossMolusco, 1, 3, 60, 1.8f, 512.f, 0.f, ComportamientoEnemigo::MiniBossMolusco},
        {55.2f, TipoEnemigo::Esbirro, 2, 1, 1, 0.4f, 100.f, 800.f, ComportamientoEnemigo::EsbirroNormal},
        {57.0f, TipoEnemigo::Esbirro, 2, 1, 1, 0.4f, 100.f, 800.f, ComportamientoEnemigo::EsbirroNormal},


        {69.f, TipoEnemigo::Esbirro, 2, 1, 1, 0.8f, 100.f, 800.f, ComportamientoEnemigo::EsbirroNormal},
        
        
        {71.f, TipoEnemigo::Nave, 4, 1, 3, 1.4f, 180.f, 220.f, ComportamientoEnemigo::NaveDiagonalDerecha},
        

        // {13.f, TipoEnemigo::Alien, 1, 2, 3, 1.8f, 450.f, 0.f, ComportamientoEnemigo::AlienLento},
        // {16.f, TipoEnemigo::Esbirro, 4, 1, 1, 1.1f, 140.f, 230.f, ComportamientoEnemigo::EsbirroRapido},
        // {21.f, TipoEnemigo::Alien, 3, 1, 3, 2.2f, 170.f, 280.f, ComportamientoEnemigo::AlienRapido},
        // {27.f, TipoEnemigo::Nave, 1, 3, 2, 1.f, 120.f, 230.f, ComportamientoEnemigo::NaveDiagonalIzquierda},
    };

    return oleadas;
}
