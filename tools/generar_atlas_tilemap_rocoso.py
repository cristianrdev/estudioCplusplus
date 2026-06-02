from pathlib import Path
import math
from random import Random

from PIL import Image, ImageDraw


TILE_LOGICO = 128
TILE = 256
MARGEN = 2
PASO = TILE + MARGEN * 2
COLUMNAS_PANTALLA = 8
FILAS_SECTOR = 16
COLUMNAS_ATLAS = 8
TILES_BASE = 1
INDICE_ENTRADA = 1
INDICE_SALIDA = 9

RAIZ = Path(__file__).resolve().parents[1]
SALIDA = RAIZ / "assets" / "atlas_terreno_rocoso.png"


def color_base(x: int, y: int, variante: int) -> tuple[int, int, int, int]:
    ruido = ((x * 73856093) ^ (y * 19349663) ^ (variante * 83492791)) & 31
    ruido -= 15
    veta = ((x // 64) ^ (y // 61)) & 1
    return (
        124 + ruido // 2 + veta,
        76 + ruido // 3 + veta,
        42 + ruido // 4,
        255,
    )


def dibujar_suelo(variante: int) -> Image.Image:
    tile = Image.new("RGBA", (TILE, TILE), (0, 0, 0, 0))
    pixeles = tile.load()
    for y in range(TILE):
        for x in range(TILE):
            pixeles[x, y] = color_base(x, y, variante)

    rng = Random(1000 + variante)
    draw = ImageDraw.Draw(tile)

    for _ in range(10):
        x = rng.randrange(0, TILE)
        y = rng.randrange(0, TILE)
        r = rng.randrange(1, 3)
        draw.ellipse((x - r, y - r, x + r, y + r), fill=(86, 53, 32, 110))

    return tile


def crear_orilla(variante: int, entrada: bool, columna: int) -> Image.Image:
    tile = dibujar_suelo(variante)
    rng = Random((1729 if entrada else 2718) + columna)
    draw = ImageDraw.Draw(tile)
    pixeles = tile.load()

    borde_base = rng.randrange(80, 136)
    for x in range(TILE):
        borde = borde_base
        borde += int(18 * math.sin((x + columna * 37) / 28))
        borde += ((x // 17 + columna) % 5 - 2) * 4
        for y in range(TILE):
            espacio = y >= borde if entrada else y < borde
            if espacio:
                rojo, verde, azul, _ = pixeles[x, y]
                pixeles[x, y] = (rojo, verde, azul, 0)

    for _ in range(26):
        x = rng.randrange(0, TILE)
        borde = borde_base + int(18 * math.sin((x + columna * 37) / 28))
        y = borde + rng.randrange(-18, 18)
        if not entrada:
            y = borde + rng.randrange(-18, 18)
        r = rng.randrange(7, 18)
        color = (139 + rng.randrange(-12, 13), 86 + rng.randrange(-8, 9), 48, 255)
        sombra = (65, 39, 25, 210)
        draw.ellipse((x - r, y - r, x + r, y + r), fill=sombra)
        draw.ellipse((x - r, y - r - 4, x + r, y + r - 4), fill=color)
        draw.arc((x - r, y - r - 4, x + r, y + r - 4), 210, 330, fill=(186, 125, 72, 230), width=2)

    return tile


def crear_tiles() -> list[Image.Image]:
    tiles = [dibujar_suelo(indice) for indice in range(TILES_BASE)]
    tiles.extend(
        crear_orilla(columna % TILES_BASE, entrada=True, columna=columna)
        for columna in range(COLUMNAS_PANTALLA)
    )
    tiles.extend(
        crear_orilla(columna % TILES_BASE, entrada=False, columna=columna)
        for columna in range(COLUMNAS_PANTALLA)
    )
    return tiles


def pegar_con_margen(atlas: Image.Image, tile: Image.Image, x: int, y: int) -> None:
    atlas.paste(tile, (x + MARGEN, y + MARGEN), tile)
    atlas.paste(tile.crop((0, 0, TILE, 1)), (x + MARGEN, y))
    atlas.paste(tile.crop((0, TILE - 1, TILE, TILE)), (x + MARGEN, y + MARGEN + TILE))
    atlas.paste(tile.crop((0, 0, 1, TILE)), (x, y + MARGEN))
    atlas.paste(tile.crop((TILE - 1, 0, TILE, TILE)), (x + MARGEN + TILE, y + MARGEN))
    atlas.paste(tile.crop((0, 0, 1, 1)), (x, y))
    atlas.paste(tile.crop((TILE - 1, 0, TILE, 1)), (x + MARGEN + TILE, y))
    atlas.paste(tile.crop((0, TILE - 1, 1, TILE)), (x, y + MARGEN + TILE))
    atlas.paste(
        tile.crop((TILE - 1, TILE - 1, TILE, TILE)),
        (x + MARGEN + TILE, y + MARGEN + TILE),
    )


def main() -> None:
    tiles = crear_tiles()
    filas_atlas = (len(tiles) + COLUMNAS_ATLAS - 1) // COLUMNAS_ATLAS
    atlas = Image.new(
        "RGBA",
        (COLUMNAS_ATLAS * PASO, filas_atlas * PASO),
        (0, 0, 0, 0),
    )
    for indice, tile in enumerate(tiles):
        x = indice % COLUMNAS_ATLAS * PASO
        y = indice // COLUMNAS_ATLAS * PASO
        pegar_con_margen(atlas, tile, x, y)
    atlas.save(SALIDA, optimize=True)
    print(f"Atlas generado: {SALIDA} ({atlas.width}x{atlas.height})")


if __name__ == "__main__":
    main()
