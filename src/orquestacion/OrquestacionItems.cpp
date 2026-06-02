#include "OrquestacionItems.hpp"

const std::vector<AparicionItem>& obtenerAparicionesItems()
{
    // tiempo, tipo, cantidad, vida, x inicial, separacion x, velocidad y, comportamiento
    static const std::vector<AparicionItem> apariciones = {
        {45.f, TipoItem::CapsulaLaserDoble, 1, 4, 512.f, 0.f, 1.5f, ComportamientoItem::RectoLento},
    };

    return apariciones;
}
