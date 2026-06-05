from pathlib import Path
from PIL import Image


ORIGEN = Path(
    r"C:\Users\luxo2\.codex\generated_images\019e85a2-b08d-7db1-8bfc-30ab60221be3"
    r"\ig_01f3f5c884327946016a2214345b0081919b962847465cb699.png"
)
DESTINO = Path("assets/explosiones/explosion_cangrejo_metalico.png")


def es_chroma(pixel):
    r, g, b, _a = pixel
    return g > 95 and g > r * 1.18 and g > b * 1.18 and g - max(r, b) > 22


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


def main():
    imagen = quitar_chroma(Image.open(ORIGEN))
    caja = imagen.getchannel("A").getbbox()
    if caja:
        margen = 8
        caja = (
            max(0, caja[0] - margen),
            max(0, caja[1] - margen),
            min(imagen.size[0], caja[2] + margen),
            min(imagen.size[1], caja[3] + margen),
        )
        imagen = imagen.crop(caja)
    imagen.save(DESTINO)
    print(f"Generado {DESTINO} {imagen.size}")


if __name__ == "__main__":
    main()
