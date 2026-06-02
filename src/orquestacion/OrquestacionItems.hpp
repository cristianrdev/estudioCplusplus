#pragma once

#include <vector>

enum class TipoItem
{
    CapsulaLaserDoble
};

enum class ComportamientoItem
{
    RectoLento
};

struct AparicionItem
{
    float tiempoSegundos;
    TipoItem tipo;
    int cantidad;
    int vida;
    float posicionXInicial;
    float separacionX;
    float velocidadY;
    ComportamientoItem comportamiento;
};

const std::vector<AparicionItem>& obtenerAparicionesItems();
