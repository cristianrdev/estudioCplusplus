#pragma once

#include <vector>

enum class TipoFondo
{
    TerrenoRocoso01,
    TerrenoRocoso02,
    TerrenoRocoso03,
    TerrenoRocoso04,
    TerrenoRocoso05,
    TerrenoRocoso06,
    TerrenoRocoso07,
    TerrenoRocoso08,
    TerrenoRocoso09,
    TerrenoRocoso10,
    TerrenoRocoso11,
    TerrenoRocoso12,
    TerrenoRocoso13,
    TerrenoRocoso14,
    TerrenoRocoso15,
    TerrenoRocoso16,
    TerrenoRocoso17,
    TerrenoRocoso18,
    TerrenoRocoso19,
    TerrenoRocoso20
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
