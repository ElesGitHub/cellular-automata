#include <assert.h>
#include <stdio.h>

#include <raylib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define GRID_WIDTH  40
#define GRID_HEIGHT 30

#define CELL_SIZE (SCREEN_WIDTH / GRID_WIDTH)
#define CELL_MARGIN (CELL_SIZE / 10)

typedef struct {
    char values[GRID_HEIGHT][GRID_WIDTH];
} Board;

Board parseBoard(char s[GRID_HEIGHT][GRID_WIDTH+1], char alive, char dead) {
    assert(alive != dead);
    Board board = {0};
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (s[y][x] == alive)
                board.values[y][x] = 1;
            else if (s[y][x] != dead)
                goto error;
        }
    }
    return board;

    error:
    printf("An error occurred while parsing board");
    return (Board){0};
}

void printBoard(Board board) {
    printf("\x1b[H");
    for (int y = 0; y < GRID_HEIGHT; y++) {
        char line[GRID_WIDTH] = {0};
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (board.values[y][x]) line[x] = '#';
            else                    line[x] = '.';
        }
        printf("%.*s\n", GRID_WIDTH, line);
    }
}

void drawBoard(Board board) {
    ClearBackground(BLACK);
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            DrawRectangle(
                x * CELL_SIZE + CELL_MARGIN, y * CELL_SIZE + CELL_MARGIN,
                CELL_SIZE - 2 * CELL_MARGIN, CELL_SIZE - 2 * CELL_MARGIN,
                board.values[y][x] ? WHITE : DARKGRAY
            );
        }
    }
}

void nextState(Board *board) {
    Board old = *board;
    
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            int neighbours = 0;
            for (int dy = -1; dy <= 1; dy++) {
                int ny = (y + dy + GRID_HEIGHT) % GRID_HEIGHT;
                for (int dx = -1; dx <= 1; dx++) {
                    int nx = (x + dx + GRID_WIDTH) % GRID_WIDTH;
                    if (old.values[ny][nx]) neighbours++;
                }
            }

            if (old.values[y][x]) {
                neighbours--;
                if (neighbours < 2 || neighbours > 3) board->values[y][x] = 0;
                else                                  board->values[y][x] = 1;
            } else {
                if (neighbours == 3) board->values[y][x] = 1;
                else                 board->values[y][x] = 0;
            }
        }
    }
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cellular automata");
    SetTargetFPS(20);

    char template[GRID_HEIGHT][GRID_WIDTH+1] = {
        "........................................",
        "........................................",
        "........................................",
        "..#.....................................",
        "...#....................................",
        ".###....................................",
        "........................................",
        "........................................",
        "........................................",
        "........................................",
        "........................................",
        "........................................",
        "..#.....................................",
        "...#....................................",
        ".###....................................",
        "........................................",
        "........................................",
        "........................................",
        "........................................",
        "........................................",
        "........................................",
        "..#.....................................",
        "...#....................................",
        ".###....................................",
        "........................................",
        "........................................",
        "........................................",
        "........................................",
        "........................................",
        "........................................"
    };
    Board board = parseBoard(template, '#', '.');

    while (!WindowShouldClose()) {
        BeginDrawing();
        drawBoard(board);
        nextState(&board);
        EndDrawing();
    }

    return 0;
}