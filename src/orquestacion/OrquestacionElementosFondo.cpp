#include "OrquestacionElementosFondo.hpp"

const std::vector<AparicionElementoFondo> &obtenerAparicionesElementosFondo()
{
    // tiempo, tipo, cantidad, x inicial, separacion x, velocidad y, comportamiento
    static const std::vector<AparicionElementoFondo> apariciones = {
        {40.5f, TipoElementoFondo::Roca01, 1, 120.f, 720.f, 1.5f, ComportamientoElementoFondo::RectoAbajo},

        {46.f, TipoElementoFondo::Roca06, 1, 900.f, 0.f, 1.5f, ComportamientoElementoFondo::RectoAbajo},
        {48.f, TipoElementoFondo::Roca02, 1, 400.f, 590.f, 1.5f, ComportamientoElementoFondo::RectoAbajo},

        // {40.f, TipoElementoFondo::Roca07, 1, 680.f, 0.f, 1.5f, ComportamientoElementoFondo::RectoAbajo},
        // {44.f, TipoElementoFondo::Roca03, 1, 310.f, 0.f, 1.5f, ComportamientoElementoFondo::RectoAbajo},
        // {48.f, TipoElementoFondo::Roca08, 2, 140.f, 730.f, 1.5f, ComportamientoElementoFondo::RectoAbajo},
        // {53.f, TipoElementoFondo::Roca04, 1, 540.f, 0.f, 1.5f, ComportamientoElementoFondo::RectoAbajo},
        // {58.f, TipoElementoFondo::Roca09, 2, 250.f, 560.f, 1.5f, ComportamientoElementoFondo::RectoAbajo},
        // {64.f, TipoElementoFondo::Roca05, 1, 760.f, 0.f, 1.5f, ComportamientoElementoFondo::RectoAbajo},
        // {70.f, TipoElementoFondo::Roca10, 2, 170.f, 680.f, 1.5f, ComportamientoElementoFondo::RectoAbajo},
    };

    return apariciones;
}
