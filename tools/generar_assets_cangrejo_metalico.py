from collections import deque
from pathlib import Path

from PIL import Image


FUENTE = Path(
    r"C:\Users\luxo2\.codex\generated_images\019e85a2-b08d-7db1-8bfc-30ab60221be3"
    r"\ig_02036dd1c00c7c4f016a1f7df18328819197f2651156d27153.png"
)
SALIDA = Path("assets")
TAMANIO_FRAME = (1280, 1024)
ESCALA_FINAL = 0.34


def es_fondo(pixel):
    r, g, b, _ = pixel
    return max(r, g, b) < 48 and abs(r - g) < 18 and abs(g - b) < 18


def remover_fondo(imagen):
    imagen = imagen.convert("RGBA")
    pixeles = imagen.load()
    ancho, alto = imagen.size
    visitado = set()
    cola = deque()

    for x in range(ancho):
        cola.append((x, 0))
        cola.append((x, alto - 1))
    for y in range(alto):
        cola.append((0, y))
        cola.append((ancho - 1, y))

    while cola:
        x, y = cola.popleft()
        if (x, y) in visitado or not (0 <= x < ancho and 0 <= y < alto):
            continue
        if not es_fondo(pixeles[x, y]):
            continue

        visitado.add((x, y))
        pixeles[x, y] = (0, 0, 0, 0)
        cola.extend(((x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)))

    return imagen


def recortar_con_margen(imagen, margen=24):
    bbox = imagen.getchannel("A").getbbox()
    if bbox is None:
        return imagen

    izquierda = max(0, bbox[0] - margen)
    arriba = max(0, bbox[1] - margen)
    derecha = min(imagen.size[0], bbox[2] + margen)
    abajo = min(imagen.size[1], bbox[3] + margen)
    return imagen.crop((izquierda, arriba, derecha, abajo))


def normalizar_tamanio(imagen, tamanio_objetivo):
    lienzo = Image.new("RGBA", tamanio_objetivo, (0, 0, 0, 0))
    posicion = (
        (tamanio_objetivo[0] - imagen.size[0]) // 2,
        (tamanio_objetivo[1] - imagen.size[1]) // 2,
    )
    lienzo.alpha_composite(imagen, posicion)
    return lienzo


def escalar_para_juego(imagen):
    ancho = max(1, int(imagen.size[0] * ESCALA_FINAL))
    alto = max(1, int(imagen.size[1] * ESCALA_FINAL))
    return imagen.resize((ancho, alto), Image.Resampling.NEAREST)


def desplazar_lateral(imagen, pixeles):
    lienzo = Image.new("RGBA", imagen.size, (0, 0, 0, 0))
    lienzo.alpha_composite(imagen, (pixeles, 0))
    return lienzo


def main():
    fuente = Image.open(FUENTE).convert("RGBA")
    mitad = fuente.size[0] // 2
    crops = [
        fuente.crop((0, 0, mitad, fuente.size[1])),
        fuente.crop((mitad, 0, fuente.size[0], fuente.size[1])),
    ]

    frames = [recortar_con_margen(remover_fondo(crop)) for crop in crops]
    maximo = (max(frame.size[0] for frame in frames), max(frame.size[1] for frame in frames))
    frames = [normalizar_tamanio(frame, maximo) for frame in frames]

    frame_izquierda = desplazar_lateral(frames[0], -10)
    frame_centro = normalizar_tamanio(frames[0], maximo)
    frame_derecha = desplazar_lateral(frames[1], 10)

    for indice, frame in enumerate((frame_izquierda, frame_centro, frame_derecha), start=1):
        escalar_para_juego(frame).save(SALIDA / f"cangrejo_metalico_{indice}.png")

    crear_orbe().save(SALIDA / "proyectil_orbe_verde.png")


def crear_orbe():
    im = Image.new("RGBA", (48, 48), (0, 0, 0, 0))
    pixeles = im.load()
    centro = 24
    for y in range(48):
        for x in range(48):
            dx = x - centro
            dy = y - centro
            d2 = dx * dx + dy * dy
            if d2 <= 15 * 15:
                pixeles[x, y] = (5, 10, 8, 255)
            if d2 <= 11 * 11:
                pixeles[x, y] = (8, 94, 38, 255)
            if d2 <= 8 * 8:
                pixeles[x, y] = (42, 234, 86, 255)
            if (x - 21) * (x - 21) + (y - 20) * (y - 20) <= 4 * 4:
                pixeles[x, y] = (168, 255, 175, 255)

    for x in range(5, 43):
        for y in range(22, 26):
            if pixeles[x, y][3] == 0:
                pixeles[x, y] = (38, 255, 94, 150)
    for y in range(5, 43):
        for x in range(22, 26):
            if pixeles[x, y][3] == 0:
                pixeles[x, y] = (38, 255, 94, 110)
    return im


if __name__ == "__main__":
    main()
