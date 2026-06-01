#pragma once

#include <vector>

enum class TipoEnemigo
{
    Nave,
    Alien,
    Esbirro,
    MiniBossMolusco
};

struct MovimientoCoseno
{
    float amplitud;
    float velocidadVertical;
    float velocidadOscilacion;
};

struct ConfiguracionMiniBoss
{
    float alturaEspera = 200.f;
    float duracionEspera = 10.f;
    float velocidadHorizontal = 3.f;
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
    MovimientoCoseno movimiento;
    ConfiguracionMiniBoss miniBoss;
};

const std::vector<OleadaEnemigos>& obtenerOleadasEnemigos();
