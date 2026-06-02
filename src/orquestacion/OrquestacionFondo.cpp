#include "OrquestacionFondo.hpp"

const std::vector<AparicionFondo>& obtenerAparicionesFondo()
{
    // tiempo, tipo, cantidad, x inicial, separacion x, velocidad y, comportamiento
    static const std::vector<AparicionFondo> apariciones = {
        {0.0000f, TipoFondo::TerrenoRocoso01, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {2.8444f, TipoFondo::TerrenoRocoso02, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {5.6889f, TipoFondo::TerrenoRocoso03, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {8.5333f, TipoFondo::TerrenoRocoso04, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {11.3778f, TipoFondo::TerrenoRocoso05, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {14.2222f, TipoFondo::TerrenoRocoso06, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {17.0667f, TipoFondo::TerrenoRocoso07, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {19.9111f, TipoFondo::TerrenoRocoso08, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {22.7556f, TipoFondo::TerrenoRocoso09, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {25.6000f, TipoFondo::TerrenoRocoso10, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {28.4444f, TipoFondo::TerrenoRocoso11, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {31.2889f, TipoFondo::TerrenoRocoso12, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {34.1333f, TipoFondo::TerrenoRocoso13, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {36.9778f, TipoFondo::TerrenoRocoso14, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {39.8222f, TipoFondo::TerrenoRocoso15, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {42.6667f, TipoFondo::TerrenoRocoso16, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {45.5111f, TipoFondo::TerrenoRocoso17, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {48.3556f, TipoFondo::TerrenoRocoso18, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {51.2000f, TipoFondo::TerrenoRocoso19, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
        {54.0444f, TipoFondo::TerrenoRocoso20, 1, 0.f, 0.f, 1.5f, ComportamientoFondo::RectoAbajo},
    };

    return apariciones;
}
