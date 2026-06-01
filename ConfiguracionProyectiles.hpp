#pragma once

#include <vector>

enum class TipoProyectilEnemigo
{
    LaserCeleste,
    BolaEnergiaPurpura,
    PuntoEnergiaAmarillo
};

struct ConfiguracionProyectilEnemigo
{
    TipoProyectilEnemigo tipo;
    const char* rutaTextura;
    float escala;
    int danio;
    bool desapareceAlImpactar;
};

const ConfiguracionProyectilEnemigo& obtenerConfiguracionProyectil(
    TipoProyectilEnemigo tipo);
const std::vector<ConfiguracionProyectilEnemigo>& obtenerConfiguracionesProyectiles();
