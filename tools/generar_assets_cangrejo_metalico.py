from PIL import Image, ImageDraw


TRANSPARENTE = (0, 0, 0, 0)
CONTORNO = (7, 8, 14, 255)
ROJO_OSCURO = (92, 12, 18, 255)
ROJO = (168, 28, 32, 255)
ROJO_CLARO = (234, 64, 54, 255)
METAL_OSCURO = (54, 57, 62, 255)
METAL = (112, 116, 118, 255)
METAL_CLARO = (190, 194, 186, 255)
VERDE = (42, 234, 86, 255)
VERDE_CLARO = (168, 255, 175, 255)
VERDE_OSCURO = (8, 94, 38, 255)


def rect(draw, xy, color):
    draw.rectangle(xy, fill=color)


def punto(draw, x, y, color, escala=2):
    rect(draw, (x, y, x + escala - 1, y + escala - 1), color)


def contorno_desde_alpha(imagen, radio=2):
    base = imagen.convert("RGBA")
    alpha = base.getchannel("A")
    contorno = Image.new("RGBA", base.size, TRANSPARENTE)
    draw = ImageDraw.Draw(contorno)

    pix = alpha.load()
    ancho, alto = base.size
    for y in range(alto):
        for x in range(ancho):
            if pix[x, y] == 0:
                continue
            for dy in range(-radio, radio + 1):
                for dx in range(-radio, radio + 1):
                    if dx * dx + dy * dy > radio * radio:
                        continue
                    px = x + dx
                    py = y + dy
                    if 0 <= px < ancho and 0 <= py < alto and pix[px, py] == 0:
                        draw.point((px, py), fill=CONTORNO)

    contorno.alpha_composite(base)
    return contorno


def crear_cangrejo(frame):
    im = Image.new("RGBA", (160, 128), TRANSPARENTE)
    d = ImageDraw.Draw(im)

    # Cuerpo principal, visto desde arriba.
    rect(d, (58, 36, 102, 84), ROJO_OSCURO)
    rect(d, (50, 44, 110, 74), ROJO)
    rect(d, (64, 28, 96, 94), ROJO)
    rect(d, (70, 34, 90, 88), ROJO_CLARO)
    rect(d, (74, 42, 86, 72), METAL_OSCURO)
    rect(d, (78, 38, 82, 78), METAL)
    rect(d, (78, 42, 82, 50), METAL_CLARO)

    # Caparazon segmentado.
    for y in (46, 58, 70):
        rect(d, (56, y, 68, y + 3), ROJO_OSCURO)
        rect(d, (92, y, 104, y + 3), ROJO_OSCURO)
    rect(d, (62, 32, 70, 40), METAL)
    rect(d, (90, 32, 98, 40), METAL)
    rect(d, (64, 34, 68, 37), METAL_CLARO)
    rect(d, (92, 34, 96, 37), METAL_CLARO)

    # Pinzas delanteras.
    apertura = 4 if frame == 1 else -2
    rect(d, (30, 26 + apertura, 54, 42 + apertura), ROJO_OSCURO)
    rect(d, (22, 20 + apertura, 38, 30 + apertura), ROJO)
    rect(d, (18, 30 + apertura, 34, 42 + apertura), ROJO)
    rect(d, (24, 24 + apertura, 32, 28 + apertura), ROJO_CLARO)

    rect(d, (106, 26 + apertura, 130, 42 + apertura), ROJO_OSCURO)
    rect(d, (122, 20 + apertura, 138, 30 + apertura), ROJO)
    rect(d, (126, 30 + apertura, 142, 42 + apertura), ROJO)
    rect(d, (128, 24 + apertura, 136, 28 + apertura), ROJO_CLARO)

    # Patas laterales animadas.
    patas = [42, 56, 70, 82]
    for i, y in enumerate(patas):
        delta = (2 if (i + frame) % 2 == 0 else -2)
        rect(d, (38 + delta, y, 52 + delta, y + 5), ROJO_OSCURO)
        rect(d, (26 + delta, y + 4, 40 + delta, y + 9), ROJO)
        rect(d, (18 + delta, y + 8, 28 + delta, y + 13), METAL)
        rect(d, (108 - delta, y, 122 - delta, y + 5), ROJO_OSCURO)
        rect(d, (120 - delta, y + 4, 134 - delta, y + 9), ROJO)
        rect(d, (132 - delta, y + 8, 142 - delta, y + 13), METAL)

    # Canon/ojos mecanicos frontales.
    rect(d, (70, 22, 90, 30), METAL_OSCURO)
    rect(d, (74, 20, 86, 26), METAL)
    rect(d, (78, 20, 82, 24), VERDE_CLARO)
    rect(d, (76, 96, 84, 104), METAL_OSCURO)
    rect(d, (78, 98, 82, 102), VERDE)

    # Brillos de metal y remaches.
    for x, y in ((54, 48), (102, 48), (56, 70), (100, 70), (70, 86), (90, 86)):
        punto(d, x, y, METAL_CLARO, 2)

    return contorno_desde_alpha(im)


def crear_orbe():
    im = Image.new("RGBA", (48, 48), TRANSPARENTE)
    d = ImageDraw.Draw(im)
    d.ellipse((10, 10, 37, 37), fill=CONTORNO)
    d.ellipse((13, 13, 34, 34), fill=VERDE_OSCURO)
    d.ellipse((16, 16, 31, 31), fill=VERDE)
    d.ellipse((19, 18, 25, 24), fill=VERDE_CLARO)
    rect(d, (5, 22, 42, 25), (38, 255, 94, 150))
    rect(d, (22, 5, 25, 42), (38, 255, 94, 110))
    return im


def main():
    crear_cangrejo(0).save("assets/cangrejo_metalico_1.png")
    crear_cangrejo(1).save("assets/cangrejo_metalico_2.png")
    crear_orbe().save("assets/proyectil_orbe_verde.png")


if __name__ == "__main__":
    main()
