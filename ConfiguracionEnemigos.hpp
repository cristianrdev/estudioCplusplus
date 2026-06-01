#pragma once

#include <vector>

enum class TipoEnemigo
{
    Nave,
    Alien
};

struct MovimientoCoseno
{
    float amplitud;
    float velocidadVertical;
    float velocidadOscilacion;
};

struct OleadaEnemigos
{
    float tiempoSegundos;
    TipoEnemigo tipo;
    int cantidad;
    int danio;
    float posicionXInicial;
    float separacionX;
    MovimientoCoseno movimiento;
};

const std::vector<OleadaEnemigos>& obtenerOleadasEnemigos();
