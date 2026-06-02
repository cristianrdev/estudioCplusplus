#include "OrquestacionFondo.hpp"

const std::vector<AparicionFondo>& obtenerAparicionesFondo()
{
    // tiempo, tipo, cantidad, x inicial, separacion x, velocidad y, comportamiento
    static const std::vector<AparicionFondo> apariciones = {
        {30.0000f, TipoFondo::TerrenoRocoso01, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {32.8444f, TipoFondo::TerrenoRocoso02, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {35.6889f, TipoFondo::TerrenoRocoso03, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {38.5333f, TipoFondo::TerrenoRocoso04, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {41.3778f, TipoFondo::TerrenoRocoso05, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {44.2222f, TipoFondo::TerrenoRocoso06, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {47.0667f, TipoFondo::TerrenoRocoso07, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {49.9111f, TipoFondo::TerrenoRocoso08, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {52.7556f, TipoFondo::TerrenoRocoso09, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {55.6000f, TipoFondo::TerrenoRocoso10, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {58.4444f, TipoFondo::TerrenoRocoso11, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {61.2889f, TipoFondo::TerrenoRocoso12, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {64.1333f, TipoFondo::TerrenoRocoso13, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {66.9778f, TipoFondo::TerrenoRocoso14, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {69.8222f, TipoFondo::TerrenoRocoso15, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {72.6667f, TipoFondo::TerrenoRocoso16, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {75.5111f, TipoFondo::TerrenoRocoso17, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {78.3556f, TipoFondo::TerrenoRocoso18, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {81.2000f, TipoFondo::TerrenoRocoso19, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {84.0444f, TipoFondo::TerrenoRocoso20, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
    };

    return apariciones;
}
