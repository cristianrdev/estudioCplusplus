#pragma once

#include <vector>

enum class TipoFondo
{
    SectorRocoso
};

enum class ComportamientoFondo
{
    RectoAbajo
};

struct AparicionFondo
{
    float tiempoSegundos;
    TipoFondo tipo;
    int cantidad;
    float posicionXInicial;
    float separacionX;
    float velocidadY;
    ComportamientoFondo comportamiento;
};

const std::vector<AparicionFondo>& obtenerAparicionesFondo();
