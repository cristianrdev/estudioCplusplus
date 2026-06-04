from PIL import Image, ImageDraw
import random


ATLAS = "assets/elementos_fondo/atlas_rocas_plataforma.png"

ROCAS = [
    ("RocaPlataforma700", 700, 260, 0),
    ("RocaPlataforma300A", 300, 150, 260),
    ("RocaPlataforma300B", 300, 150, 410),
    ("RocaPlataforma150A", 150, 92, 560),
    ("RocaPlataforma150B", 150, 92, 652),
]

PALETA = {
    "sombra": (55, 32, 19, 255),
    "sombra2": (77, 45, 25, 255),
    "borde": (91, 56, 31, 255),
    "borde_claro": (151, 101, 59, 255),
    "superficie": (168, 116, 70, 255),
    "superficie2": (184, 132, 82, 255),
    "luz": (213, 164, 105, 255),
    "grieta": (72, 42, 25, 255),
}


def punto_irregular(cx, cy, rx, ry, pasos, rng):
    puntos = []
    for i in range(pasos):
        ang = 2.0 * 3.14159265 * i / pasos
        variacion = rng.uniform(0.82, 1.12)
        if i % 5 == 0:
            variacion *= rng.uniform(0.88, 1.18)
        x = cx + int(round(rx * variacion * __import__("math").cos(ang)))
        y = cy + int(round(ry * variacion * __import__("math").sin(ang)))
        puntos.append((x, y))
    return puntos


def desplazar(puntos, dx, dy):
    return [(x + dx, y + dy) for x, y in puntos]


def dibujar_roca(imagen, draw, x0, y0, w, h, seed):
    rng = random.Random(seed)
    margen_x = max(12, w // 24)
    top_h = int(h * 0.55)
    cx = x0 + w // 2
    cy = y0 + int(h * 0.38)
    rx = w // 2 - margen_x
    ry = max(28, int(top_h * 0.45))

    contorno = punto_irregular(cx, cy, rx, ry, 34 if w >= 300 else 22, rng)
    sombra = desplazar(contorno, 0, int(h * 0.24))
    pared = contorno + list(reversed(sombra))

    draw.polygon(pared, fill=PALETA["sombra2"])
    draw.line(sombra + [sombra[0]], fill=PALETA["sombra"], width=4)
    draw.polygon(contorno, fill=PALETA["superficie"])
    draw.line(contorno + [contorno[0]], fill=PALETA["borde"], width=4)
    draw.line(desplazar(contorno, 0, 2) + [desplazar(contorno, 0, 2)[0]], fill=PALETA["borde_claro"], width=2)

    # Labio frontal para dar perspectiva similar a las rocas del atlas.
    frente = [(x, y) for x, y in contorno if y >= cy]
    if frente:
        frente_ordenado = sorted(frente)
        draw.line(frente_ordenado, fill=PALETA["sombra"], width=5)
        draw.line([(x, y - 3) for x, y in frente_ordenado], fill=PALETA["borde_claro"], width=2)

    # Placas planas de superficie para poder apoyar objetos encima.
    cantidad_placas = max(5, w // 70)
    for _ in range(cantidad_placas):
        pw = rng.randint(max(18, w // 18), max(28, w // 8))
        ph = rng.randint(12, max(18, h // 10))
        px = rng.randint(x0 + margen_x, x0 + w - margen_x - pw)
        py = rng.randint(y0 + int(h * 0.22), y0 + int(h * 0.48))
        color = PALETA["superficie2"] if rng.random() < 0.55 else PALETA["superficie"]
        draw.rounded_rectangle((px, py, px + pw, py + ph), radius=3, fill=color, outline=PALETA["grieta"], width=1)
        draw.line((px + 3, py + 2, px + pw - 4, py + 2), fill=PALETA["luz"], width=1)

    # Grietas y cortes pixelados.
    for _ in range(max(8, w // 45)):
        sx = rng.randint(x0 + margen_x, x0 + w - margen_x)
        sy = rng.randint(y0 + int(h * 0.18), y0 + int(h * 0.56))
        largo = rng.randint(8, max(12, w // 18))
        pasos = rng.randint(2, 4)
        pts = [(sx, sy)]
        for _paso in range(pasos):
            sx += rng.choice([-1, 1]) * rng.randint(4, max(6, largo // pasos))
            sy += rng.randint(-3, 5)
            pts.append((sx, sy))
        draw.line(pts, fill=PALETA["grieta"], width=2)
        draw.point(pts[-1], fill=PALETA["luz"])

    # Rocas pequeñas en el borde inferior para la sensación flotante.
    for _ in range(max(9, w // 42)):
        r = rng.randint(3, 8 if w >= 300 else 5)
        px = rng.randint(x0 + 8, x0 + w - 8)
        py = rng.randint(y0 + int(h * 0.58), y0 + h - r - 4)
        color = rng.choice([PALETA["borde"], PALETA["sombra2"], PALETA["superficie"]])
        draw.rectangle((px - r, py - r // 2, px + r, py + r), fill=color)
        draw.line((px - r, py - r // 2, px + r, py - r // 2), fill=PALETA["borde_claro"], width=1)

    # Pixel dithering controlado, tonos cafe consistentes.
    for _ in range(w * h // 220):
        px = rng.randint(x0 + 4, x0 + w - 5)
        py = rng.randint(y0 + 4, y0 + h - 5)
        if imagen.getpixel((px, py))[3] == 0:
            continue
        color = rng.choice([PALETA["grieta"], PALETA["borde_claro"], PALETA["superficie2"]])
        draw.rectangle((px, py, px + 1, py + 1), fill=color)


def main():
    ancho = 700
    alto = ROCAS[-1][3] + ROCAS[-1][2]
    atlas = Image.new("RGBA", (ancho, alto), (0, 0, 0, 0))
    draw = ImageDraw.Draw(atlas)

    for indice, (_nombre, w, h, y) in enumerate(ROCAS):
        dibujar_roca(atlas, draw, 0, y, w, h, 0x7150 + indice * 97)

    atlas.save(ATLAS)
    print(f"Generado {ATLAS} {atlas.size}")


if __name__ == "__main__":
    main()
