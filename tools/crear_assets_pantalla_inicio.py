from pathlib import Path
from PIL import Image, ImageDraw, ImageFont, ImageOps


FONDO_IA = Path(
    r"C:\Users\luxo2\.codex\generated_images\019e85a2-b08d-7db1-8bfc-30ab60221be3"
    r"\ig_01f3f5c884327946016a20d215f9c88191a3ed8f556042bb5a.png"
)
LOGO_IA = Path(
    r"C:\Users\luxo2\.codex\generated_images\019e85a2-b08d-7db1-8bfc-30ab60221be3"
    r"\ig_01f3f5c884327946016a20d27abafc8191846e8a1dee5176f6.png"
)

SALIDA_FONDO = Path("assets/pantalla_inicio.png")
SALIDA_TITULO = Path("assets/titulo_retroid_x.png")
FUENTE = Path(r"C:\Windows\Fonts\consola.ttf")


def es_chroma(pixel):
    r, g, b, _a = pixel
    return g > 105 and g > r * 1.18 and g > b * 1.18 and g - max(r, b) > 22


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


def bbox_alpha(imagen):
    return imagen.getchannel("A").getbbox()


def dibujar_texto_pixel(draw, texto, centro, fuente, relleno, sombra, borde):
    caja = draw.textbbox((0, 0), texto, font=fuente, stroke_width=2)
    ancho = caja[2] - caja[0]
    alto = caja[3] - caja[1]
    x = int(centro[0] - ancho / 2)
    y = int(centro[1] - alto / 2)
    for dx, dy in [(-4, 0), (4, 0), (0, -4), (0, 4), (-3, -3), (3, -3), (-3, 3), (3, 3)]:
        draw.text((x + dx, y + dy), texto, font=fuente, fill=borde, stroke_width=2, stroke_fill=borde)
    draw.text((x + 4, y + 5), texto, font=fuente, fill=sombra, stroke_width=2, stroke_fill=sombra)
    draw.text((x, y), texto, font=fuente, fill=relleno, stroke_width=2, stroke_fill=(35, 0, 70, 255))


def crear_fondo():
    fondo = Image.open(FONDO_IA).convert("RGBA")
    fondo = ImageOps.fit(fondo, (1024, 1010), method=Image.Resampling.LANCZOS, centering=(0.5, 0.5))
    draw = ImageDraw.Draw(fondo)
    fuente = ImageFont.truetype(str(FUENTE), 54)
    dibujar_texto_pixel(
        draw,
        "PRESS START",
        (512, 824),
        fuente,
        (255, 225, 255, 255),
        (28, 0, 48, 255),
        (255, 45, 235, 255),
    )
    fondo.save(SALIDA_FONDO)


def crear_titulo():
    logo = quitar_chroma(Image.open(LOGO_IA))
    caja = bbox_alpha(logo)
    if caja:
        logo = logo.crop(caja)
    escala = min(760 / logo.size[0], 245 / logo.size[1])
    nuevo_tamano = (
        max(1, int(round(logo.size[0] * escala))),
        max(1, int(round(logo.size[1] * escala))),
    )
    logo = logo.resize(nuevo_tamano, Image.Resampling.LANCZOS)
    salida = Image.new("RGBA", nuevo_tamano, (0, 0, 0, 0))
    salida.alpha_composite(logo, (0, 0))
    salida.save(SALIDA_TITULO)


def main():
    crear_fondo()
    crear_titulo()
    print(f"Generados {SALIDA_FONDO} y {SALIDA_TITULO}")


if __name__ == "__main__":
    main()
