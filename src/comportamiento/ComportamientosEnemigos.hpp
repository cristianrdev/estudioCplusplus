#pragma once

enum class ComportamientoEnemigo
{
    EsbirroNormal,
    EsbirroRapido,
    AlienNormal,
    AlienLento,
    AlienRapido,
    NaveDiagonalDerecha,
    NaveDiagonalIzquierda,
    MoluscoGiratorio,
    MiniBossMolusco,
    PescadoGigante
};

struct MovimientoEnemigo
{
    float amplitudOVelocidadHorizontal = 0.f;
    float velocidadVertical = 0.f;
    float velocidadOscilacion = 0.f;
};

struct MovimientoMiniBoss
{
    float alturaEspera = 200.f;
    float duracionEspera = 10.f;
    float velocidadHorizontal = 3.f;
};

struct ConfiguracionComportamientoEnemigo
{
    const char* nombre;
    MovimientoEnemigo movimiento;
    MovimientoMiniBoss miniBoss;
};

const ConfiguracionComportamientoEnemigo& obtenerComportamientoEnemigo(
    ComportamientoEnemigo comportamiento);
