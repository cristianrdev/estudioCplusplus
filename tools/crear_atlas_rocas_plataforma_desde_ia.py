from pathlib import Path
from PIL import Image


ORIGEN = Path(
    r"C:\Users\luxo2\.codex\generated_images\019e85a2-b08d-7db1-8bfc-30ab60221be3"
    r"\ig_01f3f5c884327946016a20b73222d48191bae6823706517784.png"
)
DESTINO = Path("assets/elementos_fondo/atlas_rocas_plataforma.png")

SLOTS = [
    (0, 0, 700, 260),
    (0, 260, 300, 150),
    (0, 410, 300, 150),
    (0, 560, 150, 92),
    (0, 652, 150, 92),
]


def es_chroma(pixel):
    r, g, b, _a = pixel
    return g > 95 and g > r * 1.22 and g > b * 1.22 and g - max(r, b) > 24


def quitar_chroma(imagen):
    rgba = imagen.convert("RGBA")
    pixeles = rgba.load()
    ancho, alto = rgba.size
    for y in range(alto):
        for x in range(ancho):
            if es_chroma(pixeles[x, y]):
                pixeles[x, y] = (0, 0, 0, 0)
            else:
                r, g, b, a = pixeles[x, y]
                if g > r and g > b:
                    g = min(g, max(r, b))
                    pixeles[x, y] = (r, g, b, a)
    return rgba


def bbox_fila(imagen):
    pixeles = imagen.load()
    ancho, alto = imagen.size
    filas = []
    for y in range(alto):
        cantidad = 0
        for x in range(ancho):
            if pixeles[x, y][3] > 0:
                cantidad += 1
        filas.append(cantidad)
    return filas


def encontrar_bandas(imagen):
    filas = bbox_fila(imagen)
    bandas = []
    inicio = None
    hueco = 0
    umbral = max(8, imagen.size[0] // 140)
    hueco_maximo = 18
    for y, cantidad in enumerate(filas):
        if cantidad > umbral:
            if inicio is None:
                inicio = y
            hueco = 0
        elif inicio is not None:
            hueco += 1
            if hueco > hueco_maximo:
                bandas.append((inicio, y - hueco))
                inicio = None
                hueco = 0
    if inicio is not None:
        bandas.append((inicio, len(filas) - 1))
    return bandas[:5]


def recortar_banda(imagen, y0, y1):
    pixeles = imagen.load()
    ancho, _alto = imagen.size
    xs = []
    for y in range(max(0, y0), min(imagen.size[1], y1 + 1)):
        for x in range(ancho):
            if pixeles[x, y][3] > 0:
                xs.append(x)
    if not xs:
        return Image.new("RGBA", (1, 1), (0, 0, 0, 0))
    margen = 8
    x0 = max(0, min(xs) - margen)
    x1 = min(ancho, max(xs) + margen + 1)
    y0 = max(0, y0 - margen)
    y1 = min(imagen.size[1], y1 + margen + 1)
    return imagen.crop((x0, y0, x1, y1))


def pegar_en_slot(atlas, sprite, slot):
    x, y, ancho, alto = slot
    margen = 4
    escala = min((ancho - margen * 2) / sprite.size[0], (alto - margen * 2) / sprite.size[1])
    nuevo_tamano = (
        max(1, int(round(sprite.size[0] * escala))),
        max(1, int(round(sprite.size[1] * escala))),
    )
    sprite = sprite.resize(nuevo_tamano, Image.Resampling.LANCZOS)
    px = x + (ancho - nuevo_tamano[0]) // 2
    py = y + (alto - nuevo_tamano[1]) // 2
    atlas.alpha_composite(sprite, (px, py))


def main():
    fuente = quitar_chroma(Image.open(ORIGEN))
    bandas = encontrar_bandas(fuente)
    if len(bandas) != 5:
        raise RuntimeError(f"Se esperaban 5 bandas de rocas, se detectaron {len(bandas)}")

    atlas = Image.new("RGBA", (700, 744), (0, 0, 0, 0))
    for banda, slot in zip(bandas, SLOTS):
        sprite = recortar_banda(fuente, *banda)
        pegar_en_slot(atlas, sprite, slot)

    atlas.save(DESTINO)
    print(f"Generado {DESTINO} desde IA con {len(bandas)} sprites")


if __name__ == "__main__":
    main()
