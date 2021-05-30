import sys

import numpy as np

_MARGIN = 2
_BG_COLOR = (32, 32, 32)
_LINE_COLOR = (192, 32, 64)

def generate_image(csv_filename, ppm_filename):
    points = get_points(csv_filename)
    bounds = get_canvas_bounds(points)
    width  = abs(bounds[1][0] - bounds[0][0]) + 1 + 2*_MARGIN
    height = abs(bounds[1][1] - bounds[0][1]) + 1 + 2*_MARGIN

    canvas = np.empty((height, width), dtype=tuple)
    canvas.fill(_BG_COLOR)

    for point in points:
        x = point[0] - bounds[0][0] + _MARGIN
        y = point[1] - bounds[0][1] + _MARGIN
        canvas[y, x] = _LINE_COLOR

    save_canvas(canvas, ppm_filename)


def get_points(csv_filename):
    f = open(csv_filename, 'r')

    points = []
    for line in f:
        elements = line.split(";")
        points.append((int(elements[0]), int(elements[1])))

    f.close()
    return points


def get_canvas_bounds(points):
    min_x = points[0][0]
    min_y = points[0][1]
    max_x = points[0][0]
    max_y = points[0][1]

    for point in points:
        x = point[0]
        y = point[1]

        if x < min_x:
            min_x = x
        if x > max_x:
            max_x = x
        if y < min_y:
            min_y = y
        if y > max_y:
            max_y = y

    return [(min_x, min_y), (max_x, max_y)]


def save_canvas(canvas, ppm_filename):
    f = open(ppm_filename, 'w')

    width  = canvas.shape[1]
    height = canvas.shape[0]

    f.write(f"P3\n{width} {height}\n255\n");
    for line in canvas:
        for pixel in line:
            f.write(f"{pixel[0]} {pixel[1]} {pixel[2]} ")

    f.close()


if __name__ == "__main__":
    if (len(sys.argv)-1) != 2:
        print(f"{sys.argv[0]} {{.csv}} {{.ppm}}")
        exit(-1)

    csv_filename = sys.argv[1]
    ppm_filename = sys.argv[2]
    generate_image(csv_filename, ppm_filename)
