from pathlib import Path
from random import Random

from PIL import Image


TILE = 128
COLUMNAS_PANTALLA = 8
FILAS_SECTOR = 16
COLUMNAS_ATLAS = 16
RAIZ = Path(__file__).resolve().parents[1]
FUENTE = RAIZ / "assets" / "atlas_terreno_rocoso_respaldo_20_tiles.png"
SALIDA = RAIZ / "assets" / "atlas_terreno_rocoso.png"


def obtener_franja(fuente: Image.Image, indice: int) -> Image.Image:
    x = indice % 2 * 1024
    y = indice // 2 * 256
    return fuente.crop((x, y, x + 1024, y + 256)).convert("RGBA")


def transparentar_negro(tile: Image.Image) -> None:
    pixeles = tile.load()
    for y in range(TILE):
        for x in range(TILE):
            rojo, verde, azul, alpha = pixeles[x, y]
            if rojo < 18 and verde < 18 and azul < 18:
                pixeles[x, y] = (rojo, verde, azul, 0)


def crear_orilla_salida(tiles: list[Image.Image]) -> None:
    rng = Random(2718)
    anclas = [rng.randint(45, 73) for _ in range(COLUMNAS_PANTALLA + 1)]
    for columna, tile in enumerate(tiles):
        pixeles = tile.load()
        inicio = anclas[columna]
        fin = anclas[columna + 1]
        for x in range(TILE):
            proporcion = x / (TILE - 1)
            borde = round(inicio + (fin - inicio) * proporcion)
            borde += ((x // 8 + columna) % 3) * 2 - 2
            for y in range(borde):
                rojo, verde, azul, _ = pixeles[x, y]
                pixeles[x, y] = (rojo, verde, azul, 0)


def crear_tiles(fuente: Image.Image) -> list[Image.Image]:
    tiles = []
    for fila in range(FILAS_SECTOR):
        indice_franja = fila // 2
        mitad_inferior_primero = 1 - fila % 2
        franja = obtener_franja(fuente, indice_franja)
        y = mitad_inferior_primero * TILE
        fila_tiles = [
            franja.crop((columna * TILE, y, (columna + 1) * TILE, y + TILE))
            for columna in range(COLUMNAS_PANTALLA)
        ]
        if fila == 0:
            for tile in fila_tiles:
                transparentar_negro(tile)
        elif fila == FILAS_SECTOR - 1:
            crear_orilla_salida(fila_tiles)
        tiles.extend(fila_tiles)
    return tiles


def main() -> None:
    fuente = Image.open(FUENTE).convert("RGBA")
    tiles = crear_tiles(fuente)
    filas_atlas = (len(tiles) + COLUMNAS_ATLAS - 1) // COLUMNAS_ATLAS
    atlas = Image.new(
        "RGBA",
        (COLUMNAS_ATLAS * TILE, filas_atlas * TILE),
        (0, 0, 0, 0),
    )
    for indice, tile in enumerate(tiles):
        x = indice % COLUMNAS_ATLAS * TILE
        y = indice // COLUMNAS_ATLAS * TILE
        atlas.paste(tile, (x, y), tile)
    atlas.save(SALIDA, optimize=True)
    print(f"Atlas generado: {SALIDA} ({atlas.width}x{atlas.height})")


if __name__ == "__main__":
    main()
