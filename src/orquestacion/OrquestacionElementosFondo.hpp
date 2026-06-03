#pragma once

#include <vector>

enum class TipoElementoFondo
{
    Roca01,
    Roca02,
    Roca03,
    Roca04,
    Roca05,
    Roca06,
    Roca07,
    Roca08,
    Roca09,
    Roca10,
    LagoCeleste500,
    LagoCeleste300,
    LagoCeleste200
};

enum class ComportamientoElementoFondo
{
    RectoAbajo
};

struct AparicionElementoFondo
{
    float tiempoSegundos;
    TipoElementoFondo tipo;
    int cantidad;
    float posicionXInicial;
    float separacionX;
    float velocidadY;
    ComportamientoElementoFondo comportamiento;
};

const std::vector<AparicionElementoFondo>& obtenerAparicionesElementosFondo();
