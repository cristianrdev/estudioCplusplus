#include "OrquestacionFondo.hpp"

const std::vector<AparicionFondo>& obtenerAparicionesFondo()
{
    // tiempo, tipo, cantidad, x inicial, separacion x, velocidad y, comportamiento
    static const std::vector<AparicionFondo> apariciones = {
        {30.f, TipoFondo::SectorRocoso, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
    };

    return apariciones;
}
