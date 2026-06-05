#ifndef CANVAS_H
#define CANVAS_H

#define CANVAS_WIDTH 80
#define CANVAS_HEIGHT 20
#define MAX_SHAPES 100

typedef enum {
    SHAPE_LINE,
    SHAPE_CIRCLE,
    SHAPE_RECTANGLE,
    SHAPE_TRIANGLE
} ShapeType;

typedef struct {
    int x1, y1;
    int x2, y2;
} LineData;

typedef struct {
    int cx, cy;
    int radius;
} CircleData;

typedef struct {
    int x, y;
    int width, height;
} RectData;

typedef struct {
    int x1, y1;
    int x2, y2;
    int x3, y3;
} TriData;

typedef struct {
    int id;
    ShapeType type;
    union {
        LineData line;
        CircleData circle;
        RectData rect;
        TriData tri;
    } data;
} Shape;

// Initialize canvas with '_'
void init_canvas(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH]);

// Display the canvas
void display_canvas(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH]);

// Clear the canvas and render all active shapes
void render_shapes(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH], Shape shapes[], int num_shapes);

// Basic shape drawing algorithms (rasterization)
void draw_line(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH], int x1, int y1, int x2, int y2);
void draw_circle(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH], int cx, int cy, int radius);
void draw_rectangle(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH], int x, int y, int w, int h);
void draw_triangle(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH], int x1, int y1, int x2, int y2, int x3, int y3);

#endif // CANVAS_H
