#include "ConfiguracionProyectiles.hpp"

#include <stdexcept>

const std::vector<ConfiguracionProyectilEnemigo>& obtenerConfiguracionesProyectiles()
{
    // Agrega una fila y un valor al enum para registrar nuevos sprites de proyectiles.
    static const std::vector<ConfiguracionProyectilEnemigo> configuraciones = {
        // tipo, ruta, escala, danio, desaparece al impactar
        {TipoProyectilEnemigo::LaserCeleste, "assets/proyectiles/proyectil_nave.png", 0.03f, 1, false},
        {TipoProyectilEnemigo::BolaEnergiaPurpura, "assets/proyectiles/proyectil_alien.png", 0.075f, 2, true},
        {TipoProyectilEnemigo::PuntoEnergiaAmarillo, "assets/proyectiles/proyectil_esbirro.png", 0.16f, 1, true},
        {TipoProyectilEnemigo::EscamaMetalica, "assets/proyectiles/proyectil_escama.png", 0.065f, 1, true},
        {TipoProyectilEnemigo::OrbeVerde, "assets/proyectiles/proyectil_orbe_verde.png", 0.55f, 2, true},
        {TipoProyectilEnemigo::OrbeRojoTortuga, "assets/proyectiles/proyectil_tortuga_rojo.png", 1.1f, 1, true},
    };

    return configuraciones;
}

const ConfiguracionProyectilEnemigo& obtenerConfiguracionProyectil(
    TipoProyectilEnemigo tipo)
{
    for (const auto& configuracion : obtenerConfiguracionesProyectiles())
    {
        if (configuracion.tipo == tipo)
            return configuracion;
    }

    throw std::invalid_argument("Tipo de proyectil enemigo sin configurar");
}
