#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "canvas.h"

// Active shapes list
Shape shapes[MAX_SHAPES];
int num_shapes = 0;
int next_id = 1;

// Grid canvas
char canvas[CANVAS_HEIGHT][CANVAS_WIDTH];

// Helper function to read a validated integer within a range
int read_int(const char *prompt, int min_val, int max_val) {
    int val;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        char *endptr;
        val = (int)strtol(buffer, &endptr, 10);
        // Validate parsing
        if (endptr == buffer || (*endptr != '\n' && *endptr != '\0')) {
            printf("Error: Please enter a valid integer.\n");
            continue;
        }
        if (val < min_val || val > max_val) {
            printf("Error: Input must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        return val;
    }
}

// Print shapes currently in memory
void print_shapes_list() {
    printf("======================== ACTIVE SHAPES ========================\n");
    if (num_shapes == 0) {
        printf("  No shapes on canvas yet. Add some below!\n");
    } else {
        printf("  ID  | Type      | Parameters\n");
        printf("  ----+-----------+--------------------------------------------\n");
        for (int i = 0; i < num_shapes; i++) {
            Shape s = shapes[i];
            switch (s.type) {
                case SHAPE_LINE:
                    printf("  %-3d | Line      | Point 1: (%d, %d) -> Point 2: (%d, %d)\n",
                           s.id, s.data.line.x1, s.data.line.y1, s.data.line.x2, s.data.line.y2);
                    break;
                case SHAPE_CIRCLE:
                    printf("  %-3d | Circle    | Center: (%d, %d), Radius: %d\n",
                           s.id, s.data.circle.cx, s.data.circle.cy, s.data.circle.radius);
                    break;
                case SHAPE_RECTANGLE:
                    printf("  %-3d | Rectangle | Top-Left: (%d, %d), Size: %d x %d\n",
                           s.id, s.data.rect.x, s.data.rect.y, s.data.rect.width, s.data.rect.height);
                    break;
                case SHAPE_TRIANGLE:
                    printf("  %-3d | Triangle  | Vertices: (%d, %d), (%d, %d), (%d, %d)\n",
                           s.id, s.data.tri.x1, s.data.tri.y1, s.data.tri.x2, s.data.tri.y2, s.data.tri.x3, s.data.tri.y3);
                    break;
            }
        }
    }
    printf("===============================================================\n");
}

// Find a shape index in the array by ID. Returns -1 if not found.
int find_shape_index(int id) {
    for (int i = 0; i < num_shapes; i++) {
        if (shapes[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Delete a shape by ID
int delete_shape(int id) {
    int index = find_shape_index(id);
    if (index == -1) {
        return 0; // Not found
    }
    // Shift elements left to fill the gap
    for (int i = index; i < num_shapes - 1; i++) {
        shapes[i] = shapes[i + 1];
    }
    num_shapes--;
    return 1; // Success
}

int main() {
    int choice;

    while (1) {
        // Clear screen (using ANSI escape sequences to make it look professional)
        printf("\033[H\033[J");

        printf("===============================================================\n");
        printf("               2D TEXT GRAPHICS CANVAS EDITOR                  \n");
        printf("===============================================================\n");

        // Render shapes and display the canvas
        render_shapes(canvas, shapes, num_shapes);
        display_canvas(canvas);

        // List active shapes
        print_shapes_list();

        // Main Menu
        printf("\nMenu Actions:\n");
        printf("  1. Add Shape\n");
        printf("  2. Delete Shape\n");
        printf("  3. Modify Shape\n");
        printf("  4. Clear Canvas (Delete All)\n");
        printf("  5. Exit Program\n");
        printf("---------------------------------------------------------------\n");

        choice = read_int("Select action (1-5): ", 1, 5);

        if (choice == 5) {
            printf("\nExiting editor. Thank you for drawing!\n");
            break;
        }

        switch (choice) {
            case 1: { // ADD SHAPE
                if (num_shapes >= MAX_SHAPES) {
                    printf("Error: Maximum shapes count reached (%d). Delete some first.\n", MAX_SHAPES);
                    printf("Press Enter to continue...");
                    getchar();
                    break;
                }

                printf("\n--- Add a New Shape ---\n");
                printf("  1. Line\n");
                printf("  2. Circle\n");
                printf("  3. Rectangle\n");
                printf("  4. Triangle\n");
                int type_choice = read_int("Select shape type (1-4): ", 1, 4);

                Shape new_shape;
                new_shape.id = next_id++;

                if (type_choice == 1) {
                    new_shape.type = SHAPE_LINE;
                    new_shape.data.line.x1 = read_int("Enter Point 1 X (0-79): ", 0, CANVAS_WIDTH - 1);
                    new_shape.data.line.y1 = read_int("Enter Point 1 Y (0-19): ", 0, CANVAS_HEIGHT - 1);
                    new_shape.data.line.x2 = read_int("Enter Point 2 X (0-79): ", 0, CANVAS_WIDTH - 1);
                    new_shape.data.line.y2 = read_int("Enter Point 2 Y (0-19): ", 0, CANVAS_HEIGHT - 1);
                } else if (type_choice == 2) {
                    new_shape.type = SHAPE_CIRCLE;
                    new_shape.data.circle.cx = read_int("Enter Center X (0-79): ", 0, CANVAS_WIDTH - 1);
                    new_shape.data.circle.cy = read_int("Enter Center Y (0-19): ", 0, CANVAS_HEIGHT - 1);
                    new_shape.data.circle.radius = read_int("Enter Radius (1-40): ", 1, 40);
                } else if (type_choice == 3) {
                    new_shape.type = SHAPE_RECTANGLE;
                    new_shape.data.rect.x = read_int("Enter Top-Left X (0-79): ", 0, CANVAS_WIDTH - 1);
                    new_shape.data.rect.y = read_int("Enter Top-Left Y (0-19): ", 0, CANVAS_HEIGHT - 1);
                    new_shape.data.rect.width = read_int("Enter Width (1-80): ", 1, CANVAS_WIDTH);
                    new_shape.data.rect.height = read_int("Enter Height (1-20): ", 1, CANVAS_HEIGHT);
                } else if (type_choice == 4) {
                    new_shape.type = SHAPE_TRIANGLE;
                    new_shape.data.tri.x1 = read_int("Enter Vertex 1 X (0-79): ", 0, CANVAS_WIDTH - 1);
                    new_shape.data.tri.y1 = read_int("Enter Vertex 1 Y (0-19): ", 0, CANVAS_HEIGHT - 1);
                    new_shape.data.tri.x2 = read_int("Enter Vertex 2 X (0-79): ", 0, CANVAS_WIDTH - 1);
                    new_shape.data.tri.y2 = read_int("Enter Vertex 2 Y (0-19): ", 0, CANVAS_HEIGHT - 1);
                    new_shape.data.tri.x3 = read_int("Enter Vertex 3 X (0-79): ", 0, CANVAS_WIDTH - 1);
                    new_shape.data.tri.y3 = read_int("Enter Vertex 3 Y (0-19): ", 0, CANVAS_HEIGHT - 1);
                }

                shapes[num_shapes++] = new_shape;
                printf("\nShape added successfully! ID: %d\n", new_shape.id);
                printf("Press Enter to update canvas...");
                getchar();
                break;
            }

            case 2: { // DELETE SHAPE
                if (num_shapes == 0) {
                    printf("\nError: No shapes to delete.\n");
                    printf("Press Enter to continue...");
                    getchar();
                    break;
                }
                printf("\n--- Delete Shape ---\n");
                int delete_id = read_int("Enter the ID of the shape to delete: ", 1, INT_MAX);
                if (delete_shape(delete_id)) {
                    printf("\nShape ID %d deleted successfully.\n", delete_id);
                } else {
                    printf("\nError: Shape with ID %d not found.\n", delete_id);
                }
                printf("Press Enter to update canvas...");
                getchar();
                break;
            }

            case 3: { // MODIFY SHAPE
                if (num_shapes == 0) {
                    printf("\nError: No shapes to modify.\n");
                    printf("Press Enter to continue...");
                    getchar();
                    break;
                }
                printf("\n--- Modify Shape ---\n");
                int modify_id = read_int("Enter the ID of the shape to modify: ", 1, INT_MAX);
                int idx = find_shape_index(modify_id);
                if (idx == -1) {
                    printf("\nError: Shape with ID %d not found.\n", modify_id);
                    printf("Press Enter to continue...");
                    getchar();
                    break;
                }

                Shape *s = &shapes[idx];
                printf("\nModifying shape ID %d (%s):\n", s->id,
                       s->type == SHAPE_LINE ? "Line" :
                       s->type == SHAPE_CIRCLE ? "Circle" :
                       s->type == SHAPE_RECTANGLE ? "Rectangle" : "Triangle");

                if (s->type == SHAPE_LINE) {
                    printf("Current: Point 1 (%d, %d) to Point 2 (%d, %d)\n",
                           s->data.line.x1, s->data.line.y1, s->data.line.x2, s->data.line.y2);
                    s->data.line.x1 = read_int("Enter New Point 1 X (0-79): ", 0, CANVAS_WIDTH - 1);
                    s->data.line.y1 = read_int("Enter New Point 1 Y (0-19): ", 0, CANVAS_HEIGHT - 1);
                    s->data.line.x2 = read_int("Enter New Point 2 X (0-79): ", 0, CANVAS_WIDTH - 1);
                    s->data.line.y2 = read_int("Enter New Point 2 Y (0-19): ", 0, CANVAS_HEIGHT - 1);
                } else if (s->type == SHAPE_CIRCLE) {
                    printf("Current: Center (%d, %d), Radius: %d\n",
                           s->data.circle.cx, s->data.circle.cy, s->data.circle.radius);
                    s->data.circle.cx = read_int("Enter New Center X (0-79): ", 0, CANVAS_WIDTH - 1);
                    s->data.circle.cy = read_int("Enter New Center Y (0-19): ", 0, CANVAS_HEIGHT - 1);
                    s->data.circle.radius = read_int("Enter New Radius (1-40): ", 1, 40);
                } else if (s->type == SHAPE_RECTANGLE) {
                    printf("Current: Top-Left (%d, %d), Size: %d x %d\n",
                           s->data.rect.x, s->data.rect.y, s->data.rect.width, s->data.rect.height);
                    s->data.rect.x = read_int("Enter New Top-Left X (0-79): ", 0, CANVAS_WIDTH - 1);
                    s->data.rect.y = read_int("Enter New Top-Left Y (0-19): ", 0, CANVAS_HEIGHT - 1);
                    s->data.rect.width = read_int("Enter New Width (1-80): ", 1, CANVAS_WIDTH);
                    s->data.rect.height = read_int("Enter New Height (1-20): ", 1, CANVAS_HEIGHT);
                } else if (s->type == SHAPE_TRIANGLE) {
                    printf("Current: Vertices (%d, %d), (%d, %d), (%d, %d)\n",
                           s->data.tri.x1, s->data.tri.y1, s->data.tri.x2, s->data.tri.y2, s->data.tri.x3, s->data.tri.y3);
                    s->data.tri.x1 = read_int("Enter New Vertex 1 X (0-79): ", 0, CANVAS_WIDTH - 1);
                    s->data.tri.y1 = read_int("Enter New Vertex 1 Y (0-19): ", 0, CANVAS_HEIGHT - 1);
                    s->data.tri.x2 = read_int("Enter New Vertex 2 X (0-79): ", 0, CANVAS_WIDTH - 1);
                    s->data.tri.y2 = read_int("Enter New Vertex 2 Y (0-19): ", 0, CANVAS_HEIGHT - 1);
                    s->data.tri.x3 = read_int("Enter New Vertex 3 X (0-79): ", 0, CANVAS_WIDTH - 1);
                    s->data.tri.y3 = read_int("Enter New Vertex 3 Y (0-19): ", 0, CANVAS_HEIGHT - 1);
                }

                printf("\nShape modified successfully!\n");
                printf("Press Enter to update canvas...");
                getchar();
                break;
            }

            case 4: { // CLEAR CANVAS
                int confirm = read_int("\nAre you sure you want to clear the canvas? (1=Yes, 0=No): ", 0, 1);
                if (confirm == 1) {
                    num_shapes = 0;
                    printf("\nCanvas cleared successfully.\n");
                } else {
                    printf("\nAction cancelled.\n");
                }
                printf("Press Enter to continue...");
                getchar();
                break;
            }
        }
    }

    return 0;
}
