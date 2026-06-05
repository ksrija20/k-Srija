#include "canvas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initialize canvas with '_'
void init_canvas(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH]) {
    for (int y = 0; y < CANVAS_HEIGHT; y++) {
        for (int x = 0; x < CANVAS_WIDTH; x++) {
            canvas[y][x] = '_';
        }
    }
}

// Display the canvas with borders
void display_canvas(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH]) {
    // Top border
    printf("  +");
    for (int x = 0; x < CANVAS_WIDTH; x++) {
        printf("-");
    }
    printf("+\n");

    // Canvas rows
    for (int y = 0; y < CANVAS_HEIGHT; y++) {
        printf("%2d|", y); // Display row index
        for (int x = 0; x < CANVAS_WIDTH; x++) {
            putchar(canvas[y][x]);
        }
        printf("|\n");
    }

    // Bottom border
    printf("  +");
    for (int x = 0; x < CANVAS_WIDTH; x++) {
        printf("-");
    }
    printf("+\n");

    // X-axis coordinate guides (tens digit)
    printf("   ");
    for (int x = 0; x < CANVAS_WIDTH; x++) {
        if (x % 10 == 0) {
            printf("%d", x / 10);
        } else {
            printf(" ");
        }
    }
    printf("\n");

    // X-axis coordinate guides (ones digit)
    printf("   ");
    for (int x = 0; x < CANVAS_WIDTH; x++) {
        printf("%d", x % 10);
    }
    printf("\n\n");
}

// Clear the canvas and render all active shapes
void render_shapes(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH], Shape shapes[], int num_shapes) {
    // 1. Reset canvas
    init_canvas(canvas);

    // 2. Render each shape onto the canvas
    for (int i = 0; i < num_shapes; i++) {
        Shape s = shapes[i];
        switch (s.type) {
            case SHAPE_LINE:
                draw_line(canvas, s.data.line.x1, s.data.line.y1, s.data.line.x2, s.data.line.y2);
                break;
            case SHAPE_CIRCLE:
                draw_circle(canvas, s.data.circle.cx, s.data.circle.cy, s.data.circle.radius);
                break;
            case SHAPE_RECTANGLE:
                draw_rectangle(canvas, s.data.rect.x, s.data.rect.y, s.data.rect.width, s.data.rect.height);
                break;
            case SHAPE_TRIANGLE:
                draw_triangle(canvas, s.data.tri.x1, s.data.tri.y1, s.data.tri.x2, s.data.tri.y2, s.data.tri.x3, s.data.tri.y3);
                break;
        }
    }
}

// Bresenham's Line Algorithm
void draw_line(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH], int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1);
    int sy = y1 < y2 ? 1 : -1;
    int err = dx + dy;
    int e2;

    while (1) {
        if (x1 >= 0 && x1 < CANVAS_WIDTH && y1 >= 0 && y1 < CANVAS_HEIGHT) {
            canvas[y1][x1] = '*';
        }
        if (x1 == x2 && y1 == y2) {
            break;
        }
        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y1 += sy;
        }
    }
}

// Midpoint Circle Algorithm Helper
static void plot_circle_points(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH], int cx, int cy, int x, int y) {
    int points[8][2] = {
        {cx + x, cy + y}, {cx - x, cy + y}, {cx + x, cy - y}, {cx - x, cy - y},
        {cx + y, cy + x}, {cx - y, cy + x}, {cx + y, cy - x}, {cx - y, cy - x}
    };
    for (int i = 0; i < 8; i++) {
        int px = points[i][0];
        int py = points[i][1];
        if (px >= 0 && px < CANVAS_WIDTH && py >= 0 && py < CANVAS_HEIGHT) {
            canvas[py][px] = '*';
        }
    }
}

// Midpoint Circle Algorithm
void draw_circle(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH], int cx, int cy, int radius) {
    if (radius < 0) return;
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;
    plot_circle_points(canvas, cx, cy, x, y);
    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        plot_circle_points(canvas, cx, cy, x, y);
    }
}

// Rectangle drawing with outline borders
void draw_rectangle(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH], int x, int y, int w, int h) {
    if (w <= 0 || h <= 0) return;

    // Draw top and bottom horizontal borders
    for (int i = x; i < x + w; i++) {
        if (i >= 0 && i < CANVAS_WIDTH) {
            if (y >= 0 && y < CANVAS_HEIGHT) {
                canvas[y][i] = '*';
            }
            if (y + h - 1 >= 0 && y + h - 1 < CANVAS_HEIGHT) {
                canvas[y + h - 1][i] = '*';
            }
        }
    }

    // Draw left and right vertical borders
    for (int j = y; j < y + h; j++) {
        if (j >= 0 && j < CANVAS_HEIGHT) {
            if (x >= 0 && x < CANVAS_WIDTH) {
                canvas[j][x] = '*';
            }
            if (x + w - 1 >= 0 && x + w - 1 < CANVAS_WIDTH) {
                canvas[j][x + w - 1] = '*';
            }
        }
    }
}

// Triangle drawing using three line segments
void draw_triangle(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH], int x1, int y1, int x2, int y2, int x3, int y3) {
    draw_line(canvas, x1, y1, x2, y2);
    draw_line(canvas, x2, y2, x3, y3);
    draw_line(canvas, x3, y3, x1, y1);
}
