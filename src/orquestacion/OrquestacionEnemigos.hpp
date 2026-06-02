#pragma once

#include "../../ComportamientosEnemigos.hpp"

#include <vector>

enum class TipoEnemigo
{
    Nave,
    Alien,
    Esbirro,
    MiniBossMolusco
};

struct OleadaEnemigos
{
    float tiempoSegundos;
    TipoEnemigo tipo;
    int cantidad;
    int danio;
    int vida;
    float frecuenciaDisparo;
    float posicionXInicial;
    float separacionX;
    ComportamientoEnemigo comportamiento;
};

const std::vector<OleadaEnemigos>& obtenerOleadasEnemigos();
